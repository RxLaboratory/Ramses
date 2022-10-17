#include "templatestepmanagerwidget.h"

#include "ramses.h"

TemplateStepManagerWidget::TemplateStepManagerWidget(QWidget *parent) :
    ObjectManagerWidget(
        Ramses::instance()->templateSteps(),
        "Template steps",
        QIcon(":icons/step"),
        parent )
{
    m_listWidget->setEditMode(ObjectListWidget::RemoveObjects);
    m_listWidget->setSortable(true);
}

RamTemplateStep *TemplateStepManagerWidget::createObject()
{
    RamTemplateStep *ts = new RamTemplateStep(
                "NEW",
                "New Template Step"
                );

    ts->edit();
    return ts;
}
