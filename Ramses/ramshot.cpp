#include "ramshot.h"
#include <QtDebug>

RAMShot::RAMShot(int i, QString n, double d, QObject *parent) : QObject(parent)
{
    id = i;
    name = n;
    duration = d;
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
