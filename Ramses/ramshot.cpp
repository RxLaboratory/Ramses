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

void RAMShot::addStatus(RAMStageStatus *s)
{
    connect(s,SIGNAL(statusChanged(RAMStatus*,RAMStage*)),this,SLOT(updateShotStatus(RAMStatus*,RAMStage*)));
    statuses.append(s);
}

QList<RAMStageStatus *> RAMShot::getStatuses()
{
    return statuses;
}

void RAMShot::updateShotStatus(RAMStatus* status,RAMStage* stage)
{
    emit shotStatusUpdated(status,stage,this);
}

void RAMShot::addAsset(RAMAsset *a)
{
    assets.append(a);
}

QList<RAMAsset *> RAMShot::getAssets()
{
    return assets;
}

