#ifndef RAMUUID_H
#define RAMUUID_H

#include <QString>
#include <QUuid>

class RamUuid
{
public:
    RamUuid();
    static QString generateUuidString(QString base = "");
    static QString generateUuidString();
};

#endif // RAMUUID_H
