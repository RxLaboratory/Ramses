#include "ramstatus.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

RAMStatus::RAMStatus(DBInterface *db, QString n, QString sN, QColor c, QString d, QString id, bool updateDb, QObject *parent) : QObject(parent)
{

    statusName = n;
    statusShortName = sN;
    statusColor = c;
    statusDescription = d;
    uuid = id;
    dbi = db;

    if (uuid == "")
    {
        uuid = RAMUuid::generateUuidString(statusName);
    }

    if (updateDb)
    {
        dbi->addStatus(statusName,statusShortName,statusColor.name(),statusDescription,uuid);
    }
}

RAMStatus::~RAMStatus()
{

}

// GET

QString RAMStatus::getId()
{
    return uuid;
}

QString RAMStatus::getName()
{
    return statusName;
}

QString RAMStatus::getShortName()
{
    return statusShortName;
}

QColor RAMStatus::getColor()
{
    return statusColor;
}

QString RAMStatus::getDescription()
{
    return statusDescription;
}

// SET

void RAMStatus::setName(QString name, bool updateDb)
{
    statusName = name;
    if (updateDb) update();
    emit nameChanged(name);
}

void RAMStatus::setShortName(QString shortName, bool updateDb)
{
    statusShortName = shortName;
    if (updateDb) update();
    emit shortNameChanged(shortName);
}

void RAMStatus::setColor(QColor color, bool updateDb)
{
    statusColor = color;
    if (updateDb) update();
    emit colorChanged(color);
}

void RAMStatus::setDescription(QString description, bool updateDb)
{
    statusDescription = description;
    if (updateDb) update();
    emit descriptionChanged(description);
}

void RAMStatus::update()
{
    dbi->updateStatus(uuid,statusName,statusShortName,statusColor.name(),statusDescription);
}

void RAMStatus::remove()
{
    dbi->removeStatus(uuid);
    emit statusRemoved(this);
}

bool RAMStatus::operator==(RAMStatus s)
{
    return this->getId() == s.getId();
}

bool RAMStatus::operator==(RAMStatus *s)
{
    return this->getId() == s->getId();
}
