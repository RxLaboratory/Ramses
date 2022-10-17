#include "applicationmanagerwidget.h"

#include "ramses.h"

ApplicationManagerWidget::ApplicationManagerWidget(QWidget *parent) :
    ObjectManagerWidget(
        Ramses::instance()->applications(),
        "Applications",
        QIcon(":icons/application"),
        parent )
{
    m_listWidget->setEditMode(ObjectListWidget::RemoveObjects);
    m_listWidget->setSortMode(RamObject::Name);
    m_listWidget->sort();
}

RamApplication *ApplicationManagerWidget::createObject()
{
    RamApplication *a = new RamApplication(
                "NEW",
                "New Application");
    a->edit();
    return a;
}
