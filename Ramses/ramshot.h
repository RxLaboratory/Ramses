#ifndef RAMSHOT_H
#define RAMSHOT_H

#include <QObject>
#include "dbinterface.h"

class RAMShot : public QObject
{
    Q_OBJECT
public:
    explicit RAMShot(DBInterface *db,int i , QString n, double d = 0.0,bool updateDb = false, QObject *parent = 0);
    ~RAMShot();
    int getId();
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
    int id;
    DBInterface *dbi;

protected:
    bool operator==(RAMShot s);
    bool operator==(RAMShot *s);
};

#endif // RAMSHOT_H
