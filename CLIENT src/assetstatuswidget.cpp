#include "assetstatuswidget.h"
#include <QtDebug>

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
    assets.append(asset);
    //add comboBox
    QComboBox *assetBox = new QComboBox();
    //add statuses
    int index = -1;
    foreach(RAMStatus *status,statusesList)
    {
        QString label = asset->getShortName() + " | " + status->getShortName();
        assetBox->addItem(label,status->getId());
        if(status->getId() == asset->getStatus()->getId())
        {
            index = assetBox->count()-1;
            assetBox->setStyleSheet("background-color:" + status->getColor().name() + ";");
        }
    }
    assetBox->setCurrentIndex(index);
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

    AddAssetDialog ad(dbi,shot,stage,statusId);
    //get button global coordinates
    QPoint thisCenter = this->parentWidget()->parentWidget()->mapToGlobal(this->parentWidget()->parentWidget()->geometry().center());
    QPoint newCenter(thisCenter.x()-ad.geometry().width()/2, thisCenter.y()-ad.geometry().height()/2);
    ad.move(ad.mapFromGlobal(newCenter));
    ad.setWindowFlags(Qt::FramelessWindowHint);
    ad.exec();

    emit editing(false);
}
