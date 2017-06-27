#include "updater.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

Updater::Updater(DBInterface *db, QList<RAMShot *> *sl, QObject *parent) : QObject(parent)
{
    dbi = db;

    shotsList = sl;

    connect(dbi,SIGNAL(gotShots(bool,QString,QJsonValue)),this,SLOT(gotShots(bool,QString,QJsonValue)));
}

void Updater::getShots(RAMProject *project)
{
    dbi->getShots(project->getId());
}

void Updater::gotShots(bool success,QString message,QJsonValue shots)
{
    /*if (!success) return;

#ifdef QT_DEBUG
    qDebug() << "Updater: Got Shots";
    qDebug() << shots;
#endif

    //convert data to Json Array
    QJsonArray shotsJsonArray = shots.toArray();

    //LOAD DATA
    foreach (QJsonValue shotJson, shotsJsonArray)
    {
        //get values
        QJsonObject shot = shotJson.toObject();
        QString shotName = shot.value("shotName").toString();
        double shotDuration = shot.value("duration").toDouble();
        int shotId = shot.value("shotId").toInt();
        int shotOrder = shot.value("shotOrder").toInt();

        //if existing, update
        bool exists = false;
        for (int i = 0;i<shotsList->count();i++)
        {
            RAMShot *ramShot = shotsList->at(i);
            if (ramShot->getId() == shotId)
            {
                ramShot->setName(shotName);
                ramShot->setDuration(shotDuration);
                ramShot->setShotOrder(shotOrder);
                exists = true;
                break;
            }
        }
        if (exists) continue;

        //add new shot
        RAMShot *ramShot = new RAMShot(dbi,shotId,shotName,shotDuration,shotOrder);
        shotsList->append(ramShot);
        emit newShot(ramShot);
    }

    //TODO Sort shots
    std::sort(shotsList->begin(),shotsList->end());

#ifdef QT_DEBUG
    qDebug() << "Updater: Shots List Updated";
#endif*/
}


//OLD Got Shots from MainWindow
/*
        if (!found)
        {
            ramShot = new RAMShot(dbi,shotId,shotName,shotDuration,shotOrder);
            shotsList << ramShot;
            //connections
            connect(ramShot,SIGNAL(stageStatusUpdated(RAMStatus*,RAMStage*,RAMShot*)),this,SLOT(updateStageStatus(RAMStatus*,RAMStage*,RAMShot*)));
            connect(ramShot,SIGNAL(assetAdded(RAMAsset*,RAMShot*)),this,SLOT(assetAssigned(RAMAsset*,RAMShot*)));
            connect(ramShot,SIGNAL(statusAdded(RAMStageStatus*,RAMShot*)),this,SLOT(shotStatusAdded(RAMStageStatus*,RAMShot*)));

            //Add to maintable

            //create Table row
            QTableWidgetItem *rowHeader = new QTableWidgetItem(shotName);
            rowHeader->setToolTip(QString::number(shotDuration) + "s");
            mainTable->setRowCount(mainTable->rowCount() + 1);
            mainTable->setVerticalHeaderItem(mainTable->rowCount()-1,rowHeader);

            //for each asset stage, add widgets
            for(int i = 0 ; i < stagesList.count() ; i++)
            {
                RAMStage *stage = stagesList[i];
                if (stage->getType() == "a")
                {
                    //create asset widget
                    AssetStatusWidget *assetWidget = new AssetStatusWidget(ramShot,stage,statusesList,assetsList,dbi);
                    connect(assetWidget,SIGNAL(editing(bool)),this,SLOT(setDisabled(bool)));
                    connect(this,SIGNAL(assetsListUpdated(QList<RAMAsset*>)),assetWidget,SLOT(assetsListUpdated(QList<RAMAsset*>)));
                    //add widget to cell
                    mainTable->setCellWidget(mainTable->rowCount()-1,i,assetWidget);
                }

            }

            //Add to admin tab
            shotsAdminList->addItem(ramShot->getName());

        }

        //find stage
        RAMStage *shotStage;
        foreach(RAMStage *stage,stagesList)
        {
            if (stage->getId() == shotStageId)
            {
                shotStage = stage;
                break;
            }
        }

        //find status
        RAMStatus *shotStatus;
        foreach(RAMStatus *status,statusesList)
        {
            if (status->getId() == shotStatusId)
            {
                shotStatus = status;
                break;
            }
        }

        if (shotStage)
        {
            //if shot production stage, add status
            if (shotStage->getType() == "s")
            {
                //add status
                RAMStageStatus *shotStageStatus = new RAMStageStatus(shotStatus,shotStage,shotComment);
                ramShot->addStatus(shotStageStatus,false);
            }
            //if asset production stage, add asset
            else if (shotStage->getType() == "a")
            {
                QString assetName = shot.value("assetName").toString();
                QString assetShortName = shot.value("assetShortName").toString();
                QString assetComment = shot.value("comment").toString();
                int assetId = shot.value("assetId").toInt();
                //check if asset is already created
                bool assetFound = false;
                RAMAsset *shotAsset;
                foreach(RAMAsset *a,assetsList)
                {
                    if (a->getId() == assetId)
                    {
                        shotAsset = a;
                        assetFound = true;
                        break;
                    }
                }
                if (!assetFound)
                {
                    QList<RAMStage*> stages;
                    stages << shotStage;
                    shotAsset = new RAMAsset(assetId, assetName, assetShortName, stages, shotStatus);
                    shotAsset->setComment(assetComment);
                    connect(shotAsset,SIGNAL(statusChanged(RAMAsset *)),this,SLOT(updateAssetStatus(RAMAsset *)));
                    loadAsset(shotAsset);
                }
                ramShot->addAsset(shotAsset,false);
            }
        }
    }

    mainTable->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
    mainTable->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
*/
