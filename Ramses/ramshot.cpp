#include "ramshot.h"
#include <QtDebug>

RAMShot::RAMShot(DBInterface *db, int i, QString n, double d, int so, QObject *parent) : QObject(parent)
{
    id = i;
    name = n;
    duration = d;
    shotOrder = so;
    dbi = db;
}

int RAMShot::getId()
{
    return id;
}

QString RAMShot::getName()
{
    return name;
}

double RAMShot::getDuration()
{
    return duration;
}

int RAMShot::getShotOrder()
{
    return shotOrder;
}

void RAMShot::setName(QString n, bool updateDB)
{
    if (updateDB)
    {
        //TODO dbi
    }
    name = n;
}

void RAMShot::setDuration(double d, bool updateDB)
{
    if (updateDB)
    {
        //TODO dbi
    }
    duration = d;
}

void RAMShot::setShotOrder(int o, bool updateDB)
{
    if (updateDB)
    {
        //TODO dbi
    }
    shotOrder = o;
}

void RAMShot::addStatus(RAMStageStatus *s,bool updateDB)
{
    if (updateDB)
    {
        //TODO dbi
    }
    connect(s,SIGNAL(statusChanged(RAMStatus*,RAMStage*)),this,SLOT(updateShotStatus(RAMStatus*,RAMStage*)));
    statuses.append(s);
    emit statusAdded(s,this);
}

QList<RAMStageStatus *> RAMShot::getStatuses()
{
    return statuses;
}

void RAMShot::updateShotStatus(RAMStatus* status,RAMStage* stage)
{
    emit stageStatusUpdated(status,stage,this);
}

void RAMShot::addAsset(RAMAsset *a, bool updateDB)
{
    if (updateDB)
    {
        dbi->assignAsset(a->getId(),id);
    }
    assets.append(a);
    emit assetAdded(a,this);
}

QList<RAMAsset *> RAMShot::getAssets()
{
    return assets;
}

bool RAMShot::operator==(RAMShot s)
{
    return s.getId() == this->getId();
}

bool RAMShot::operator==(RAMShot *s)
{
    return s->getId() == this->getId();
}

bool RAMShot::operator>(RAMShot *s)
{
    return this->getShotOrder() > s->getShotOrder();
}

bool RAMShot::operator>(RAMShot s)
{
    return this->getShotOrder() > s.getShotOrder();
}

bool RAMShot::operator<(RAMShot *s)
{
    return this->getShotOrder() < s->getShotOrder();
}

bool RAMShot::operator<(RAMShot s)
{
    return this->getShotOrder() < s.getShotOrder();
}

bool RAMShot::operator>=(RAMShot *s)
{
    return this->getShotOrder() >= s->getShotOrder();
}

bool RAMShot::operator>=(RAMShot s)
{
    return this->getShotOrder() >= s.getShotOrder();
}

bool RAMShot::operator<=(RAMShot *s)
{
    return this->getShotOrder() <= s->getShotOrder();
}

bool RAMShot::operator<=(RAMShot s)
{
    return this->getShotOrder() <= s.getShotOrder();
}
