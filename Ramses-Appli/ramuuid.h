#ifndef RAMUUID_H
#define RAMUUID_H

#include <QObject>
#include <QUuid>

class RAMUuid : public QObject
{
    Q_OBJECT
public:
    explicit RAMUuid(QObject *parent = nullptr);
    static QString generateUuidString(QString base = "");  
    static QString generateUuidString();
signals:

public slots:
};

#endif // RAMUUID_H
