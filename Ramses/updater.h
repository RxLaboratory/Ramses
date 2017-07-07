#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QTimer>
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
    void clean();
    void setUpdateFrequency(int f);
    void start();
    void stop();
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
     * Loads shots and assets for this project from remote server
     * @param project
     */
    void setCurrentProject(RAMProject *project);
    /**
     * @brief Updates all the data from remote server
     */
    void updateAll();

private slots:
    void dataReceived(QJsonObject data);

private:
    /**
     * @brief Connects signals and slots
     * Run only once by the constructor
     */
    void mapEvents();

    /**
     * @brief Run when the statuses are received from the remote server
     * Updates the statuses
     * @param newStatuses
     */
    void gotStatuses(QJsonValue newStatuses);
    /**
     * @brief Run when the stages are received from the remote server
     * Updates the stages
     * @param newStages
     */
    void gotStages(QJsonValue newStages);
    /**
     * @brief Run when the projects are received from the remote server
     * Updates the projects
     * @param newProjects
     */
    void gotProjects(QJsonValue newProjects);
    /**
     * @brief Run when the shots are received from the remote server
     * Updates the shots
     * @param newShots
     */
    void gotShots(QJsonValue newShots);
    /**
     * @brief Run when the assets are received from the remote server
     * Updates the assets
     * @param newAssets
     */
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

    //Timer for pulling data
    QTimer pullTimer;


};

#endif // UPDATER_H
