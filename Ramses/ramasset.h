#ifndef RAMASSET_H
#define RAMASSET_H

#include <QObject>
#include "ramstatus.h"
#include "ramshot.h"
#include "ramstage.h"
#include "dbinterface.h"

class RAMAsset : public QObject
{
    Q_OBJECT
public:
    explicit RAMAsset(DBInterface *db,int i, QString n, QString sn, RAMStatus *st, RAMStage *s, QString c = "",bool updateDb = false, QObject *parent = 0);
    RAMStatus* getStatus();
    QString getName();
    QString getShortName();
    int getId();
    QString getComment();
    RAMStage *getStage();
    QList<RAMShot*> getAssignments();
    void assign(RAMShot *shot,bool updateDb = false);
    void setName(QString n);
    void setShortName(QString sn);
    void setComment(QString c);
    void setStatus(RAMStatus *s);

signals:
    void statusChanged(RAMAsset *);
    void statusChanged();

public slots:

private:
    QString name;
    QString shortName;
    int id;
    QString comment;
    RAMStatus *status;
    RAMStage *stage;
    QList<RAMShot*> assignments;
    DBInterface *dbi;
protected:
    bool operator==(RAMAsset a);
    bool operator==(RAMAsset *a);
};

#endif // RAMASSET_H
