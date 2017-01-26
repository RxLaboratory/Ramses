#include "shotstatuswidget.h"
#include <QtDebug>

ShotStatusWidget::ShotStatusWidget(DBInterface *db, RAMShot *sh, RAMStageStatus *ss, QList<RAMStatus *> sl, QWidget *parent) :
    QWidget(parent)
{
    freezeDBI = true;
    setupUi(this);

    shot = sh;
    stageStatus = ss;
    statusesList = sl;
    dbi = db;

    connect(stageStatus,SIGNAL(statusChanged(RAMStatus*,RAMStage*)),this,SLOT(stageStatusChanged(RAMStatus*,RAMStage*)));

    int index = -1;
    freezeUI = true;
    foreach(RAMStatus *status,statusesList)
    {
        comboBox->addItem(status->getShortName(),status->getId());
        if (status->getId() == stageStatus->getStatus()->getId())
        {
            index = comboBox->count()-1;
        }
    }
    comboBox->setCurrentIndex(-1);
    freezeUI = false;

    comboBox->setCurrentIndex(index);

    freezeDBI = false;
}

void ShotStatusWidget::on_comboBox_currentIndexChanged(int index)
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
                stageStatus->setStatus(status);
                freezeUI = false;
            }
            break;
        }
    }

}

void ShotStatusWidget::stageStatusChanged(RAMStatus *newStatus, RAMStage *st)
{
    freezeDBI = true;
    for (int i = 0; i< comboBox->count() ; i++)
    {
        if (comboBox->itemData(i) == newStatus->getId())
        {
            comboBox->setCurrentIndex(i);
        }
    }
    freezeDBI = false;
}

void ShotStatusWidget::on_detailsButton_clicked()
{
    ShotDetailsDialog sd;
    sd.setComment(stageStatus->getComment());
    emit dialogShown(true);
    if (sd.exec())
    {
        stageStatus->setComment(sd.getComment());
        dbi->setStageComment(sd.getComment(),stageStatus->getStage()->getId(),shot->getId());
    }
    emit dialogShown(false);
}
