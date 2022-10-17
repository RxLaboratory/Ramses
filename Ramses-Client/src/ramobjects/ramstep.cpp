#include "ramstep.h"

#include "ramapplication.h"
#include "ramasset.h"
#include "ramassetgroup.h"
#include "ramscheduleentry.h"
#include "ramses.h"
#include "ramstatus.h"
#include "ramnamemanager.h"
#include "ramworkingfolder.h"
#include "stepeditwidget.h"
#include "ramshot.h"

// STATIC //

QFrame *RamStep::ui_editWidget = nullptr;

QHash<QString, RamStep*> RamStep::m_existingObjects = QHash<QString, RamStep*>();

RamStep *RamStep::get(QString uuid)
{
    if (!checkUuid(uuid, Step)) return nullptr;

    RamStep *s = m_existingObjects.value(uuid);
    if (s) return s;

    // Finally return a new instance
    return new RamStep(uuid);
}

RamStep *RamStep::c(RamObject *o)
{
    //return qobject_cast<RamStep*>(o);
    // For performance, reinterpret_cast, but be careful with the object passed!
    return reinterpret_cast<RamStep*>(o);
}

RamStep *RamStep::createFromTemplate(RamTemplateStep *tempStep, RamProject *project)
{
    RamStep *step = new RamStep(tempStep->shortName(), tempStep->name(), project);

    // Copy data

    // populate apps list
    RamObjectModel *apps = tempStep->applications();
    for (int i = 0; i < apps->rowCount(); i++)
    {
        step->applications()->appendObject(apps->get(i)->uuid());
    }

    // Set new data
    QJsonObject tempD = tempStep->data();
    tempD.insert("project", project->uuid());
    step->setData(tempD);

    return step;
}

// PUBLIC //

RamStep::RamStep(QString shortName, QString name, RamProject *project):
    RamTemplateStep(shortName, name, Step)
{
    construct();

    QJsonObject d = data();
    d.insert("project", project->uuid());

    setData(d);
}

RamStep::RamStep(QString uuid):
    RamTemplateStep(uuid, Step)
{
    construct();
}

RamProject *RamStep::project() const
{
    QString projUuid = getData("project").toString();
    return RamProject::get( projUuid );
}

RamAssetGroup *RamStep::estimationMultiplyGroup() const
{
    return RamAssetGroup::get( getData("estimationMultiplyGroup").toString("none") );
}

void RamStep::setEstimationMultiplyGroup(RamObject *newEstimationMultiplyGroup)
{
    if (newEstimationMultiplyGroup) insertData("estimationMultiplyGroup", newEstimationMultiplyGroup->uuid() );
    else insertData("estimationMultiplyGroup", "none" );

    m_estimationChanged = true;
    emit estimationComputed(this);
}

qint64 RamStep::timeSpent() const
{
    return m_timeSpent;
}

float RamStep::estimation() const
{
    return m_estimation;
}

float RamStep::completionRatio() const
{
    return m_completionRatio;
}

float RamStep::latenessRatio() const
{
    if (neededDays() > 0)
        return missingDays() / neededDays();
    else return 0;
}

float RamStep::assignedDays() const
{
    return m_scheduledHalfDays/2.0;
}

float RamStep::unassignedDays() const
{
    return m_missingDays;
}

float RamStep::missingDays() const
{
    return neededDays() - m_scheduledFutureHalfDays/2.0;
}

float RamStep::daysSpent() const
{
    return m_estimation * m_completionRatio / 100;
}

float RamStep::neededDays() const
{
    return m_estimation - daysSpent();
}

