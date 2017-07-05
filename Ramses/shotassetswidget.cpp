#include "shotassetswidget.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

ShotAssetsWidget::ShotAssetsWidget(RAMShot *s, RAMStage *st, DBInterface *d, Updater *up, QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    shot = s;
    stage = st;
    dbi = d;
    updater = up;

    //check existing assets, add them
    foreach(RAMAsset *asset,stage->getAssets())
    {
        addAsset(asset);
    }

    connect(stage,SIGNAL(assetAdded(RAMAsset*)),this,SLOT(addAsset(RAMAsset*)));
}

void ShotAssetsWidget::addAsset(RAMAsset *asset)
{
    //check assignment
    QList<RAMShot*> assignments = asset->getAssignments();
    connect(asset,SIGNAL(assetAssigned(RAMShot*,RAMAsset*)),this,SLOT(assign(RAMShot*,RAMAsset*)));

    if (assignments.indexOf(shot) < 0) return;

    assignedAssets << asset;

    //add comboBox
    AssetStatusBox *assetBox = new AssetStatusBox(asset,shot,updater,this);
    connect(assetBox,SIGNAL(dialogShown(bool)),this,SLOT(setEditing(bool)));
    assetsWidget->layout()->addWidget(assetBox);

    connect(asset,SIGNAL(assetUnAssigned(RAMShot*,RAMAsset*)),this,SLOT(unAssign(RAMShot*,RAMAsset*)));
    connect(asset,SIGNAL(assetRemoved(RAMAsset*)),this,SLOT(unAssign(RAMAsset*)));
}

void ShotAssetsWidget::on_addButton_clicked()
{
    setEditing(true);

    QList<RAMAsset*> assets = stage->getAssets();
    //remove assets already assigned here
    foreach(RAMAsset *a,assignedAssets)
    {
        assets.removeAll(a);
    }

    AddAssetDialog ad(dbi,shot,stage,updater,assets);
    //get button global coordinates
    QPoint thisCenter = this->parentWidget()->parentWidget()->mapToGlobal(this->parentWidget()->parentWidget()->geometry().center());
    QPoint newCenter(thisCenter.x()-ad.geometry().width()/2, thisCenter.y()-ad.geometry().height()/2);
    ad.move(ad.mapFromGlobal(newCenter));
    ad.setWindowFlags(Qt::FramelessWindowHint);
    ad.exec();

    setEditing(false);
}

void ShotAssetsWidget::setEditing(bool e)
{
    emit editing(e);
}

void ShotAssetsWidget::assign(RAMShot *s, RAMAsset *a)
{
    if (s == shot) addAsset(a);
}

void ShotAssetsWidget::unAssign(RAMShot *s,RAMAsset *a)
{
    if (s != shot) return;
    assignedAssets.removeAll(a);
}

void ShotAssetsWidget::unAssign(RAMAsset *a)
{
    assignedAssets.removeAll(a);
}

