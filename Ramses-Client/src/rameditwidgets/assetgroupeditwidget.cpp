#include "assetgroupeditwidget.h"

#include "ramasset.h"
#include "ramproject.h"

AssetGroupEditWidget::AssetGroupEditWidget(QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();
}

AssetGroupEditWidget::AssetGroupEditWidget(RamAssetGroup *ag, QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();
    setObject(ag);
}

RamAssetGroup *AssetGroupEditWidget::assetGroup() const
{
    return m_assetGroup;
}

void AssetGroupEditWidget::reInit(RamObject *o)
{
    m_assetGroup = qobject_cast<RamAssetGroup*>(o);
    if (m_assetGroup)
    {
        ui_assetsList->setObjectModel(m_assetGroup->project()->assets());
        ui_assetsList->setFilter(m_assetGroup);
        ui_colorSelector->setColor(m_assetGroup->color());

        ui_folderWidget->setPath(m_assetGroup->path());
    }
    else
    {
        ui_folderWidget->setPath("");
        ui_assetsList->setObjectModel(nullptr);
        ui_colorSelector->setColor(QColor(67,67,67));
    }
}

void AssetGroupEditWidget::setColor(QColor c)
{
    if (!m_assetGroup) return;
    m_assetGroup->setColor(c);
}

void AssetGroupEditWidget::createAsset()
{
    if (!m_assetGroup) return;
    RamAsset *asset = new RamAsset(
                "NEW",
                "New Asset",
                m_assetGroup);
    m_assetGroup->project()->assets()->appendObject(asset->uuid());
    asset->edit();
}

void AssetGroupEditWidget::setupUi()
{
    ui_folderWidget = new DuQFFolderDisplayWidget(this);
    ui_mainLayout->insertWidget(1, ui_folderWidget);

    QLabel *colorLabel = new QLabel("Color", this);
    ui_mainFormLayout->addWidget(colorLabel, 3, 0);
    ui_colorSelector = new DuQFColorSelector(this);
    ui_mainFormLayout->addWidget(ui_colorSelector, 3, 1);

    ui_assetsList = new ObjectListWidget(true, RamUser::ProjectAdmin, this);
    ui_assetsList->setEditMode(ObjectListWidget::RemoveObjects);
    ui_assetsList->setTitle("Assets");
    ui_mainLayout->addWidget(ui_assetsList);
}

void AssetGroupEditWidget::connectEvents()
{
    connect(ui_assetsList, SIGNAL(add()), this, SLOT(createAsset()));
    connect(ui_colorSelector, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
}
