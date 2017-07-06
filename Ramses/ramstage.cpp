#include "ramstage.h"

RAMStage::RAMStage(DBInterface *db, QString n, QString sN, int i, bool updateDb, QObject *parent) : QObject(parent)
{
    stageId = i;
    stageName = n;
    stageShortName = sN;
    dbi = db;
    if (updateDb)
    {
        dbi->addStage(stageName,stageShortName,stageId);
    }
}

RAMStage::~RAMStage()
{

}

int RAMStage::getId()
{
    return stageId;
}

QString RAMStage::getName()
{
    return stageName;
}

QString RAMStage::getShortName()
{
    return stageShortName;
}

void RAMStage::setId(int id, bool updateDb)
{
    stageId = id;
    if (updateDb) update();
}

void RAMStage::setName(QString name, bool updateDb)
{
    stageName = name;
    if (updateDb) update();
    emit nameChanged(name);
}

void RAMStage::setShortName(QString shortName, bool updateDb)
{
    stageShortName = shortName;
    if (updateDb) update();
    emit shortNameChanged(shortName);
}

void RAMStage::addAsset(RAMAsset *a)
{
    if (assets.indexOf(a) >= 0) return;
    assets << a;
    a->setParent(this);
    connect(a,SIGNAL(assetRemoved(RAMAsset*)),this,SLOT(removeAsset(RAMAsset*)));
    emit assetAdded(a);
}

QList<RAMAsset *> RAMStage::getAssets()
{
    return assets;
}

void RAMStage::removeAsset(RAMAsset *a)
{
    assets.removeAll(a);
    emit assetRemoved(a);
}

void RAMStage::update()
{
    dbi->updateStage(stageId,stageName,stageShortName);
}

void RAMStage::remove()
{
    dbi->removeStage(stageId);
    emit stageRemoved(this);
}

bool RAMStage::operator==(RAMStage s)
{
    return s.getId() == this->getId();
}

bool RAMStage::operator==(RAMStage *s)
{
    return s->getId() == this->getId();
}
