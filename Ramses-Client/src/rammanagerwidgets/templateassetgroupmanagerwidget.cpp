#include "templateassetgroupmanagerwidget.h"

#include "ramses.h"

TemplateAssetGroupManagerWidget::TemplateAssetGroupManagerWidget(QWidget *parent) :
    ObjectManagerWidget(
        Ramses::instance()->templateAssetGroups(),
        "Template asset groups",
        QIcon(":icons/asset-group"),
        parent )
{
    m_listWidget->setEditMode(ObjectListWidget::RemoveObjects);
    m_listWidget->setSortable(true);
}

RamTemplateAssetGroup *TemplateAssetGroupManagerWidget::createObject()
{
    RamTemplateAssetGroup *ag = new RamTemplateAssetGroup(
                "NEW",
                "New Template group");
    //Ramses::instance()->templateAssetGroups()->append(ag);
    ag->edit();
    return ag;
}
