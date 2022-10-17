#include "ramproject.h"

#include "ramses.h"
#include "rampipe.h"
#include "projecteditwidget.h"
#include "ramshot.h"

QFrame *RamProject::ui_editWidget = nullptr;

QHash<QString, RamProject*> RamProject::m_existingObjects = QHash<QString, RamProject*>();

RamProject *RamProject::get(QString uuid)
{
    if (!checkUuid(uuid, Project)) return nullptr;

    RamProject *p = m_existingObjects.value(uuid);
    if (p) return p;

    // Finally return a new instance
    return new RamProject(uuid);
}

RamProject *RamProject::c(RamObject *o)
{
    // return qobject_cast<RamProject*>(o);
    // For performance, reinterpret_cast, but be careful with the object passed!
    return reinterpret_cast<RamProject*>(o);
}

// PUBLIC //

RamProject::RamProject(QString shortName, QString name):
    RamObject(shortName, name, Project)
{
    construct();
}

RamProject::RamProject(QString uuid):
    RamObject(uuid, Project)
{
    construct();

    QJsonObject d = data();
    loadModel(m_shots, "shots", d);
    loadModel(m_assets, "assets", d);
    loadModel(m_users, "users", d);
    loadModel(m_scheduleComments, "scheduleComments", d);
    loadModel(m_pipeFiles, "pipeFiles", d);
    loadModel(m_steps, "steps", d);
    loadModel(m_pipeline, "pipeline", d);
    loadModel(m_sequences, "sequences", d);
    loadModel(m_assetGroups, "assetGroups", d);

    computeEstimation(true);
}

RamObjectModel *RamProject::steps() const
{
    return m_steps;
}

RamObjectModel *RamProject::assetGroups() const
{
    return m_assetGroups;
}

RamObjectModel *RamProject::sequences() const
{
    return m_sequences;
}

RamObjectModel *RamProject::shots() const
{
    return m_shots;
}

RamObjectModel *RamProject::assets() const
{
    return m_assets;
}

RamObjectModel *RamProject::pipeline() const
{
    return m_pipeline;
}

RamObjectModel *RamProject::pipeFiles() const
{
    return m_pipeFiles;
}

RamObjectModel *RamProject::users() const
{
    return m_users;
}

RamObjectModel *RamProject::scheduleComments() const
{
    return m_scheduleComments;
}

qreal RamProject::framerate() const
{
    return getData("framerate").toDouble(24);
}

void RamProject::setFramerate(const qreal &newFramerate)
{
    insertData("framerate", newFramerate);
}

int RamProject::width() const
{
    return getData("width").toInt(1920);
}

void RamProject::setWidth(const int width, const qreal &pixelAspect)
{
    insertData("width", width);
    updateAspectRatio(pixelAspect);
}

int RamProject::height() const
{
    return getData("height").toInt(1080);
}

void RamProject::setHeight(const int height, const qreal &pixelAspect)
{
    insertData("height", height);
    updateAspectRatio(pixelAspect);
}

qreal RamProject::aspectRatio() const
{
    QJsonObject d = data();
    double w = d.value("width").toDouble(1920);
    double h = d.value("height").toDouble(1080);
    return d.value("aspectRatio").toDouble(w/h);
}

void RamProject::updateAspectRatio(const qreal &pixelAspect)
{
    QJsonObject d = data();
    qreal w = d.value("width").toDouble(1920);
    qreal h = d.value("height").toDouble(1080);
    qreal r = w / h * pixelAspect;
    setAspectRatio(r);
}

void RamProject::setAspectRatio(const qreal &aspectRatio)
{
    insertData("aspectRatio", aspectRatio);
}

QDate RamProject::deadline() const
{
    return QDate::fromString( getData("deadline").toString(), "yyyy-MM-dd");
}

void RamProject::setDeadline(const QDate &newDeadline)
{
    insertData("deadline", newDeadline.toString("yyyy-MM-dd"));
}

double RamProject::duration() const
{
    double duration = 0;
    for (int i = 0; i < m_sequences->rowCount(); i++)
    {
        RamShot *shot = RamShot::c( m_sequences->get(i) );
        if (shot) duration += shot->duration();
    }
    return duration;
}

RamPipe *RamProject::pipe(RamStep *outputStep, RamStep *inputStep)
{
    for (int i = 0; i < m_pipeline->rowCount(); i++)
    {
        RamPipe *p = RamPipe::c( m_pipeline->get(i) );
        if ( p->outputStep()->is(outputStep) && p->inputStep()->is(inputStep) ) return p;
    }
    return nullptr;
}

void RamProject::freezeEstimations(bool freeze, bool reCompute)
{
    m_freezeEstimations = freeze;

    // Freeze steps
    for(int i = 0; i < m_steps->rowCount(); i++)
    {
        RamStep *step = RamStep::c(m_steps->get(i));
        step->freezeEstimations(freeze, reCompute);
    }

    // No need to recompute, it's triggered by the steps when unfreezing
}

qint64 RamProject::timeSpent() const
{
    return m_timeSpent;
}

float RamProject::estimation() const
{
    return m_estimation;
}

int RamProject::completionRatio() const
{
    return m_completionRatio;
}

float RamProject::latenessRatio() const
{
    return m_latenessRatio;
}

float RamProject::assignedDays() const
{
    return m_assignedDays;
}

