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
#include "projectselectorwidget.h"
#include "ramstatus.h"
#include "ramstage.h"
#include "ramproject.h"
#include "addshotsdialog.h"
#include "ramasset.h"
#include "assetstatuswidget.h"
#include "helpdialog.h"
#include "xmlreader.h"
#include "adminwidget.h"

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

    /**
     * @brief Keeps a list of pointers to delete when refreshing the data
     */
    QList<QObject*> removedItems;

    // ==================================================
    //                      ADMIN
    // ==================================================

    AdminWidget *adminWidget;

    // ----------------- STATUS -----------------------
    /**
     * @brief the current list of statuses
     */
    QList<RAMStatus*> statusList;
    /**
     * @brief Gets a status using its Id
     * @param id    The status id
     * @return The status
     */
    RAMStatus *getStatus(int id);
    /**
     * @brief Called when the remote server has sent the list of statuses
     * @param statuses  The list
     */
    void gotStatuses(QJsonValue statuses);

    // ----------------- STAGES -----------------------
    /**
     * @brief Called when the remote server has sent the list of stages
     * @param stages    The list
     */
    void gotStages(QJsonValue stages);
    /**
     * @brief The current list of stages
     */
    QList<RAMStage*> stagesList;
    /**
     * @brief Gets a stage using its Id
     * @param id    The stage id
     * @return The stage
     */
    RAMStage* getStage(int id);

    // ----------------- PROJECTS -----------------------

    /**
     * @brief Called when the remote server has sent the list of projects
     * @param projects    The list
     */
    void gotProjects(QJsonValue projects);
    /**
     * @brief The current list of projects
     */
    QList<RAMProject*> projectsList;
    /**
     * @brief Gets a project using its Id
     * @param id    The project id
     * @return The project
     */
    RAMProject* getProject(int id);

    // ----------------- SHOTS -----------------------

    /**
     * @brief Called when the remote server has sent the list of shots
     * @param shots    The list
     */
    void gotShots(QJsonValue shots);
    /**
     * @brief The current list of shots
     */
    QList<RAMShot*> allShots;
    /**
     * @brief Gets a shot using its Id
     * @param id    The shot id
     * @return The shot
     */
    RAMShot *getShot(int id);
    /**
     * @brief Resets the order of the shots in the DB
     */
    void resetShotsOrder();
    /**
     * @brief Sorter method to sort the item selection in shotsAdminList
     * @param a
     * @param b
     * @return
     */
    static bool shotsAdminSelectionSort(QListWidgetItem *a,QListWidgetItem *b);

    // ----------------- ASSETS -----------------------

    /**
     * @brief Called when the remote server has sent the list of assets
     * @param assets    The list
     */
    void gotAssets(QJsonValue assets);
    void newAsset(RAMAsset *asset);
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

    // IO
    // TODO
    void importEDL(QString f);
    void importXML(QString f);

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

    //status
    void newStatus(RAMStatus *rs);
    void removeStatus(RAMStatus *rs);
    //stages
    void newStage(RAMStage *rs);
    void removeStage(RAMStage *rs);
    //projects
    void newProject(RAMProject *rp);
    void removeProject(RAMProject *rp);
    //shots
    void newShot(RAMShot *rs, int row);
    void removeShot(RAMShot* rs);

    //admin - assets
    void assetCreated(RAMAsset *asset);
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
    /**
     * @brief Called when data is received from the remote server of the DBI
     * @param data  The Json data received
     */
    void dataReceived(QJsonObject data);


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
