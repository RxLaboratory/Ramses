#include "usermanagerwidget.h"

#include "ramses.h"

UserManagerWidget::UserManagerWidget(QWidget *parent) :
    ObjectManagerWidget(
        Ramses::instance()->users(),
        "Users",
        QIcon(":icons/user"),
        parent )
{
    m_listWidget->setEditMode(ObjectListWidget::RemoveObjects);
    QStringList dontRemove;
    dontRemove << "Ramses" << "Removed";
    m_listWidget->setDontRemoveShortNameList(dontRemove);
    m_listWidget->setSortable(false);
    m_listWidget->setSortMode(RamObject::ShortName);
}

RamUser *UserManagerWidget::createObject()
{
    RamUser *user = new RamUser("NEW","J-Doe");
    user->edit();
    return user;
}

