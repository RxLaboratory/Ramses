#ifndef RAMPROJECT_H
#define RAMPROJECT_H

#include <QObject>
#include <QList>
#include "dbinterface.h"
#include "ramstage.h"
#include "ramshot.h"
#include "ramuuid.h"

class RAMProject : public QObject
{
    Q_OBJECT
public:
    explicit RAMProject(DBInterface *db, QString n, QString sN,RAMStatus *defStatus, bool updateDb = false,QString i = "", QObject *parent = 0);
    ~RAMProject();
    //get
    QString getId();
    QString getName();
    QString getShortName();
    QList<RAMStage *> getStages();
    QList<RAMShot *> getShots();
    RAMShot *getShot(QString id);
    //set
    void setName(QString name, bool updateDb = false);
    void setShortName(QString shortName, bool updateDb = false);
    void update();
    void addShot(RAMShot *shot, int row = -1, bool updateDb = false);
    void removeShot(RAMShot *shot);
    //stages
    void addStage(RAMStage *s, bool updateDb = false);
    void removeStage(RAMStage *s, bool updateDb = false);
    void createStageAssets(RAMStage*stage);
    //remove
    void remove();

signals:
    void projectRemoved(RAMProject*);
    void stageAdded(RAMProject*,RAMStage*);
    void stageRemoved(RAMProject*,RAMStage*);
    void nameChanged(RAMProject*,QString);
    void shortNameChanged(RAMProject*,QString);
    void shotAdded(RAMProject*,RAMShot*,int row);
    void shotRemoved(RAMProject*,RAMShot*);

public slots:

private:
    QString projectId;
    QString projectName;
    QString projectShortName;
    QList<RAMStage *> projectStages;
    QList<RAMShot *> shots;
    RAMStatus *defaultStatus;
    DBInterface *dbi;

private slots:
    void stageDeleted(RAMStage *s);

protected:
    bool operator==(RAMProject s);
    bool operator==(RAMProject *s);
};

#endif // RAMSTATUS_H
