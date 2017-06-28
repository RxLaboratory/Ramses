#include "ramasset.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

RAMAsset::RAMAsset(DBInterface *db, int i, QString n, QString sn, RAMStatus *st, RAMStage *s, QString c, bool updateDb, QObject *parent) : QObject(parent)
{
    name = n;
    id = i;
    status = st;
    shortName = sn;
    comment = c;
    stage = s;
    dbi = db;
    if (updateDb)
    {
        dbi->addAsset(name,shortName,status->getId(),comment,id);
    }
}

int RAMAsset::getId()
{
    return id;
}

QString RAMAsset::getComment()
{
    return comment;
}

RAMStage *RAMAsset::getStage()
{
    return stage;
}

QList<RAMShot *> RAMAsset::getAssignments()
{
    return assignments;
}

void RAMAsset::assign(RAMShot *shot, bool updateDb)
{
    assignments << shot;
    if (updateDb)
    {
        dbi->assignAsset(id,shot->getId());
    }
}

void RAMAsset::setName(QString n)
{
    name = n;
}

void RAMAsset::setShortName(QString sn)
{
    shortName = sn;
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