QVector<float> RamStep::stats(RamUser *user)
{
    if (!user)
    {
        return QVector<float>( { m_estimation,
                                 m_estimation * m_completionRatio / 100,
                                 static_cast<float>(m_scheduledHalfDays/2),
                                 static_cast<float>(m_scheduledFutureHalfDays/2)
                               } );
    }

    RamProject *proj = project();
    if (!proj) {
        return QVector<float>( 4 );
    }

    float assignedHalfDays = 0;
    float assignedFutureHalfDays = 0;

    // Count assigned and future days
    for (int j = 0; j < user->schedule()->rowCount(); j++)
    {
        RamScheduleEntry *entry = RamScheduleEntry::c( user->schedule()->get(j) );
        if (!entry) continue;
        if (this->is(entry->step()))
        {
            assignedHalfDays++;
            if (entry->date() > QDateTime::currentDateTime()) assignedFutureHalfDays++;
        }
    }

    // check completed days
    RamObjectModel *items;
    if (type() == ShotProduction) items = proj->shots();
    else if(type() == AssetProduction) items = proj->assets();
    else return QVector<float>( {
                                    0,
                                    0,
                                    static_cast<float>(assignedHalfDays / 2.0),
                                    static_cast<float>(assignedFutureHalfDays / 2.0) }
                                );

    float estimation = 0;
    float completedDays = 0;

    RamState *no = Ramses::instance()->noState();
    Type t = type();

    for (int i =0; i < items->rowCount(); i++)
    {
        RamAbstractItem *item;
        if (t == ShotProduction) item = RamShot::c(items->get(i));
        else item = RamAsset::c( items->get(i) );

        RamStatus *status = item->status(this);

        if (!status) continue;
        if (no->is(status->state())) continue;
        if (!status->assignedUser()) continue;
        if (!status->assignedUser()->is(user)) continue;

        float estim = 0;
        if (status->useAutoEstimation()) estim = status->estimation();
        else estim = status->goal();

        completedDays += estim * status->completionRatio() / 100.0;
        estimation += estim;
    }

    return QVector<float>( {
                               estimation,
                               completedDays,
                               static_cast<float>(assignedHalfDays / 2.0),
                               static_cast<float>(assignedFutureHalfDays / 2.0)
                           } );
}

void RamStep::freezeEstimations(bool freeze, bool reCompute)
{
    m_freezeEstimations = freeze;
    if (!freeze && reCompute)
    {
        this->computeEstimation();
        this->countAssignedDays();
    }
}

void RamStep::openFile(QString filePath) const
{
    // Get the application
    for (int i = 0; i < m_applications->rowCount(); i++)
    {
        RamApplication *app = RamApplication::c( m_applications->get(i) );
        if (app->open( filePath )) return;
    }

    // Try with the default app on the system
    QDesktopServices::openUrl(QUrl("file:///" + filePath));
}

QSet<RamWorkingFolder> RamStep::templateWorkingFolders() const
{
    QString templatesPath = path(TemplatesFolder);
    QDir dir(templatesPath);
    QStringList subdirs = dir.entryList( QDir::Dirs | QDir::NoDotAndDotDot );

    RamNameManager nm;
    QSet<RamWorkingFolder> templateFolders;
    foreach (QString subdir, subdirs)
    {
        // check name
        if (nm.setFileName(subdir))
            templateFolders.insert(RamWorkingFolder( templatesPath + "/" + subdir ));
    }

    return templateFolders;
}

QSet<RamObject *> RamStep::inputFileTypes()
{
    QSet<RamObject *> fts;

    for ( int i = 0; i < m_applications->rowCount(); i++)
    {
        RamApplication *app = RamApplication::c( m_applications->get(i) );
        for (int f = 0; f < app->importFileTypes()->rowCount(); f++)
        {
            RamFileType *ft = RamFileType::c( app->importFileTypes()->get(f) );
            fts << ft;
        }
        for (int f = 0; f < app->nativeFileTypes()->rowCount(); f++)
        {
            RamFileType *ft = RamFileType::c( app->nativeFileTypes()->get(f) );
            fts << ft;
        }
    }

    return fts;
}

