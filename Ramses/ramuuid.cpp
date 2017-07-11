#include "ramuuid.h"

RAMUuid::RAMUuid(QObject *parent) : QObject(parent)
{

}

QString RAMUuid::generateUuidString(QString base)
{
    return QUuid::createUuidV5(QUuid::createUuid(),base).toString().replace("{","").replace("}","");
}


QString RAMUuid::generateUuidString()
{
    return QUuid::createUuid().toString().replace("{","").replace("}","");
}
