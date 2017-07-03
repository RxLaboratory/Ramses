#include "assetstatuswidget.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

AssetStatusWidget::AssetStatusWidget(RAMShot *s, RAMStage *st, QList<RAMStatus *> sl, DBInterface *d, QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    shot = s;
    stage = st;
    dbi = d;
    statusesList = sl;
}

void AssetStatusWidget::addAsset(RAMAsset *asset)
{
    assignedAssets << asset;

    //add comboBox
    AssetStatusBox *assetBox = new AssetStatusBox(asset,statusesList,shot,this);
    connect(assetBox,SIGNAL(dialogShown(bool)),this,SLOT(setEditing(bool)));
    assetsWidget->layout()->addWidget(assetBox);

    connect(asset,SIGNAL(assetUnAssigned(RAMShot*,RAMAsset*)),this,SLOT(unAssign(RAMShot*,RAMAsset*)));
}

void AssetStatusWidget::on_addButton_clicked()
{
    setEditing(true);

    //get STB status
    RAMStatus *status;
    foreach(RAMStatus *s,statusesList)
    {
        if (s->getShortName() == "STB")
        {
            status = s;
            break;
        }
    }

    QList<RAMAsset*> assets = stage->getAssets();
    //remove assets already assigned here
    foreach(RAMAsset *a,assignedAssets)
    {
        assets.removeAll(a);
    }

    AddAssetDialog ad(dbi,shot,stage,status,assets);
    //get button global coordinates
    QPoint thisCenter = this->parentWidget()->parentWidget()->mapToGlobal(this->parentWidget()->parentWidget()->geometry().center());
    QPoint newCenter(thisCenter.x()-ad.geometry().width()/2, thisCenter.y()-ad.geometry().height()/2);
    ad.move(ad.mapFromGlobal(newCenter));
    ad.setWindowFlags(Qt::FramelessWindowHint);
    if (ad.exec() == QDialog::Accepted)
    {
        //add to UI
        addAsset(ad.getAsset());
        stage->addAsset(ad.getAsset());
        emit newAsset(ad.getAsset(),stage);
    }

    setEditing(false);
}

void AssetStatusWidget::setEditing(bool e)
{
    emit editing(e);
}

void AssetStatusWidget::unAssign(RAMShot *s,RAMAsset *a)
{
    if (s != shot) return;
    assignedAssets.removeAll(a);
}

