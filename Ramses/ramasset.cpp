#include "ramasset.h"

RAMAsset::RAMAsset(QString n, QString sn, RAMStage *s, RAMStatus *st, QObject *parent) : QObject(parent)
{
    name = n;
    status = st;
    stage = s;
    shortName = sn;
}

RAMStatus* RAMAsset::getStatus()
{
    return status;
}

QString RAMAsset::getName()
{
    return name;
}

RAMStage* RAMAsset::getStage()
{
    return stage;
}

QString RAMAsset::getShortName()
{
    return shortName;
}
