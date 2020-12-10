#include "ramproject.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

RAMProject::RAMProject(DBInterface *db, QString n, QString sN, RAMStatus *defStatus, bool updateDb, QString i, QObject *parent) : QObject(parent)
{
    projectId = i;
    projectName = n;
    projectShortName = sN;
    defaultStatus = defStatus;
    dbi = db;

    if (projectId == "") projectId = RAMUuid::generateUuidString(projectName);

    if (updateDb)
    {
        dbi->addProject(projectName,projectShortName,projectId);
    }
}

RAMProject::~RAMProject()
{

}

QString RAMProject::getId()
{
    return projectId;
}

QString RAMProject::getName()
{
    return projectName;
}

QString RAMProject::getShortName()
{
    return projectShortName;
}

QList<RAMStage *> RAMProject::getStages()
{
    return projectStages;
}

QList<RAMShot *> RAMProject::getShots()
{
    return shots;
}

RAMShot *RAMProject::getShot(QString id)
{
    foreach(RAMShot *rs,shots)
    {
        if (rs->getId() == id) return rs;
    }
    return 0;
}

void RAMProject::setName(QString name, bool updateDb)
{
    projectName = name;
    if (updateDb) update();
    emit nameChanged(this,name);
}

void RAMProject::setShortName(QString shortName, bool updateDb)
{
    projectShortName = shortName;
    if (updateDb) update();
    emit shortNameChanged(this,shortName);
}

void RAMProject::update()
{
    dbi->updateProject(projectId,projectName,projectShortName);
}

void RAMProject::addShot(RAMShot *shot, int row,bool updateDb)
{
    if (shots.indexOf(shot) >= 0)return;
    if (row >= 0 && row <= shots.count()) shots.insert(row,shot);
    else shots << shot;
    shot->setParent(this);
    if (updateDb) dbi->insertShot(shot->getId(),projectId,row);
    emit shotAdded(this,shot,row);
}

void RAMProject::removeShot(RAMShot *shot)
{
    shots.removeAll(shot);
    emit shotRemoved(this,shot);
}

void RAMProject::addStage(RAMStage *s, bool updateDb)
{
    if (projectStages.indexOf(s) >= 0) return;
    projectStages << s;

    if (updateDb) dbi->addProjectStage(projectId,s->getId());

    //connect
    connect(s,SIGNAL(stageRemoved(RAMStage*)),this,SLOT(stageDeleted(RAMStage*)));

    emit stageAdded(this,s);
}

void RAMProject::removeStage(RAMStage *s, bool updateDb)
{
    projectStages.removeAll(s);
    if (updateDb) dbi->removeProjectStage(projectId,s->getId());
    emit stageRemoved(this,s);
}

void RAMProject::remove()
{
    dbi->removeProject(projectId);
    emit projectRemoved(this);
}

void RAMProject::createStageAssets(RAMStage *stage)
{
    //list shots which already have assets
    QList<RAMShot*> shotsAssigned;
    foreach(RAMAsset *asset,stage->getAssets())
    {
        shotsAssigned << asset->getAssignments();
    }

    //for each shot, create asset if needed
    QList<QStringList> assetsToCreate;
    QList<RAMAsset*> newAssets;
    foreach(RAMShot *shot,shots)
    {
        if (shotsAssigned.indexOf(shot) < 0)
        {
            QString name = "Shot " + shot->getName() + " " + stage->getName();
            QString shortName = stage->getShortName() + " " + shot->getName();
            RAMAsset *asset = new RAMAsset(dbi,name,shortName,defaultStatus,stage->getId(),false,"","",projectId);
            //the asset values for the db
            QStringList newAsset;
            newAsset << name << shortName << defaultStatus->getId() << "" << shot->getId() << asset->getId();
            assetsToCreate << newAsset;
            //the new RAMAsset
            newAssets << asset;
            //assign the asset
            asset->assign(shot,false);
            stage->addAsset(asset);
        }
    }
    //create and assign in db
    dbi->addAssignAssets(assetsToCreate,stage->getId(),projectId);
}

void RAMProject::stageDeleted(RAMStage *s)
{
    removeStage(s,false);
}

bool RAMProject::operator==(RAMProject s)
{
    return s.getId() == projectId;
}

bool RAMProject::operator==(RAMProject *s)
{
    return s->getId() == projectId;
}
