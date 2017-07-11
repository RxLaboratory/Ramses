#ifndef RAMSHOT_H
#define RAMSHOT_H

#include <QObject>
#include "dbinterface.h"
#include "ramuuid.h"

class RAMShot : public QObject
{
    Q_OBJECT
public:
    explicit RAMShot(DBInterface *db, QString n, double d = 0.0,bool updateDb = false,QString i ="" , QObject *parent = 0);
    ~RAMShot();
    QString getId();
    QString getName();
    double getDuration();
    void setName(QString n, bool updateDB = false);
    void setDuration(double d, bool updateDB = false);
    void update();
    void remove(bool updateDB = false);

signals:
    void shotRemoved(RAMShot*);
    void nameChanged(QString);
    void durationChanged(double);

private:
    double duration;
    QString name;
    QString id;
    DBInterface *dbi;

protected:
    bool operator==(RAMShot s);
    bool operator==(RAMShot *s);
};

#endif // RAMSHOT_H
