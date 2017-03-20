#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <algorithm>
#include "dbinterface.h"
#include "ramshot.h"
#include "ramproject.h"

class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(DBInterface *db, QList<RAMShot*> *sl, QObject *parent = 0);

signals:
    void newShot(RAMShot *);

public slots:
    void getShots(RAMProject *project);

private slots:
    void gotShots(bool success, QString message, QJsonValue shots);

private:
    DBInterface *dbi;
    QList<RAMShot*> *shotsList;
};

#endif // UPDATER_H
