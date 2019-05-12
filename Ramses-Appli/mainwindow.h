#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QFile>
#include <QStyleFactory>
#include <QEvent>
#include <QMouseEvent>
#include <QComboBox>
#include <QCryptographicHash>
#include <QColorDialog>
#include <QProgressBar>
#include <QDesktopWidget>
#include "idletimer.h"
#include "dbinterface.h"
#include "projectselectorwidget.h"
#include "addshotsdialog.h"
#include "shotassetswidget.h"
#include "helpdialog.h"
#include "adminwidget.h"
#include "maintable.h"
#include "updater.h"
#include "settingswidget.h"
#include "ramuser.h"

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

    // ----------------- GENERAL -----------------------

    /**
     * @brief  Connects required signals and slots
     * Executed on construction only
     */
    void mapEvents();

    /**
     * @brief Hashes the password and stores credentials in username and passHash
     * Then initiates the connection to the DBI
     * @param user  The username
     * @param password  The password (clear text)
     */
    void login();

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
     * @brief The object which handles data updates from remote server
     */
    Updater *updater;

    // ----------------- WIDGETS -----------------------

    AdminWidget *adminWidget;
    MainTable *mainTable;
    SettingsWidget *settingsWidget;

    //login
    RAMUser *currentUser;

    //used to drag window grabing the toolbar
    QPoint dragPosition;
    bool toolBarClicked;

    //database interface
    DBInterface *dbi;

    //pageShown
    int currentPage;

    //resources
    QString resourcesFolder;
    //statusbar
    QProgressBar *mainStatusProgress;
    //selectors
    ProjectSelectorWidget *projectSelector;
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

    // =======BUTTONS

    //login page
    void on_loginButton_clicked();
    void on_serverSettingsButton_clicked();
    void on_usernameEdit_returnPressed();
    void on_passwordEdit_returnPressed();

    // =======ACTIONS
    void on_actionRefresh_triggered();
    void on_actionMain_triggered();
    void on_actionStage_triggered();
    void on_actionStats_triggered();
    void on_actionAdmin_triggered();
    void on_actionSettings_triggered();
    void on_actionLogout_triggered(bool checked);
    void on_actionHelp_triggered(bool checked);
    // =======WINDOW BUTTONS
    void maximizeButton_clicked();
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
     * @brief Sets/unsets the window in "waiting" mode
     * Disables some widgets to prevent user interaction
     * @param w true to enable waiting mode, false to disable
     */
    void setWaiting(bool w = true);
    void showProgress();
    /**
     * @brief slot which calls setWaiting(false)
     * Disables the waiting mode
     */
    void stopWaiting();
    /**
     * @brief Shows a message in the status bar and help window
     * @param m The message
     * @param type Type of message, one of: "remote", "local", "general", "connexion", "critical", "warning","debug"
     */
    void showMessage(QString m, QString type = "general");
    /**
     * @brief cleans everything
     * Remove UI items and deletes data
     */
    void clean();
    /**
     * @brief Empties all lists
     * Then displays the login page
     */
    void logout();
    /**
     * @brief quits the application
     */
    void quit();

    void setToolBarStyle(int s);

protected:
    //events
    bool eventFilter(QObject *obj, QEvent *event);
    bool event(QEvent *event);
};

#endif // MAINWINDOW_H
