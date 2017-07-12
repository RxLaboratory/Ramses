#ifndef RAMUSER_H
#define RAMUSER_H

#include <QObject>
#include "ramuuid.h"
#include "dbinterface.h"

class RAMUser : public QObject
{
    Q_OBJECT
public:
    explicit RAMUser(DBInterface *db,QString user,bool updateDb = false,QString password = "password",QString id = "", QString first = "", QString last = "",QObject *parent = nullptr);
    QString getUsername();
    QString getFirstname();
    QString getLastname();
    QString getPassHash();
    QString getId();
    void setUsername(QString user);
    void setFirstname(QString first);
    void setLastname(QString last);
    void setPassword(QString pass);
    void login();
    void update();
    QString generatePassHash(QString pass);

signals:

public slots:

private slots:
    void gotData(QJsonObject data);
    void connected(QJsonValue mess);

private:

    QString username;
    QString firstname;
    QString lastname;
    QString passHash;
    QString uuid;
    DBInterface *dbi;
};

#endif // RAMUSER_H
