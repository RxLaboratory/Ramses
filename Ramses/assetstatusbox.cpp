#include "assetstatusbox.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

AssetStatusBox::AssetStatusBox(RAMAsset *as,QList<RAMStatus *> sl, QWidget *parent) :
    QWidget(parent)
{
    freezeDBI = true;
    setupUi(this);

    asset = as;
    statusesList = sl;

    connect(asset,SIGNAL(statusChanged()),this,SLOT(assetStatusChanged()));

    int index = -1;
    freezeUI = true;
    foreach(RAMStatus *status,statusesList)
    {
        comboBox->addItem(asset->getShortName() + " | " + status->getShortName(),status->getId());
        if (status->getId() == asset->getStatus()->getId())
        {
            index = comboBox->count()-1;
        }
    }

    comboBox->setCurrentIndex(-1);

    freezeUI = false;

    comboBox->setCurrentIndex(index);

    freezeDBI = false;
}

void AssetStatusBox::on_comboBox_currentIndexChanged(int index)
{
    if (freezeUI) return;

    if (index < 0) return;
    //find status
    foreach(RAMStatus *status,statusesList)
    {
        if (status->getId() == comboBox->currentData().toInt())
        {
            //update color
            comboBox->setStyleSheet("background-color:" + status->getColor().name() + ";");

            //update stageStatus
            if (!freezeDBI)
            {
                freezeUI = true;
                asset->setStatus(status);
                freezeUI = false;
            }
            break;
        }
    }

}

void AssetStatusBox::assetStatusChanged()
{
    freezeDBI = true;
    for (int i = 0; i< comboBox->count() ; i++)
    {
        if (comboBox->itemData(i) == asset->getStatus()->getId())
        {
            comboBox->setCurrentIndex(i);
        }
    }
    freezeDBI = false;
}

void AssetStatusBox::on_detailsButton_clicked()
{
    AssetDetailsDialog ad;
    ad.setComment(asset->getComment());
    emit dialogShown(true);
    if (ad.exec())
    {
        asset->setComment(ad.getComment());
        //dbi->setStageComment(sd.getComment(),stageStatus->getStage()->getId(),shot->getId());
    }
    emit dialogShown(false);
}
