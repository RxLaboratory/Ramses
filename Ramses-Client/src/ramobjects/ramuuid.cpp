#include "ramuuid.h"

RamUuid::RamUuid()
{

}

QString RamUuid::generateUuidString(QString base)
{
    return QUuid::createUuidV5(QUuid::createUuid(),base).toString().replace("{","").replace("}","");
}


QString RamUuid::generateUuidString()
{
    return QUuid::createUuid().toString().replace("{","").replace("}","");
}
