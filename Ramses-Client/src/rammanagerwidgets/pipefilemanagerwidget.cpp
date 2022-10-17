#include "pipefilemanagerwidget.h"

#include "ramses.h"

PipeFileManagerWidget::PipeFileManagerWidget(QWidget *parent):
    ObjectManagerWidget(
        "Pipe Types",
        QIcon(":icons/file"),
        parent)
{
    changeProject(Ramses::instance()->currentProject());
    connect(Ramses::instance(), SIGNAL(currentProjectChanged(RamProject*)), this, SLOT(changeProject(RamProject*)));
    m_listWidget->setEditMode(ObjectListWidget::RemoveObjects);
    m_listWidget->setSortable(true);
}

RamPipeFile *PipeFileManagerWidget::createObject()
{
    RamProject *project = Ramses::instance()->currentProject();
    if (!project) return nullptr;
    RamPipeFile *pf = new RamPipeFile(
                "NEW",
                project);
    project->pipeFiles()->appendObject(pf->uuid());
    pf->edit();
    return pf;
}

void PipeFileManagerWidget::changeProject(RamProject *project)
{
    // empty list
    this->clear();
    if (!project) return;
    this->setObjectModel( project->pipeFiles() );
}
