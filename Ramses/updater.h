#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include "dbinterface.h"
#include "ramproject.h"
#include "ramstage.h"
#include "ramstatus.h"
#include "ramshot.h"
#include "ramasset.h"

class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(DBInterface *db,QObject *parent = 0);
    void updateAll();
    void clean();
    //statuses
    void addStatus(RAMStatus*status);
    void removeStatus(RAMStatus *status);
    QList<RAMStatus*> getStatuses();
    RAMStatus *getStatus(int id);
    RAMStatus *getDefaultStatus();
    //stages
    void addStage(RAMStage*stage);
    void removeStage(RAMStage *stage);
    QList<RAMStage*> getStages();
    RAMStage* getStage(int id);
    //projects
    QList<RAMProject*> getProjects();
    void addProject(RAMProject*project);
    void removeProject(RAMProject *project);
    RAMProject *getProject(int id);
    RAMProject *getCurrentProject();

signals:
    void message(QString,QString);
    void currentProjectChanged(RAMProject*);
    void currentProjectChanging();
    void statusRemoved(RAMStatus*);
    void statusAdded(RAMStatus*);
    void stageRemoved(RAMStage*);
    void stageAdded(RAMStage*);
    void projectRemoved(RAMProject*);
    void projectAdded(RAMProject*);
    void working(bool);

public slots:
    /**
     * @brief Sets the current project
     * Load shots and assets for this project
     * @param project
     */
    void setCurrentProject(RAMProject *project);

private slots:
    void dataReceived(QJsonObject data);

private:
    void mapEvents();

    void gotStatuses(QJsonValue newStatuses);
    void gotStages(QJsonValue newStages);
    void gotProjects(QJsonValue newProjects);
    void gotShots(QJsonValue newShots);
    void gotAssets(QJsonValue newAssets);

    //lists
    QList<QObject*> removedItems;
    QList<RAMStatus*> statuses;
    QList<RAMStage*> stages;
    QList<RAMProject*> projects;

    /**
     * @brief Keeps the currently selected project
     * This is needed to load only current shots and assets
     * and improve performance
     */
    RAMProject *currentProject;
    /**
     * @brief The status assigned to assets by default
     */
    RAMStatus *defaultStatus;

    //DBI
    DBInterface *dbi;


};

#endif // UPDATER_H
