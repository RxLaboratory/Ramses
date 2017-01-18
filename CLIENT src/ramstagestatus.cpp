#include "ramstagestatus.h"

RAMStageStatus::RAMStageStatus(RAMStatus *stat, RAMStage *stag, QString com, QObject *parent) : QObject(parent)
{
    status = stat;
    stage = stag;
    comment = com;
}

void RAMStageStatus::setStatus(RAMStatus *s)
{
    status = s;
    emit statusChanged(s,stage);
}

void RAMStageStatus::setStage(RAMStage *s)
{
    stage = s;
}

void RAMStageStatus::setComment(QString c)
{
    comment = c;
}

RAMStage* RAMStageStatus::getStage()
{
    return stage;
}

RAMStatus* RAMStageStatus::getStatus()
{
    return status;
}

QString RAMStageStatus::getComment()
{
    return comment;
}
