#include "ramabstractitem.h"

#include "duqf-utils/guiutils.h"
#include "mainwindow.h"
#include "ramses.h"

// STATIC //

StepStatusHistoryWidget *RamAbstractItem::ui_historyWidget = nullptr;

// PUBLIC //

RamAbstractItem::RamAbstractItem(QString shortName, QString name, ObjectType type, RamProject *project) :
    RamObject(shortName, name, type, project)
{
    construct();

    QJsonObject d = data();
    d.insert("project", project->uuid());
    connectProject(project);

    // Prepare history
    for (int i = 0; i < project->steps()->rowCount(); i++)
    {
        RamStep *step = RamStep::c( project->steps()->get(i) );
        createStepHistory(step, d);
    }
    setData(d);

    connectProject(project);
}

RamObject *RamAbstractItem::objectForColumn(QString columnUuid) const
{
    if (columnUuid == "") return nullptr;
    RamStep *step = RamStep::get( columnUuid );
    return status(step);
}

RamAbstractItem::RamAbstractItem(QString uuid, ObjectType type):
    RamObject(uuid, type)
{
    construct();

    QJsonObject d = data();

    // Get project first
    QString projectUuid = d.value("project").toString("");
    if (projectUuid != "")
    {
        RamProject *project = RamProject::get(projectUuid);
        setParent(project);
        if (project)
        {
            RamObjectModel *steps = project->steps();
            if (steps) {
                for (int i = 0; i < project->steps()->rowCount(); i++)
                {
                    RamStep *step = RamStep::c( project->steps()->get(i) );
                    createStepHistory(step, d);
                }
            }
        }


        connectProject(project);
    }
}

void RamAbstractItem::stepInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);

    RamProject *p = RamProject::get( getData("project").toString("none") );
    if (!p) return;

    for (int i = first; i <= last; i++)
    {
        RamStep *step = RamStep::c( p->steps()->get(i) );
        if (!step) return;
        RamObjectModel *history = statusHistory(step);
        if (!history) createStepHistory(step);
    }
}

void RamAbstractItem::stepRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);

    RamProject *p = RamProject::get( getData("project").toString("none") );
    if (!p) return;

    for (int i = first; i <= last; i++)
    {
        RamObject *step = p->steps()->get(i);
        if (!step) return;
        RamObjectModel *history = m_history.take( step->uuid() );
        if (history) history->deleteLater();
    }
}

RamProject *RamAbstractItem::project() const
{
    return RamProject::get( getData("project").toString("none") );
}

QMap<QString, RamObjectModel*> RamAbstractItem::statusHistory() const
{
    return m_history;
}

RamObjectModel *RamAbstractItem::statusHistory(RamObject *stepObj) const
{
    if (!stepObj) return nullptr;

    return m_history.value(stepObj->uuid(), nullptr);
}

RamStatus *RamAbstractItem::setStatus(RamUser *user, RamState *state, RamStep *step, int completionRatio, QString comment, int version)
{
    RamStatus *newStatus = new RamStatus(user, this, step );
    newStatus->setState(state);
    if (completionRatio >= 0) newStatus->setCompletionRatio(completionRatio);
    if (comment != "") newStatus->setComment(comment);
    newStatus->setVersion(version);

    addStatus(newStatus);

    return newStatus;
}

void RamAbstractItem::addStatus(RamStatus *status)
{
    RamStep *step = status->step();
    if (!step) return;

    // Check if there's a user assigned
    if (!status->assignedUser())
        status->assignUser( assignedUser(step) );

    RamObjectModel *history = statusHistory(step);
    if (history) history->appendObject( status->uuid() );
}

RamStatus *RamAbstractItem::status(RamStep *step) const
{
    // Get the current status
    RamObjectModel *history = statusHistory(step);
    if (!history) return nullptr;
    if (history->rowCount() == 0) return nullptr;

    RamStatus *currentStatus = RamStatus::c ( history->get(history->rowCount() - 1) );
    if (!currentStatus) return nullptr;

    return currentStatus;
}

