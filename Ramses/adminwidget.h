#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include "ui_adminwidget.h"
#include "dbinterface.h"
#include "updater.h"
#include "addshotsdialog.h"
#include <QColorDialog>
#include <QErrorMessage>
#include <QFileDialog>
#include <QXmlStreamReader>
#include "xmlreader.h"
#include "importerdialog.h"

class AdminWidget : public QWidget, private Ui::AdminWidget
{
    Q_OBJECT

public:
    explicit AdminWidget(DBInterface *db,Updater *up,QWidget *parent = 0);
    /**
     * @brief initializes the admin widget with all lists and items
     * @param status    The status list
     */
    void init();

signals:
    void statusCreated(RAMStatus*);
    void stageCreated(RAMStage*);
    void projectCreated(RAMProject*);
    void shotCreated(RAMShot*,int row);
    void message(QString,QString);
    void working(bool);

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



    void on_createAssetsButton_clicked();

private:
    // ----------------- GENERAL ------------------------
    QErrorMessage *error;
    /**
     * @brief The interface to the local and remote Databases
     */
    DBInterface *dbi;
    Updater *updater;

    // ----------------- STATUSES -----------------------

    /**
     * @brief Creates the UI item for a status
     * @param rs    the status
     */
    void newStatus(RAMStatus *rs);
    /**
     * @brief Resets the admin panel of the statuses
     */
    void statusesAdminReset();


    // ----------------- STAGES -----------------------

    /**
     * @brief Creates a new stage and adds it to the list
     * @param rs the stage
     */
    void newStage(RAMStage *rs);
    /**
     * @brief Resets the admin panel of the stages
     */
    void stagesAdminReset();

    // ----------------- PROJECTS -----------------------

    /**
     * @brief Creates a new project and adds it to the list
     * @param rp the stage
     */
    void newProject(RAMProject *rp);
    /**
     * @brief Resets the admin panel for the projects
     */
    void projectsAdminReset();
    /**
     * @brief The project currently selected
     */

    // ----------------- SHOTS -----------------------

    /**
     * @brief Creates a new shot and adds it to the list
     * @param rs the shot
     */
    void newShot(RAMShot *rs, int row);
    /**
     * @brief Gets the row to create a new shot
     * @returns The row
     */
    int getNewShotRow();
    /**
     * @brief Generates a new Id for a shot
     * @return The id
     */
    int generateShotId();
    /**
     * @brief Resets the order of the shots in the DB
     */
    void resetShotsOrder();
    /**
     * @brief Resets the admin panel for the shots
     */
    void shotsAdminReset();
    // IO
    /**
     * @brief Imports an EDL file
     * @param f The path to the file
     */
    void importEDL(QString f);
    /**
     * @brief Imports a Final Cut XML file
     * @param f The path to the file
     */
    void importXML(QString f);
};

#endif // ADMINWIDGET_H
