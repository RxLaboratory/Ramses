#ifndef RAMSHOT_H
#define RAMSHOT_H

#include <QObject>
#include "dbinterface.h"

class RAMShot : public QObject
{
    Q_OBJECT
public:
    explicit RAMShot(DBInterface *db,int pid,int i , QString n, double d = 0.0,int so = 0,bool updateDb = false, QObject *parent = 0);
    int getId();
    QString getName();
    int getShotOrder();
    double getDuration();
    void setName(QString n, bool updateDB = false);
    void setDuration(double d, bool updateDB = false);
    void setShotOrder(int o);
    void update();
    void remove();

private:
    double duration;
    QString name;
    int shotOrder;
    int projectId;
    int id;
    DBInterface *dbi;

protected:
    bool operator==(RAMShot s);
    bool operator==(RAMShot *s);
    bool operator>(RAMShot *s);
    bool operator>(RAMShot s);
    bool operator<(RAMShot *s);
    bool operator<(RAMShot s);
    bool operator>=(RAMShot *s);
    bool operator>=(RAMShot s);
    bool operator<=(RAMShot *s);
    bool operator<=(RAMShot s);
};

#endif // RAMSHOT_H
