#include "ramasset.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

RAMAsset::RAMAsset(DBInterface *db, QString n, QString sn, RAMStatus *st, RAMStage *s, bool updateDb, QString c, int i, QObject *parent) : QObject(parent)
{
    id = i;
    name = n;
    status = st;
    shortName = sn;
    comment = c;
    stage = s;
    dbi = db;
    if (updateDb)
    {
        id = dbi->addAsset(name,shortName,status->getId(),stage->getId(),comment);
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

void RAMAsset::setName(QString n,bool updateDb = false)
{
    name = n;
    emit nameChanged(n);
}

void RAMAsset::setShortName(QString sn,bool updateDb = false)
{
    shortName = sn;
    emit shortNameChanged(sn);
}

void RAMAsset::setComment(QString c,bool updateDb = false)
{
    comment = c;
    emit commentChanged(c);
}

void RAMAsset::setStatus(RAMStatus *s, bool updateDb)
{
    status = s;
    emit statusChanged(this,s);
    emit statusChanged(s);
}

void RAMAsset::update()
{

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

bool RAMAsset::operator==(RAMAsset a)
{
    return a.getId() == id;
}

bool RAMAsset::operator==(RAMAsset *a)
{
    return a->getId() == id;
}
