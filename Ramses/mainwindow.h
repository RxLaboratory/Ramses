#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QFile>
#include <QStyleFactory>
#include <QEvent>
#include <QMouseEvent>
#include <QComboBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QColorDialog>
#include <QProgressBar>
#include <QJsonArray>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QXmlStreamReader>
#include "idletimer.h"
#include "dbinterface.h"
#include "updater.h"
#include "projectselectorwidget.h"
#include "ramstatus.h"
#include "ramstage.h"
#include "ramproject.h"
#include "addshotsdialog.h"
#include "ramstagestatus.h"
#include "shotstatuswidget.h"
#include "ramasset.h"
#include "assetstatuswidget.h"
#include "helpdialog.h"
#include "xmlreader.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent    Parent widget
     */
    explicit MainWindow(QWidget *parent = 0);
private:

    // ==================================================
    //                       GENERAL
    // ==================================================

    /**
     * @brief  Connects required signals and slots
     * Executed on construction only
     */
    void mapEvents();

    /**
     * @brief Updates the current stylesheet for the application
     * (Just reloads it)
     */
    void updateCSS();

    /**
     * @brief Hashes the password and stores credentials in username and passHash
     * Then initiates the connection to the DBI
     * @param user  The username
     * @param password  The password (clear text)
     */
    void login();

    /**
     * @brief Empties all lists
     * Then displays the login page
     */
    void logout();

    /**
     * @brief Changes the page displayed on the main widget
     * Changes the page in the help window too
     * 0- Login
     * 1- Main
     * 2- Stages
     * 3- Statistics
     * 4- Admin
     * 5- Settings
     * @param page  The index of the page to display
     */
    void showPage(int page = 0);

    /**
     * @brief Sets/unsets the window in "waiting" mode
     * Disables some widgets to prevent user interaction
     * @param w true to enable waiting mode, false to disable
     */
    void setWaiting(bool w = true);

    // IO
    // TODO
    void importEDL(QString f);
    void importXML(QString f);

    // ==================================================
    //                      ADMIN
    // ==================================================

    // STATUSES

    void getStatuses();
    QList<RAMStatus*> statusesList;

    // STAGES

    void getStages();
    QList<RAMStage*> stagesList;

    // PROJECTS

    void getProjects();
    QList<RAMProject*> projectsList;
    //admin - shots
    //void getShots();
    //QList<RAMShot*> shotsList;
    QList<RAMShot*> *allShots;
    //admin - assets
    QList<RAMAsset*> assetsList;
    //login
    QString username;
    QString passHash;
    RAMProject *currentProject;
    RAMStage *currentStage;
    QList<RAMStage *> currentStages;
    //QList<RAMShot *> currentShots;
    //used to drag window grabing the toolbar
    QPoint dragPosition;
    bool toolBarClicked;
    //settings
    QSqlDatabase settingsDB;
    //database interface
    DBInterface *dbi;
    //updater
    Updater *updater;

    //resources
    QString resourcesFolder;
    //statusbar
    QProgressBar *mainStatusProgress;
    QPushButton *mainStatusStopButton;
    //selectors
    QComboBox *projectSelector;
    QComboBox *stageSelector;
    bool freezeSelectors;
    //window buttons
    QPushButton *maximizeButton;
    QPushButton *minimizeButton;
    QPushButton *quitButton;
    //help panel
    HelpDialog *helpDialog;
    bool helpDialogDocked;
    //desktop
    QDesktopWidget *desktop;

signals:
    void assetsListUpdated(QList<RAMAsset *> a);

public slots:
    /**
     * @brief Logs out when the application is idle
     */
    void idle();

