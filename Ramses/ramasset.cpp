#include "ramasset.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

RAMAsset::RAMAsset(int i, QString n, QString sn, QList<RAMStage *> s, RAMStatus *st, QObject *parent) : QObject(parent)
{
    name = n;
    id = i;
    status = st;
    stages = s;
    shortName = sn;
}

int RAMAsset::getId()
{
    return id;
}

QString RAMAsset::getComment()
{
    return comment;
}

void RAMAsset::setComment(QString c)
{
    comment = c;
}

RAMStatus* RAMAsset::getStatus()
{
    return status;
}

QString RAMAsset::getName()
{
    return name;
}

QList<RAMStage*> RAMAsset::getStages()
{
    return stages;
}

QString RAMAsset::getShortName()
{
    return shortName;
}

void RAMAsset::setStatus(RAMStatus *s)
{
    status = s;
    emit statusChanged(this);
    emit statusChanged();
}

bool RAMAsset::operator==(RAMAsset a)
{
    return a.getId() == id;
}

bool RAMAsset::operator==(RAMAsset *a)
{
    return a->getId() == id;
}
