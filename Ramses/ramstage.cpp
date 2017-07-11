#include "ramstage.h"

RAMStage::RAMStage(DBInterface *db, QString n, QString sN, bool updateDb, QString i, QObject *parent) : QObject(parent)
{
    uuid = i;
    stageName = n;
    stageShortName = sN;
    dbi = db;

    //generate uuid
    if (uuid == "") uuid = RAMUuid::generateUuidString(stageName);

    if (updateDb)
    {
        dbi->addStage(stageName,stageShortName,uuid);
    }
}

RAMStage::~RAMStage()
{

}

QString RAMStage::getId()
{
    return uuid;
}

QString RAMStage::getName()
{
    return stageName;
}

QString RAMStage::getShortName()
{
    return stageShortName;
}

void RAMStage::setId(QString id)
{
    uuid = id;
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
    dbi->updateStage(uuid,stageName,stageShortName);
}

void RAMStage::remove()
{
    dbi->removeStage(uuid);
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
