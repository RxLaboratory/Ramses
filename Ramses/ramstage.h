#ifndef RAMSTAGE_H
#define RAMSTAGE_H

#include <QObject>
#include "dbinterface.h"
#include "ramasset.h"
#include "ramuuid.h"

class RAMStage : public QObject
{
    Q_OBJECT
public:
    explicit RAMStage(DBInterface *db, QString n, QString sN, bool updateDb = false, QString i = "", QObject *parent = 0);
    ~RAMStage();
    QString getId();
    QString getName();
    QString getShortName();
    void setId(QString id);
    void setName(QString name, bool updateDb = false);
    void setShortName(QString shortName, bool updateDb = false);
    void addAsset(RAMAsset *a);
    QList<RAMAsset*> getAssets();
    void update();
    void remove();

signals:
    void stageRemoved(RAMStage*);
    void nameChanged(QString);
    void shortNameChanged(QString);
    void assetAdded(RAMAsset*);
    void assetRemoved(RAMAsset*);

public slots:
    void removeAsset(RAMAsset *a);

private:
    QString uuid;
    QString stageName;
    QString stageShortName;
    DBInterface *dbi;
    QList<RAMAsset*> assets;

protected:
    bool operator==(RAMStage s);
    bool operator==(RAMStage *s);
};

#endif // RAMSTAGE_H