private slots:

    // =======DEV AND DEBUG
    void on_updateCSSButton_clicked();

    // =======BUTTONS

    //login page
    void on_loginButton_clicked();
    void on_serverSettingsButton_clicked();
    void on_usernameEdit_returnPressed();
    void on_passwordEdit_returnPressed();

    //selectors
    void selectorProjectChanged(int i);
    //settings
    void on_serverAddressEdit_editingFinished();
    void on_sslCheckBox_clicked(bool checked);
    void on_updateFreqSpinBox_editingFinished();
    void on_timeOutEdit_editingFinished();
    //admin
    void on_adminWidget_currentChanged(int index);
    //admin - status
    void on_addStatusButton_clicked();
    void on_statusColorButton_clicked();
    void statusAdded(bool success,QString message);
    void gotStatuses(bool success, QString message, QJsonValue statuses);
    void on_statusAdminList_itemClicked(QListWidgetItem *i);
    void statusUpdated(bool success,QString message);
    void on_statusApplyButton_clicked();
    void on_removeStatusButton_clicked();
    void statusRemoved(bool success,QString message);
    void statusesAdminReset();
    //admin - stages
    void on_addStageButton_clicked();
    void stageAdded(bool success,QString message);
    void gotStages(bool success, QString message, QJsonValue statuses);
    void on_stagesAdminList_itemClicked(QListWidgetItem *item);
    void stageUpdated(bool success,QString message);
    void on_stageApplyButton_clicked();
    void on_removeStageButton_clicked();
    void stageRemoved(bool,QString);
    void stagesAdminReset();
    //admin - projects
    void on_addProjectButton_clicked();
    void projectAdded(bool success,QString message);
    void gotProjects(bool success, QString message, QJsonValue projects);
    void on_projectAdminList_itemClicked(QListWidgetItem *item);
    void projectUpdated(bool success,QString message);
    void on_projectApplyButton_clicked();
    void on_removeProjectButton_clicked();
    void projectRemoved(bool,QString);
    void on_projectAddStageButton_clicked();
    void projectStageAdded(bool success,QString message);
    void on_removeStageProjectButton_clicked();
    void projectStageRemoved(bool success,QString message);
    void projectsAdminReset();
    //admin - shots
    void on_addShotButton_clicked();
    void on_batchAddShotButton_clicked();
    void shotAdded(bool success,QString message);
    void shotAdded(RAMShot *shot);
    //void gotShots(bool success, QString message, QJsonValue shots);
    void on_shotsAdminList_itemClicked(QListWidgetItem *item);
    void shotUpdated(bool success,QString message);
    void stageStatusUpdated(bool success,QString message);
    void stageCommentUpdated(bool success,QString message);
    void on_shotApplyButton_clicked();
    void on_removeShotButton_clicked();
    void shotRemoved(bool success ,QString message);
    void updateStageStatus(RAMStatus*status, RAMStage*stage, RAMShot*shot);
    void shotsAdminReset();
    void shotsMoved(bool success, QString message);
    void shotStatusAdded(RAMStageStatus *st,RAMShot *sh);
    void on_moveShotUpButton_clicked();
    void on_moveShotDownButton_clicked();
    void on_importShotsButton_clicked();
    //admin - assets
    void assetAdded(bool success,QString message);
    void updateAssetStatus(RAMAsset *asset);
    void assetStatusUpdated(bool success,QString message);
    void assetAssigned(bool success,QString message);
    void assetAssigned(RAMAsset *a,RAMShot *s);
    void loadAsset(RAMAsset *a);

    // =======ACTIONS
    void on_actionMain_triggered(bool checked);
    void on_actionStage_triggered(bool checked);
    void on_actionStats_triggered(bool checked);
    void on_actionAdmin_triggered(bool checked);
    void on_actionSettings_triggered(bool checked);
    void on_actionLogout_triggered(bool checked);
    void on_actionHelp_triggered(bool checked);
    // =======WINDOW BUTTONS
    void maximizeButton_clicked();
    void on_settingsLogoutButton_clicked();
    void dockHelpDialog(bool dock);

    // =======DBI
    /**
     * @brief Called when the DBI is waiting for connexion
     */
    void connecting();
    /**
     * @brief Called when the connexion is established or failed
     * @param available True if the connexion is successful
     * @param err   The error to be displayed if the connexion failed
     */
    void connected(bool available, QString err);

    // ==================================================
    //                      GENERAL
    // ==================================================

    /**
     * @brief slot which calls setWaiting(false)
     * Disables the waiting mode
     */
    void stopWaiting();
    /**
     * @brief Shows a message in the status bar and help window
     * @param m The message
     * @param i Timeout for the status bar
     */
    void showMessage(QString m, int i = 0);



protected:
    //events
    bool eventFilter(QObject *obj, QEvent *event);
    bool event(QEvent *event);
};

#endif // MAINWINDOW_H
