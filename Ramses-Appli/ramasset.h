#ifndef RAMASSET_H
#define RAMASSET_H

#include <QObject>
#include "ramstatus.h"
#include "ramshot.h"
#include "dbinterface.h"
#include "ramuuid.h"

class RAMAsset : public QObject
{
    Q_OBJECT
public:
    explicit RAMAsset(DBInterface *db, QString n, QString sn, RAMStatus *st, QString stageId, bool updateDb = false, QString c = "", QString i = "", QString projectId = "", QObject *parent = 0);
    ~RAMAsset();
    RAMStatus* getStatus();
    QString getName();
    QString getShortName();
    QString getId();
    QString getComment();
    QList<RAMShot*> getAssignments();
    void assign(RAMShot *shot,bool updateDb = false);
    void unAssign(RAMShot *shot,bool updateDb = false);
    void setName(QString n,bool updateDb = false);
    void setShortName(QString sn,bool updateDb = false);
    void setComment(QString c,bool updateDb = false);
    void setStatus(RAMStatus *s,bool updateDb = false);
    void setId(QString i);
    void update();

signals:
    void statusChanged(RAMAsset *,RAMStatus*);
    void statusChanged(RAMStatus*);
    void nameChanged(QString);
    void shortNameChanged(QString);
    void commentChanged(QString);
    void assetRemoved(RAMAsset*);
    void assetUnAssigned(RAMShot*,RAMAsset*);
    void assetAssigned(RAMShot*,RAMAsset*);

public slots:
    void remove(bool updateDB = false);

private:
    QString name;
    QString shortName;
    QString id;
    QString comment;
    RAMStatus *status;
    QList<RAMShot*> assignments;
    DBInterface *dbi;

private slots:
    void statusDeleted(RAMStatus *s);
    void shotDeleted(RAMShot *s);

protected:
    bool operator==(RAMAsset a);
    bool operator==(RAMAsset *a);
};

#endif // RAMASSET_H
