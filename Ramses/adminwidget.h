#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include "ui_adminwidget.h"
#include "dbinterface.h"
#include "ramstatus.h"
#include "ramstage.h"
#include "ramproject.h"
#include <QColorDialog>
#include <QErrorMessage>

class AdminWidget : public QWidget, private Ui::AdminWidget
{
    Q_OBJECT

public:
    explicit AdminWidget(DBInterface *db,QWidget *parent = 0);
    /**
     * @brief initializes the admin widget with all lists and items
     * @param status    The status list
     */
    void init(QList<RAMStatus*> status, QList<RAMStage *> stages, QList<RAMProject *> projects, QList<RAMShot *> shots,RAMProject *current);
    void setCurrentProject(RAMProject *project);

signals:
    void statusCreated(RAMStatus*);
    void stageCreated(RAMStage*);
    void projectCreated(RAMProject*);
    void shotCreated(RAMShot*,int row);

private slots:
    void on_adminTab_currentChanged(int index);
    //status
    void on_addStatusButton_clicked();
    void on_statusAdminList_itemClicked(QListWidgetItem *i);
    void on_statusColorButton_clicked();
    void on_statusApplyButton_clicked();
    void on_removeStatusButton_clicked();
    //stage
    void on_addStageButton_clicked();
    void on_stagesAdminList_itemClicked(QListWidgetItem *item);
    void on_stageApplyButton_clicked();
    void on_removeStageButton_clicked();
    //project
    void on_addProjectButton_clicked();
    void on_projectAdminList_itemClicked(QListWidgetItem *item);
    void on_projectApplyButton_clicked();
    void on_removeProjectButton_clicked();
    void on_projectAddStageButton_clicked();
    void on_removeStageProjectButton_clicked();
    //shots
    void on_addShotButton_clicked();
    void on_batchAddShotButton_clicked();
    void on_importShotsButton_clicked();
    void on_shotsAdminList_itemClicked(QListWidgetItem *item);
    void on_shotApplyButton_clicked();
    void on_removeShotButton_clicked();
    void on_moveShotUpButton_clicked();
    void on_moveShotDownButton_clicked();



private:
    // ----------------- GENERAL ------------------------
    QErrorMessage *error;

    // ----------------- STATUSES -----------------------

    /**
     * @brief the current list of statuses
     */
    QList<RAMStatus*> statusList;
    /**
     * @brief Creates the UI item for a status
     * @param rs    the status
     */
    void newStatus(RAMStatus *rs);
    /**
     * @brief Gets a status using its Id
     * @param id    The status id
     * @return The status
     */
    RAMStatus *getStatus(int id);
    /**
     * @brief Resets the admin panel of the statuses
     */
    void statusesAdminReset();
    /**
     * @brief The interface to the local and remote Databases
     */
    DBInterface *dbi;

    // ----------------- STAGES -----------------------

    /**
     * @brief The current list of stages
     */
    QList<RAMStage*> stagesList;
    /**
     * @brief Creates a new stage and adds it to the list
     * @param rs the stage
     */
    void newStage(RAMStage *rs);
    /**
     * @brief Gets a stage using its Id
     * @param id    The stage id
     * @return The stage
     */
    RAMStage *getStage(int id);
    /**
     * @brief Resets the admin panel of the stages
     */
    void stagesAdminReset();

    // ----------------- PROJECTS -----------------------

    /**
     * @brief The current list of projects
     */
    QList<RAMProject*> projectsList;
    /**
     * @brief Creates a new project and adds it to the list
     * @param rp the stage
     */
    void newProject(RAMProject *rp);
    /**
     * @brief Gets a project using its Id
     * @param id    The project id
     * @return The project
     */
    RAMProject *getProject(int id);
    /**
     * @brief Resets the admin panel for the projects
     */
    void projectsAdminReset();
    /**
     * @brief The project currently selected
     */
    RAMProject *currentProject;

    // ----------------- SHOTS -----------------------

    /**
     * @brief The current list of shots
     */
    QList<RAMShot*> shotsList;
    /**
     * @brief Creates a new shot and adds it to the list
     * @param rs the shot
     */
    void newShot(RAMShot *rs, int row);
    /**
     * @brief Resets the order of the shots in the DB
     */
    void resetShotsOrder();
    /**
     * @brief Gets a shot using its Id
     * @param id    The shot id
     * @return The shot
     */
    RAMShot *getShot(int id);
    /**
     * @brief Resets the admin panel for the shots
     */
    void shotsAdminReset();
};

#endif // ADMINWIDGET_H
