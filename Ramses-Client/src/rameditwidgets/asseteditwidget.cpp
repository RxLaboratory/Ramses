#include "asseteditwidget.h"

#include "ramproject.h"

AssetEditWidget::AssetEditWidget(QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();
}

AssetEditWidget::AssetEditWidget(RamAsset *asset, QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    setObject(asset);
    connectEvents();

}

RamAsset *AssetEditWidget::asset() const
{
    return m_asset;
}

void AssetEditWidget::reInit(RamObject *o)
{
    m_asset = qobject_cast<RamAsset*>(o);
    if (m_asset)
    {
        ui_tagsEdit->setText(m_asset->tags().join(", "));
        ui_folderWidget->setPath(m_asset->path());
        //set asset group
        RamProject *project = m_asset->project();
        QSignalBlocker b(ui_assetGroupBox);
        ui_assetGroupBox->setObjectModel( project->assetGroups() );
        ui_assetGroupBox->setObject( m_asset->assetGroup() );
    }
    else
    {
        ui_tagsEdit->setText("");
        ui_folderWidget->setPath("");
        ui_assetGroupBox->setCurrentIndex(-1);
    }
}

void AssetEditWidget::setTags()
{
    if (!m_asset) return;
    m_asset->setTags(ui_tagsEdit->text());
}

void AssetEditWidget::setAssetGroup(RamObject *ag)
{
    if(!m_asset) return;
    m_asset->setAssetGroup(RamAssetGroup::c(ag));
}

void AssetEditWidget::setupUi()
{
    // Tags
    QLabel *tagsLabel = new QLabel("Tags", this);
    ui_mainFormLayout->addWidget(tagsLabel, 3, 0);

    ui_tagsEdit = new QLineEdit(this);
    ui_tagsEdit->setPlaceholderText("tag1, tag2, ...");
    ui_mainFormLayout->addWidget(ui_tagsEdit, 3, 1);

    QLabel *assetGroupLabel = new QLabel("Asset group", this);
    ui_mainFormLayout->addWidget(assetGroupLabel, 4, 0);

    ui_assetGroupBox = new RamObjectComboBox(this);
    ui_mainFormLayout->addWidget(ui_assetGroupBox, 4, 1);

    ui_folderWidget = new DuQFFolderDisplayWidget(this);
    ui_mainLayout->insertWidget(1, ui_folderWidget);

    ui_mainLayout->addStretch();
}

void AssetEditWidget::connectEvents()
{
    connect(ui_tagsEdit, SIGNAL(editingFinished()), this, SLOT(setTags()));
    connect(ui_assetGroupBox, &RamObjectComboBox::currentObjectChanged, this, &AssetEditWidget::setAssetGroup);
}