float RamProject::unassignedDays() const
{
    return m_missingDays;
}

QVector<float> RamProject::stats(RamUser *user)
{
    QVector<float> s(4);
    for (int i = 0; i < m_steps->rowCount(); i++)
    {
        RamStep *step = RamStep::c(m_steps->get(i));
        if (!step) continue;
        QVector<float> stepStats = step->stats(user);
        s[0] = s.at(0) + stepStats.at(0);
        s[1] = s.at(1) + stepStats.at(1);
        s[2] = s.at(2) + stepStats.at(2);
        s[3] = s.at(3) + stepStats.at(3);
    }
    return s;
}

void RamProject::setFolderPath(const QString &newFolderPath)
{
    if (newFolderPath == folderPath()) return;

    // Store this as a local setting
    QSettings settings;
    settings.beginGroup("projects");
    settings.beginGroup(m_uuid);
    settings.setValue("path", newFolderPath);
    settings.endGroup();
    settings.endGroup();

    emit dataChanged(this);
}

void RamProject::resetDbFolderPath()
{
    setFolderPath(dbFolderPath());
}

QString RamProject::defaultPath() const
{
    return Ramses::instance()->path(RamObject::ProjectsFolder) + "/" + shortName();
}

bool RamProject::pathIsDefault(QString p) const
{
    return p == "" || p.toLower() == "auto";
}

QString RamProject::dbFolderPath() const
{
    return getData("path").toString("auto");
}

void RamProject::setDbFolderPath(const QString &newDbFolderPath)
{
    insertData("path", newDbFolderPath);
}

QString RamProject::details() const
{
    return QString::number(width()) +
            " x " +
            QString::number(height()) +
            " (" +
            QString::number(aspectRatio(),'f',2) +
            ":1)" +
            " @ " +
            QString::number(framerate(), 'f', 2) +
            "fps";
}

QStringList RamProject::filterListUuids() const
{
    // Return the list of user uuids
    QStringList userUuids;
    for (int i = 0; i < m_users->rowCount(); i++)
    {
        userUuids << m_users->get(i)->uuid();
    }
    return userUuids;
}

// PUBLIC SLOTS //

void RamProject::updatePath()
{
    QString path = folderPath();
    if (path == "") path = "auto";
    insertData("path", path);
}

void RamProject::edit(bool show)
{
    if (!ui_editWidget) ui_editWidget = createEditFrame(new ProjectEditWidget());

    if (show) showEdit( ui_editWidget );
}

void RamProject::computeEstimation(bool recompute)
{
    if (m_freezeEstimations) return;
    m_timeSpent = 0;
    m_estimation = 0;
    m_completionRatio = 0;
    m_latenessRatio = 0;
    m_assignedDays = 0;
    m_missingDays = 0;
    int numItems = 0;


    for (int i =0; i < m_steps->rowCount(); i++)
    {
        RamStep *step = RamStep::c(m_steps->get(i));

        if (!step) continue;

        if (recompute)
        {
            step->computeEstimation();
            step->countAssignedDays();
        }

        //Ignore pre and post procution
        if ( step->type() != RamStep::ShotProduction && step->type() != RamStep::AssetProduction) continue;

        m_timeSpent += step->timeSpent();
        m_estimation += step->estimation();
        m_completionRatio += step->completionRatio();
        m_latenessRatio += step->latenessRatio();
        m_assignedDays += step->assignedDays();
        m_missingDays += step->unassignedDays();

        numItems++;
    }

    if (numItems > 0)
    {
        m_completionRatio /= numItems;
        m_latenessRatio /= numItems;
    }
    else
    {
        m_completionRatio  = 100;
        m_latenessRatio = 1;
    }

    emit completionRatioChanged(m_completionRatio);
    emit latenessRatioChanged(m_latenessRatio);
    emit timeSpentChanged(m_timeSpent);
    emit estimationChanged(m_estimation);
    emit estimationComputed(this);
}

// PROTECTED //

QString RamProject::folderPath() const
{
    // Get path in settings first
    QSettings settings;
    settings.beginGroup("projects");
    settings.beginGroup(m_uuid);
    QString p = settings.value("path", "no-path").toString();
    settings.endGroup();
    settings.endGroup();

    if (p == "no-path")
    {
        // Then return the DB path
        p = dbFolderPath();
    }

    if (pathIsDefault(p))
    {
        return defaultPath();
    }
    else return p;
}

// PRIVATE //

void RamProject::construct()
{
    m_existingObjects[m_uuid] = this;
    m_icon = ":/icons/project";
    m_editRole = ProjectAdmin;

    m_shots = createModel(RamObject::Shot, "shots");
    m_assets = createModel(RamObject::Asset, "assets");
    m_users = createModel(RamObject::User, "users" );
    m_scheduleComments = createModel(RamObject::ScheduleComment, "scheduleComments" );
    m_pipeFiles = createModel(RamObject::PipeFile, "pipeFiles" );
    m_steps = createModel(RamObject::Step, "steps" );
    m_pipeline = createModel(RamObject::Pipe, "pipeline" );
    m_sequences = createModel(RamObject::Sequence, "sequences" );
    m_assetGroups = createModel(RamObject::AssetGroup, "assetGroups" );

    m_shots->setColumnModel(m_steps);
    m_assets->setColumnModel(m_steps);
}

