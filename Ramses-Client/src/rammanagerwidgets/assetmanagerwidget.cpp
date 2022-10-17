#include "assetmanagerwidget.h"

#include "ramasset.h"
#include "ramassetgroup.h"
#include "ramses.h"

AssetManagerWidget::AssetManagerWidget(QWidget *parent):
    ObjectManagerWidget(
        "Assets",
        QIcon(":icons/asset"),
        parent)
{
    changeProject(Ramses::instance()->currentProject());
    connect(Ramses::instance(), SIGNAL(currentProjectChanged(RamProject*)), this, SLOT(changeProject(RamProject*)));
    m_listWidget->setEditMode(ObjectListWidget::RemoveObjects);
    m_listWidget->setSortable(true);
}

RamAbstractItem *AssetManagerWidget::createObject()
{
    RamProject *project = Ramses::instance()->currentProject();
    if (!project) return nullptr;
    if (project->assetGroups()->rowCount() == 0 ) return nullptr;

    RamAssetGroup *ag = RamAssetGroup::c( currentFilter() );
    if (!ag) ag = RamAssetGroup::c( project->assetGroups()->get(0) );
    if (!ag) return nullptr;

    RamAsset *asset = new RamAsset(
                "NEW",
                "New Asset",
                ag
                );

    project->assets()->appendObject(asset->uuid());
    asset->edit();
    return asset;
}

void AssetManagerWidget::changeProject(RamProject *project)
{
    if (!project) return;
    this->setObjectModel( project->assets() );
    m_listWidget->setFilterList( project->assetGroups(), "Assets" );
}