QSet<RamObject *> RamStep::outputFileTypes()
{
    QSet<RamObject *> fts;

    for ( int i = 0; i < m_applications->rowCount(); i++)
    {
        RamApplication *app = RamApplication::c( m_applications->get(i) );
        for (int f = 0; f < app->exportFileTypes()->rowCount(); f++)
        {
            RamFileType *ft = RamFileType::c( app->exportFileTypes()->get(f) );
            fts << ft;
        }
        for (int f = 0; f < app->nativeFileTypes()->rowCount(); f++)
        {
            RamFileType *ft = RamFileType::c( app->nativeFileTypes()->get(f) );
            fts << ft;
        }
    }

    return fts;
}

// PUBLIC SLOTS //

void RamStep::edit(bool show)
{
    if (!ui_editWidget) ui_editWidget = createEditFrame(new StepEditWidget());

    if (show) showEdit( ui_editWidget );
}

void RamStep::computeEstimation()
{
    if (m_freezeEstimations) return;

    RamProject *proj = project();
    if (!proj) return;

    Type t = type();

    if (t == PreProduction) return;
    if (t == PostProduction) return;

    RamObjectModel *items;
    if (t == ShotProduction) items = proj->shots();
    else items = proj->assets();

    m_timeSpent = 0;
    m_estimation = 0;
    m_completionRatio = 0;
    m_latenessRatio = 0;
    int numItems = 0;

    float completedDays = 0;

    RamState *no = Ramses::instance()->noState();

    for (int i =0; i < items->rowCount(); i++)
    {
        RamAbstractItem *item;
        if (t == ShotProduction) item = RamShot::c( items->get(i) );
        else item = RamAsset::c( items->get(i) );
        RamStatus *status = item->status(this);

        if (!status) continue;

        if (no->is(status->state())) continue;

        m_timeSpent += status->timeSpent();

        float estimation = 0;
        if (status->useAutoEstimation()) estimation = status->estimation();
        else estimation = status->goal();

        completedDays += estimation * status->completionRatio() / 100.0;
        m_estimation += estimation;

        m_latenessRatio += status->latenessRatio();

        numItems++;
    }

    if (numItems > 0) m_latenessRatio /= numItems;
    else m_latenessRatio = 1;

    if (m_estimation > 0) m_completionRatio = completedDays / m_estimation * 100;
    else m_completionRatio = 100;

    m_completionRatio = std::min(100, (int)m_completionRatio);

    // update missing days
    m_missingDays = m_estimation - m_scheduledHalfDays/2.0;

    proj->computeEstimation();
    emit estimationComputed(this);
}

void RamStep::countAssignedDays()
{
    if (m_freezeEstimations) return;
    RamProject *proj = project();
    if (!proj) return;

    m_scheduledHalfDays = 0;
    m_scheduledFutureHalfDays = 0;

    for (int i = 0; i < proj->users()->rowCount(); i++)
    {
        RamUser *u = RamUser::c( proj->users()->get(i) );
        if (!u) continue;

        for (int j = 0; j < u->schedule()->rowCount(); j++)
        {
            RamScheduleEntry *entry = RamScheduleEntry::c( u->schedule()->get(j) );
            if (!entry) continue;
            if (this->is(entry->step()))
            {
                m_scheduledHalfDays++;
                if (entry->date() > QDateTime::currentDateTime()) m_scheduledFutureHalfDays++;
            }
        }
    }

    // update missing
    m_missingDays = m_estimation - m_scheduledHalfDays/2.0;
    proj->computeEstimation();
    emit estimationComputed(this);
}

// PROTECTED //

QString RamStep::folderPath() const
{
    RamProject *proj = project();
    if (!proj) return "";

    if (type() == RamStep::PreProduction)
        return proj->path(RamObject::PreProdFolder) + "/" + proj->shortName() + "_G_" + shortName();

    else if (type() == RamStep::PostProduction)
        return proj->path(RamObject::PostProdFolder) + "/" + proj->shortName() + "_G_" + shortName();

    else
        return proj->path(RamObject::ProdFolder) + "/" + proj->shortName() + "_G_" + shortName();
}

// PRIVATE //

void RamStep::construct()
{
    m_existingObjects[m_uuid] = this;
    m_objectType = Step;
    m_editRole = RamObject::ProjectAdmin;
}
