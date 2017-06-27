#include "ramshot.h"
#include <QtDebug>

RAMShot::RAMShot(DBInterface *db, int pid, int i, QString n, double d, int so, bool updateDb , QObject *parent) : QObject(parent)
{
    projectId = pid;
    id = i;
    name = n;
    duration = d;
    shotOrder = so;
    dbi = db;
    if (updateDb)
    {
        dbi->addShot(projectId,id,name,duration,shotOrder);
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

int RAMShot::getShotOrder()
{
    return shotOrder;
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

void RAMShot::setShotOrder(int o)
{
    shotOrder = o;
}

void RAMShot::update()
{
    dbi->updateShot(id,name,duration);
}

bool RAMShot::operator==(RAMShot s)
{
    return s.getId() == this->getId();
}

bool RAMShot::operator==(RAMShot *s)
{
    return s->getId() == this->getId();
}

bool RAMShot::operator>(RAMShot *s)
{
    return this->getShotOrder() > s->getShotOrder();
}

bool RAMShot::operator>(RAMShot s)
{
    return this->getShotOrder() > s.getShotOrder();
}

bool RAMShot::operator<(RAMShot *s)
{
    return this->getShotOrder() < s->getShotOrder();
}

bool RAMShot::operator<(RAMShot s)
{
    return this->getShotOrder() < s.getShotOrder();
}

bool RAMShot::operator>=(RAMShot *s)
{
    return this->getShotOrder() >= s->getShotOrder();
}

bool RAMShot::operator>=(RAMShot s)
{
    return this->getShotOrder() >= s.getShotOrder();
}

bool RAMShot::operator<=(RAMShot *s)
{
    return this->getShotOrder() <= s->getShotOrder();
}

bool RAMShot::operator<=(RAMShot s)
{
    return this->getShotOrder() <= s.getShotOrder();
}
