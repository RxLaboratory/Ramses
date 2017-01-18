#include "ramstatus.h"
#include <QtDebug>

RAMStatus::RAMStatus(int i, QString n, QString sN, QColor c, QString d, QObject *parent) : QObject(parent)
{
    statusId = i;
    statusName = n;
    statusShortName = sN;
    statusColor = c;
    statusDescription = d;
}

int RAMStatus::getId()
{
    return statusId;
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

