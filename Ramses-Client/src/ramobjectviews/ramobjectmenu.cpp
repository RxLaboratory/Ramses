#include "ramobjectmenu.h"

RamObjectMenu::RamObjectMenu(bool checkable, QWidget *parent):
    QMenu(parent)
{
    m_objects = new RamObjectSortFilterProxyModel("", this);
    setSortMode(RamObject::Order);
    m_objects->setSourceModel( RamObjectModel::emptyModel() );

    m_checkable = checkable;

    if (m_checkable)
    {
        QAction *all = new QAction("Select all");
        this->addAction(all);
        QAction *none = new QAction("Select none");
        this->addAction(none);
        this->addSeparator();

        connect(all,SIGNAL(triggered()),this,SLOT(selectAll()));
        connect(none,SIGNAL(triggered()),this,SLOT(selectNone()));
    }

    connect(m_objects, SIGNAL(rowsInserted(QModelIndex,int,int)),this,SLOT(newObject(QModelIndex,int,int)));
    connect(m_objects, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)),this,SLOT(removeObject(QModelIndex,int,int)));
    connect(m_objects, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this,SLOT(objectChanged(QModelIndex,QModelIndex,QVector<int>)));
    connect(m_objects, SIGNAL(modelAboutToBeReset()),this,SLOT(clear()));
    connect(m_objects, SIGNAL(modelReset()),this,SLOT(reset()));
}

void RamObjectMenu::setObjectModel(RamObjectModel *list)
{
    if (!list) m_objects->setSourceModel( RamObjectModel::emptyModel() );
    else {
        m_objects->setSourceModel(list);
        m_objects->sort(0);
    }
    clear();
    reset();
}

RamObjectSortFilterProxyModel *RamObjectMenu::model()
{
    return m_objects;
}

void RamObjectMenu::setSortMode(RamAbstractObject::DataRole mode)
{
    m_objects->setSortRole(mode);
}

void RamObjectMenu::addCreateButton()
{
    QAction *createAction = new QAction("Create new...");
    if (this->actions().count() > 0)
    {
        this->insertAction(this->actions().at(0), createAction);
        this->insertSeparator(this->actions().at(1));
    }
    else
    {
         this->addAction(createAction);
         this->addSeparator();
    }

    connect(createAction,SIGNAL(triggered()),this,SLOT(actionCreate()));
}

void RamObjectMenu::setObjectVisible(RamObject *obj, bool visible)
{
    if (!obj) return;
    QList<QAction *> actions = this->actions();
    for(int j= actions.count() -1; j >= 0; j--)
    {
        RamObject *o = object(actions.at(j));
        if (obj->is(o)) actions.at(j)->setVisible(visible);
    }
}

void RamObjectMenu::showAll()
{
    QList<QAction *> actions = this->actions();
    for(int j= actions.count() -1; j >= 0; j--)
    {
        actions.at(j)->setVisible(true);
    }
}

RamObject *RamObjectMenu::objectAt(int i)
{
    QList<QAction*> actions = this->actions();
    return object( actions[i] );
}

bool RamObjectMenu::isAllChecked() const
{
    QList<QAction*> as = this->actions();

    for (int i = 0; i < as.count(); i++)
    {
        if (as.at(i)->isCheckable() && !as.at(i)->isChecked()) return false;
    }

    return true;
}

void RamObjectMenu::selectAll()
{
    QList<QAction*> actions = this->actions();
    for (int j = actions.count() -1; j >= 0; j--)
    {
        QAction *a = actions.at(j);
        if (a->isCheckable()) a->setChecked(true);

        RamObject *o = object(a);
        if (o) emit assignmentChanged( o, true );
    }
}

void RamObjectMenu::selectNone()
{
    QList<QAction*> actions = this->actions();
    for (int j = actions.count() -1; j >= 0; j--)
    {
        QAction *a = actions.at(j);
        if (a->isCheckable()) a->setChecked(false);

        RamObject *o = object(a);
        if (o) emit assignmentChanged( o, false );
    }
}

void RamObjectMenu::setFilterObject(RamObject *o)
{
    //clear();
    if (o) m_objects->setFilterUuid( o->uuid() );
    else m_objects->setFilterUuid( "" );
    //reset();
}

