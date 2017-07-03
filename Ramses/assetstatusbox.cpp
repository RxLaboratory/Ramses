#include "assetstatusbox.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

AssetStatusBox::AssetStatusBox(RAMAsset *as,QList<RAMStatus *> sl, RAMShot *s,QWidget *parent) :
    QWidget(parent)
{
    freezeDBI = true;
    setupUi(this);

    asset = as;
    statusesList = sl;
    shot = s;

    detailsButton->setText(as->getShortName());

    connect(asset,SIGNAL(statusChanged(RAMAsset*,RAMStatus*)),this,SLOT(assetStatusChanged(RAMAsset*,RAMStatus*)));
    connect(asset,SIGNAL(nameChanged(QString)),this,SLOT(nameChanged(QString)));
    connect(asset,SIGNAL(shortNameChanged(QString)),this,SLOT(shortNameChanged(QString)));

    int index = -1;
    freezeUI = true;

    //add statuses
    foreach(RAMStatus *status,statusesList)
    {
        comboBox->addItem(status->getShortName(),status->getId());
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
            QString bgColor = "background-color:" + status->getColor().name() + ";";
            this->setStyleSheet(bgColor);

            //update stageStatus
            if (!freezeDBI)
            {
                freezeUI = true;
                asset->setStatus(status,true);
                freezeUI = false;
            }
            break;
        }
    }

}

void AssetStatusBox::on_removeButton_clicked()
{
    asset->unAssign(shot,true);
    delete this;
}

void AssetStatusBox::assetStatusChanged(RAMAsset *a, RAMStatus *s)
{
    freezeDBI = true;
    for (int i = 0; i< comboBox->count() ; i++)
    {
    if (comboBox->itemData(i) == s->getId())
        {
            comboBox->setCurrentIndex(i);
        }
    }
    freezeDBI = false;
}

void AssetStatusBox::on_detailsButton_clicked()
{
    AssetDetailsDialog ad(asset);

    //get button global coordinates
    QPoint thisCenter = this->parentWidget()->parentWidget()->parentWidget()->mapToGlobal(this->parentWidget()->parentWidget()->parentWidget()->geometry().center());
    QPoint newCenter(thisCenter.x()-ad.geometry().width()/2, thisCenter.y()-ad.geometry().height()/2);
    ad.move(ad.mapFromGlobal(newCenter));


    emit dialogShown(true);
    ad.exec();
    emit dialogShown(false);
}

void AssetStatusBox::nameChanged(QString n)
{
    this->setToolTip(n);
}

void AssetStatusBox::shortNameChanged(QString n)
{
    detailsButton->setText(n);
}


