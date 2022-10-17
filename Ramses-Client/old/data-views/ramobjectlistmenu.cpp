#include "ramobjectlistmenu.h"

RamObjectListMenu::RamObjectListMenu(bool checkable, QWidget *parent):
    QMenu(parent)
{
    m_emptyList = RamObjectList::emptyList();
    m_objects = new RamObjectFilterModel(this);
    m_objects->setSourceModel(m_emptyList);

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

void RamObjectListMenu::setList(RamObjectList *list)
{
    if (!list) m_objects->setList(m_emptyList);
    else {
        list->sort();
        m_objects->setList(list);
    }
}

void RamObjectListMenu::addCreateButton()
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

RamObjectFilterModel *RamObjectListMenu::filteredList()
{
    return m_objects;
}

void RamObjectListMenu::setObjectVisible(RamObject *obj, bool visible)
{
    if (!obj) return;
    QList<QAction *> actions = this->actions();
    for(int j= actions.count() -1; j >= 0; j--)
    {
        RamObject *o = object(actions.at(j));
        if (obj->is(o)) actions.at(j)->setVisible(visible);
    }
}

void RamObjectListMenu::showAll()
{
    QList<QAction *> actions = this->actions();
    for(int j= actions.count() -1; j >= 0; j--)
    {
        actions.at(j)->setVisible(true);
    }
}

RamObject *RamObjectListMenu::objectAt(int i)
{
    QList<QAction*> actions = this->actions();
    return object( actions[i] );
}

bool RamObjectListMenu::isAllChecked() const
{
    QList<QAction*> as = this->actions();

    for (int i = 0; i < as.count(); i++)
    {
        if (as.at(i)->isCheckable() && !as.at(i)->isChecked()) return false;
    }

    return true;
}

void RamObjectListMenu::newObject(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    for(int i = first; i <= last; i++)
    {
        RamObject *o = m_objects->at(i);
        if (!o) continue;
        QAction *a = new QAction( o->name() );
        a->setData( reinterpret_cast<quintptr>(o) );
        a->setCheckable(m_checkable);
        if (m_checkable) a->setChecked(true);
        this->addAction(a);
        connect(a,SIGNAL(toggled(bool)),this,SLOT(actionAssign(bool)));
        connect(a,SIGNAL(triggered()),this,SLOT(actionAssign()));
    }
}

void RamObjectListMenu::removeObject(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    for(int i = first; i <= last; i++)
    {
        QList<QAction*> actions = this->actions();
        for (int j = actions.count() -1; j >= 0; j--)
        {
            RamObject *o = object(actions.at(j));
            RamObject *t = m_objects->at(i);
            if (t && t->is(o))
            {
                actions.at(j)->deleteLater();
                break;
            }
        }
    }
}

void RamObjectListMenu::objectChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(roles)
    QList<QAction*> actions = this->actions();
    for (int i = topLeft.row(); i <= bottomRight.row(); i++)
    {
        for (int j = actions.count() -1; j >= 0; j--)
        {
            RamObject *o = object(actions.at(j));
            RamObject *t = m_objects->at(i);
            if (t && t->is(o))
            {
                actions.at(j)->setText(m_objects->at(i)->name());
                break;
            }
        }
    }
}

void RamObjectListMenu::actionAssign(bool checked)
{
    QAction *a = qobject_cast<QAction*>( sender() );
    emit assignmentChanged( object(a), checked );
}

void RamObjectListMenu::actionAssign()
{
    QAction *a = qobject_cast<QAction*>( sender() );
    emit assigned( object(a) );
}

void RamObjectListMenu::actionCreate()
{
    emit createTriggered();
}

void RamObjectListMenu::clear()
{
    // Remove all
    QList<QAction*> actions = this->actions();
    for (int j = actions.count() -1; j >= 0; j--)
    {
        if (actions.at(j)->data().toInt() != 0)
            actions.at(j)->deleteLater();
    }
}

void RamObjectListMenu::reset()
{
    //Add all
    newObject(QModelIndex(),0,m_objects->rowCount()-1);
}

QString RamObjectListMenu::objectUuid(QAction *a) const
{
    RamObject *obj = object(a);

    if (!obj) return a->text();

    return obj->uuid();
}

RamObject *RamObjectListMenu::object(QAction *a) const
{
    quintptr iptr = a->data().toULongLong();
    if (iptr == 0) return nullptr;
    RamObject *obj = reinterpret_cast<RamObject*>(iptr);
    return obj;
}

void RamObjectListMenu::selectAll()
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

void RamObjectListMenu::selectNone()
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

void RamObjectListMenu::filter(RamObject *o)
{
    //clear();
    if (o) m_objects->setFilterUuid( o->uuid() );
    else m_objects->setFilterUuid( "" );
    //reset();
}

void RamObjectListMenu::select(RamObject *o)
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

void RamObjectListMenu::saveState(QSettings *settings, QString group) const
{
    settings->beginGroup(group);
    QList<QAction*> as = this->actions();
    for (int i = 0; i < as.count(); i++)
    {
        QAction *a = as.at(i);
        if (!a->isCheckable()) continue;

        QString uuid = objectUuid(a);

        settings->setValue(uuid, a->isChecked() );
    }
    settings->endGroup();
}

void RamObjectListMenu::restoreState(QSettings *settings, QString group)
{
    settings->beginGroup(group);

    QList<QAction*> as = this->actions();

    for (int i = 0; i < as.count(); i++)
    {
        QAction *a = as.at(i);

        RamObject *obj = object(a);
        QString uuid = objectUuid(a);

        bool ok = settings->value(uuid, true).toBool();
        a->setChecked( ok );
        if (obj) emit assignmentChanged( obj, ok );
    }

    settings->endGroup();
}



