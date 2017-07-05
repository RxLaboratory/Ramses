#include "assetstatusbox.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

AssetStatusBox::AssetStatusBox(RAMAsset *as, RAMShot *s, Updater *up, QWidget *parent) :
    QWidget(parent)
{
    freezeDBI = true;
    freezeUI = false;
    setupUi(this);

    updater = up;

    asset = as;
    statusesList = updater->getStatuses();
    shot = s;

    detailsButton->setText(as->getShortName());

    //remove wheel event from combobox
    comboBox->installEventFilter(this);

    connect(asset,SIGNAL(statusChanged(RAMAsset*,RAMStatus*)),this,SLOT(assetStatusChanged(RAMAsset*,RAMStatus*)));
    connect(asset,SIGNAL(nameChanged(QString)),this,SLOT(nameChanged(QString)));
    connect(asset,SIGNAL(shortNameChanged(QString)),this,SLOT(shortNameChanged(QString)));
    connect(asset,SIGNAL(assetRemoved(RAMAsset*)),this,SLOT(deleteLater()));
    connect(asset,SIGNAL(assetUnAssigned(RAMShot*,RAMAsset*)),this,SLOT(unAssign(RAMShot*,RAMAsset*)));
    connect(updater,SIGNAL(statusAdded(RAMStatus*)),this,SLOT(addStatus(RAMStatus*)));
    connect(updater,SIGNAL(statusRemoved(RAMStatus*)),this,SLOT(removeStatus(RAMStatus*)));

    //add statuses
    foreach(RAMStatus *status,statusesList)
    {
        addStatus(status);
    }


    freezeDBI = false;
}

void AssetStatusBox::addStatus(RAMStatus *status)
{
    comboBox->addItem(status->getShortName(),status->getId());
    if (status->getId() == asset->getStatus()->getId())
    {
        comboBox->setCurrentIndex(comboBox->count()-1);
    }
}

void AssetStatusBox::removeStatus(RAMStatus *status)
{
    for (int i = 0 ; i < comboBox->count() ; i++)
    {
        if (comboBox->itemData(i).toInt() == status->getId())
        {
            comboBox->removeItem(i);
            break;
        }
    }
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

void AssetStatusBox::unAssign(RAMShot *s, RAMAsset *a)
{
    if (s == shot) deleteLater();
}

bool AssetStatusBox::eventFilter(QObject *obj, QEvent *event)
{
    if ( event->type() == QEvent::Wheel )
    {
        event->ignore();
        return true;
    }
    return QWidget::eventFilter( obj, event );
}

