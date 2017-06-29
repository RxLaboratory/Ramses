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
    explicit RAMAsset(DBInterface *db, QString n, QString sn, RAMStatus *st, RAMStage *s, bool updateDb = false, QString c = "",int i = -1, QObject *parent = 0);
    RAMStatus* getStatus();
    QString getName();
    QString getShortName();
    int getId();
    QString getComment();
    RAMStage *getStage();
    QList<RAMShot*> getAssignments();
    void assign(RAMShot *shot,bool updateDb = false);
    void setName(QString n,bool updateDb = false);
    void setShortName(QString sn,bool updateDb = false);
    void setComment(QString c,bool updateDb = false);
    void setStatus(RAMStatus *s,bool updateDb = false);
    void update();

signals:
    void statusChanged(RAMAsset *,RAMStage*);
    void statusChanged(RAMStage*);
    void nameChanged(QString);
    void shortNameChanged(QString);
    void commentChanged(QString);

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
