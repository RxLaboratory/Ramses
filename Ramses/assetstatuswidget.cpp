#include "assetstatuswidget.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

AssetStatusWidget::AssetStatusWidget(RAMShot *s, RAMStage *st, QList<RAMStatus *> sl, QList<RAMAsset *> aa, DBInterface *d, QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    shot = s;
    stage = st;
    dbi = d;
    statusesList = sl;
    allAssets = aa;
}

void AssetStatusWidget::addAsset(RAMAsset *asset)
{
    assets.append(asset);

    //add comboBox
    AssetStatusBox *assetBox = new AssetStatusBox(asset,statusesList,this);
    connect(assetBox,SIGNAL(dialogShown(bool)),this,SLOT(setEditing(bool)));
    assetsWidget->layout()->addWidget(assetBox);
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

    //remove assets already assigned here
    foreach(RAMAsset *a,assets)
    {
        allAssets.removeAll(a);
    }

    AddAssetDialog ad(dbi,shot,stage,status,allAssets);
    //get button global coordinates
    QPoint thisCenter = this->parentWidget()->parentWidget()->mapToGlobal(this->parentWidget()->parentWidget()->geometry().center());
    QPoint newCenter(thisCenter.x()-ad.geometry().width()/2, thisCenter.y()-ad.geometry().height()/2);
    ad.move(ad.mapFromGlobal(newCenter));
    ad.setWindowFlags(Qt::FramelessWindowHint);
    if (ad.exec() == QDialog::Accepted)
    {
        //add to UI
        addAsset(ad.getAsset());
        emit newAsset(ad.getAsset());
    }

    setEditing(false);
}

void AssetStatusWidget::setEditing(bool e)
{
    emit editing(e);
}

void AssetStatusWidget::assetsListUpdated(QList<RAMAsset *> aa)
{
    allAssets = aa;
}
