#include "projectmanagerwidget.h"

#include "ramses.h"

ProjectManagerWidget::ProjectManagerWidget(QWidget *parent) :
    ObjectManagerWidget(
        Ramses::instance()->projects(),
        "Projects",
        QIcon(":icons/project"),
        parent )
{
    m_listWidget->setEditMode(ObjectListWidget::RemoveObjects);
    m_listWidget->setSortable(true);
}

RamProject *ProjectManagerWidget::createObject()
{
    RamProject *project = new RamProject(
                "NEW",
                "New Project"
                );
    //Ramses::instance()->projects()->append(project);
    project->edit();
    return project;
}