QVector<RamStatus *> RamAbstractItem::status()
{
    QVector<RamStatus *> statuses;
    if (m_history.isEmpty()) return statuses;
    QMapIterator<QString, RamObjectModel*> i(m_history);
    while(i.hasNext())
    {
        i.next();
        RamObjectModel *h = i.value();
        statuses << qobject_cast<RamStatus*>( h->get(h->rowCount() - 1) );
    }
    return statuses;
}

RamUser *RamAbstractItem::assignedUser(RamStep *step)
{
    RamStatus *st = status(step);
    if (st)
        return st->assignedUser();

    return nullptr;
}

bool RamAbstractItem::isUserAssigned(RamObject *u, RamStep *step)
{
    if(step)
    {
        RamStatus *s = status(step);
        if (!s) return false;
        if ( !s->assignedUser() ) return false;
        return s->assignedUser()->is(u);
    }

    QVector<RamStatus*> s = status();
    for (int i = 0; i < s.count(); i++)
    {
        if (!s.at(i)) continue;
        if (!s.at(i)->assignedUser()) continue;
        if (s.at(i)->assignedUser()->is(u)) return true;
    }
    return false;
}

bool RamAbstractItem::isUnassigned(RamStep *step)
{
    if (step)
    {
        RamStatus *s = status(step);
        if (!s) return true;
        if ( !s->assignedUser() ) return true;
        return false;
    }

    QVector<RamStatus*> s = status();
    for (int i = 0; i < s.count(); i++)
    {
        if (!s.at(i)) return true;
        if (!s.at(i)->assignedUser()) return true;
    }
    return false;
}

bool RamAbstractItem::hasState(RamObject *state, RamStep *step)
{
    RamState *noState = Ramses::instance()->noState();

    if(step)
    {
        RamStatus *s = status(step);
        if (!s && noState->is(state)) return true;
        if (!s) return false;
        if( !s->state() && noState->is(state)) return true;
        if( !s->state()) return false;
        return s->state()->is(state);
    }

    QVector<RamStatus*> s = status();

    for (int i = 0; i < s.count(); i++)
    {
        if(!s.at(i) && state->is(noState)) return true;
        if(!s.at(i)) continue;
        if(!s.at(i)->state() && state->is(noState)) return true;
        if(!s.at(i)->state()) continue;
        if(s.at(i)->state()->is(state)) return true;
    }
    return false;
}

void RamAbstractItem::editHistory(RamStep *step)
{
    if (!step) return;
    if (!ui_historyWidget) ui_historyWidget = new StepStatusHistoryWidget();
    ui_historyWidget->setHistory( statusHistory(step) );

    MainWindow *mw = (MainWindow*)GuiUtils::appMainWindow();
    QString title = this->shortName() + " | " + step->shortName();
    mw->setPropertiesDockWidget( ui_historyWidget, title, this->iconName() );
}

// PRIVATE //

void RamAbstractItem::construct()
{
    m_icon = ":/icons/asset";
    m_editRole = Admin;
}

void RamAbstractItem::connectProject(RamProject *proj)
{
    connect(proj->steps(), &RamObjectModel::rowsInserted, this, &RamAbstractItem::stepInserted);
    connect(proj->steps(), &RamObjectModel::rowsAboutToBeRemoved, this, &RamAbstractItem::stepRemoved);
}

void RamAbstractItem::createStepHistory(RamStep *step, QJsonObject d)
{
    // Just for our steps
    if (m_objectType == Asset && step->type() != RamStep::AssetProduction) return;
    if (m_objectType == Shot && step->type() != RamStep::ShotProduction) return;
    QString modelName = "statusHistory-" + step->uuid();
    RamObjectModel *history = createModel(RamObject::Status, modelName);
    loadModel(history, modelName, d);
    m_history[step->uuid()] = history;
}
