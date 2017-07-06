#include "ramshot.h"
#include <QtDebug>

RAMShot::RAMShot(DBInterface *db, int i, QString n, double d, bool updateDb , QObject *parent) : QObject(parent)
{
    id = i;
    name = n;
    duration = d;
    dbi = db;
    if (updateDb)
    {
        dbi->addShot(id,name,duration);
    }
}

RAMShot::~RAMShot()
{

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

void RAMShot::setName(QString n, bool updateDB)
{
    if (updateDB)
    {
        update();
    }
    name = n;
    emit nameChanged(n);
}

void RAMShot::setDuration(double d, bool updateDB)
{
    if (updateDB)
    {
        update();
    }
    duration = d;
    emit durationChanged(d);
}

void RAMShot::update()
{
    dbi->updateShot(id,name,duration);
}

void RAMShot::remove(bool updateDB)
{
    if (updateDB) dbi->removeShot(id);
    emit shotRemoved(this);
}

bool RAMShot::operator==(RAMShot s)
{
    return s.getId() == this->getId();
}

bool RAMShot::operator==(RAMShot *s)
{
    return s->getId() == this->getId();
}