void RamObjectMenu::select(RamObject *o)
{
    if (!o)
    {
        selectNone();
        return;
    }

    QList<QAction*> actions = this->actions();
    for (int j = actions.count() -1; j >= 0; j--)
    {
        RamObject *obj = object( actions.at(j) );
        if (o->is(obj))
            actions.at(j)->setChecked(true);
    }
}

void RamObjectMenu::saveState(QSettings *settings, QString group) const
{
    settings->beginGroup(group);
    QList<QAction*> as = this->actions();
    for (int i = 0; i < as.count(); i++)
    {
        QAction *a = as.at(i);
        if (!a->isCheckable()) continue;

        QString uuid = a->data().toString();

        settings->setValue(uuid, a->isChecked() );
    }
    settings->endGroup();
}

void RamObjectMenu::restoreState(QSettings *settings, QString group)
{
    settings->beginGroup(group);

    QList<QAction*> as = this->actions();

    for (int i = 0; i < as.count(); i++)
    {
        QAction *a = as.at(i);

        RamObject *obj = object(a);
        QString uuid = a->data().toString();

        if (uuid != "")
        {
            bool ok = settings->value(uuid, true).toBool();
            a->setChecked( ok );
            if (obj) emit assignmentChanged( obj, ok );
        }
    }

    settings->endGroup();
}

void RamObjectMenu::newObject(const QModelIndex &parent, int first, int last)
{
    for(int i = first; i <= last; i++)
    {
        QModelIndex index = m_objects->index(i, 0, parent);
        QString uuid = index.data(RamObject::UUID).toString();
        if (uuid == "") continue;
        QAction *a = new QAction( index.data(Qt::DisplayRole).toString() );
        a->setData( uuid );
        a->setCheckable(m_checkable);
        if (m_checkable) a->setChecked(true);
        this->addAction(a);
        connect(a,SIGNAL(toggled(bool)),this,SLOT(actionAssign(bool)));
        connect(a,SIGNAL(triggered()),this,SLOT(actionAssign()));
    }
}

void RamObjectMenu::removeObject(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    for(int i = first; i <= last; i++)
    {
        QList<QAction*> actions = this->actions();
        for (int j = actions.count() -1; j >= 0; j--)
        {
            QString aUuid = actions.at(j)->data().toString();
            QString oUuid = m_objects->data(m_objects->index(i, 0), RamObject::UUID).toString();
            if (aUuid == oUuid)
            {
                actions.at(j)->deleteLater();
                break;
            }
        }
    }
}

void RamObjectMenu::objectChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(roles)
    QList<QAction*> actions = this->actions();
    for (int i = topLeft.row(); i <= bottomRight.row(); i++)
    {
        for (int j = actions.count() -1; j >= 0; j--)
        {
            QString aUuid = actions.at(j)->data().toString();
            QString oUuid = m_objects->data(m_objects->index(i, 0), Qt::UserRole).toString();
            if (aUuid == oUuid)
            {
                actions.at(j)->setText(m_objects->get(i)->name());
                break;
            }
        }
    }
}

void RamObjectMenu::actionAssign(bool checked)
{
    QAction *a = qobject_cast<QAction*>( sender() );
    emit assignmentChanged( object(a), checked );
}

void RamObjectMenu::actionAssign()
{
    QAction *a = qobject_cast<QAction*>( sender() );
    emit assigned( object(a) );
}

void RamObjectMenu::actionCreate()
{
    emit createTriggered();
}

void RamObjectMenu::clear()
{
    // Remove all
    QList<QAction*> actions = this->actions();
    for (int j = actions.count() -1; j >= 0; j--)
    {
        QAction* a = actions.at(j);
        if (actions.at(j)->data().toString() != "") {
            this->removeAction(a);
            a->deleteLater();
        }
    }
}

void RamObjectMenu::reset()
{
    //Add all
    newObject(QModelIndex(),0,m_objects->rowCount()-1);
}

RamObject *RamObjectMenu::object(QAction *a) const
{
    QString uuid = a->data().toString();
    if (uuid == "") return nullptr;
    RamObject::ObjectType type = m_objects->type();
    if (type == RamObject::Object) return nullptr;
    return RamObject::get(uuid, m_objects->type() );
}
