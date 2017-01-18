#include "ramstage.h"

RAMStage::RAMStage(int i,QString n,QString sN, QString t, QObject *parent) : QObject(parent)
{
    stageId = i;
    stageName = n;
    stageShortName = sN;
    stageType = t;
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

QString RAMStage::getType()
{
    return stageType;
}

