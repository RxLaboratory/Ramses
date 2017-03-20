#ifndef RAMSHOT_H
#define RAMSHOT_H

#include <QObject>
#include "ramstagestatus.h"
#include "ramasset.h"
#include "dbinterface.h"

class RAMShot : public QObject
{
    Q_OBJECT
public:
    explicit RAMShot(DBInterface *db,int i, QString n, double d = 0.0,int so = 0, QObject *parent = 0);
    int getId();
    QString getName();
    void setName(QString n, bool updateDB = false);
    double getDuration();
    void setDuration(double d, bool updateDB = false);
    int getShotOrder();
    void setShotOrder(int o, bool updateDB = false);
    void addStatus(RAMStageStatus *s, bool updateDB = true);
    QList<RAMStageStatus *> getStatuses();
    void addAsset(RAMAsset *a,bool updateDB = true);
    QList<RAMAsset *> getAssets();

signals:
    void stageStatusUpdated(RAMStatus*,RAMStage*,RAMShot*);
    void assetAdded(RAMAsset*,RAMShot*);
    void statusAdded(RAMStageStatus*,RAMShot*);

public slots:
    void updateShotStatus(RAMStatus* status,RAMStage* stage);

private:
    double duration;
    QString name;
    int id;
    int shotOrder;
    QList<RAMStageStatus *> statuses;
    QList<RAMAsset *> assets;
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
