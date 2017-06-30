#ifndef RAMPROJECT_H
#define RAMPROJECT_H

#include <QObject>
#include <QList>
#include "dbinterface.h"
#include "ramstage.h"
#include "ramshot.h"

class RAMProject : public QObject
{
    Q_OBJECT
public:
    explicit RAMProject(DBInterface *db,int i, QString n, QString sN, bool updateDb, QObject *parent = 0);
    ~RAMProject();
    //get
    int getId();
    QString getName();
    QString getShortName();
    QList<RAMStage *> getStages();
    //set
    void setName(QString name, bool updateDb = false);
    void setShortName(QString shortName, bool updateDb = false);
    void update();
    //stages
    void addStage(RAMStage *s, bool updateDb = false);
    void removeStage(RAMStage *s, bool updateDb = false);
    //remove
    void remove();

signals:
    void projectRemoved(RAMProject*);
    void stageAdded(RAMProject*,RAMStage*);
    void stageRemoved(RAMProject*,RAMStage*);
    void nameChanged(RAMProject*,QString);
    void shortNameChanged(RAMProject*,QString);

public slots:

private:
    int projectId;
    QString projectName;
    QString projectShortName;
    QList<RAMStage *> projectStages;
    DBInterface *dbi;

private slots:
    void stageDeleted(RAMStage *s);

protected:
    bool operator==(RAMProject s);
    bool operator==(RAMProject *s);
};

#endif // RAMSTATUS_H
