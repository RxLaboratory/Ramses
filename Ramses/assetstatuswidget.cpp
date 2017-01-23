#include "assetstatuswidget.h"
#include <QtDebug>

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
    assetsWidget->layout()->addWidget(assetBox);
}

void AssetStatusWidget::on_addButton_clicked()
{
    emit editing(true);

    //get STB status
    int statusId = 0;
    foreach(RAMStatus *s,statusesList)
    {
        if (s->getShortName() == "STB")
        {
            statusId = s->getId();
            break;
        }
    }

    qDebug() << allAssets;
    foreach(RAMAsset *a,assets)
    {
        allAssets.removeAll(a);
        qDebug() << a->getName();
    }

    AddAssetDialog ad(dbi,shot,stage,statusId,allAssets);
    //get button global coordinates
    QPoint thisCenter = this->parentWidget()->parentWidget()->mapToGlobal(this->parentWidget()->parentWidget()->geometry().center());
    QPoint newCenter(thisCenter.x()-ad.geometry().width()/2, thisCenter.y()-ad.geometry().height()/2);
    ad.move(ad.mapFromGlobal(newCenter));
    ad.setWindowFlags(Qt::FramelessWindowHint);
    ad.exec();

    emit editing(false);
}
