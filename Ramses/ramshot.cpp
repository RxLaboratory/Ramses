#include "ramshot.h"
#include <QtDebug>

RAMShot::RAMShot(DBInterface *db, int pid, int i, QString n, double d, bool updateDb , QObject *parent) : QObject(parent)
{
    projectId = pid;
    id = i;
    name = n;
    duration = d;
    dbi = db;
    if (updateDb)
    {
        dbi->addShot(projectId,id,name,duration);
    }
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
}

void RAMShot::setDuration(double d, bool updateDB)
{
    if (updateDB)
    {
        update();
    }
    duration = d;
}

void RAMShot::update()
{
    dbi->updateShot(id,name,duration);
}

void RAMShot::remove()
{
    dbi->removeShot(id);
}

bool RAMShot::operator==(RAMShot s)
{
    return s.getId() == this->getId();
}

bool RAMShot::operator==(RAMShot *s)
{
    return s->getId() == this->getId();
}
