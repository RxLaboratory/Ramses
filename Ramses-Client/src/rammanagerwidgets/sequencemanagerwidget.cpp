#include "sequencemanagerwidget.h"

#include "ramsequence.h"
#include "ramses.h"

SequenceManagerWidget::SequenceManagerWidget(QWidget *parent):
    ObjectManagerWidget(
        "Sequences",
        QIcon(":icons/sequence"),
        parent)
{
    changeProject(Ramses::instance()->currentProject());
    connect(Ramses::instance(), SIGNAL(currentProjectChanged(RamProject*)), this, SLOT(changeProject(RamProject*)));
    m_listWidget->setEditMode(ObjectListWidget::RemoveObjects);
    m_listWidget->setSortable(true);
}

RamSequence *SequenceManagerWidget::createObject()
{
    RamProject *project = Ramses::instance()->currentProject();
    if (!project) return nullptr;
    RamSequence *s = new RamSequence(
                "NEW",
                "New Sequence",
                project);
    project->sequences()->appendObject(s->uuid());
    s->edit();
    return s;
}

void SequenceManagerWidget::changeProject(RamProject *project)
{
    // empty list
    this->clear();
    if (!project) return;
    this->setObjectModel( project->sequences() );
}
