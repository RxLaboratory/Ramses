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
#include "idletimer.h"
#include "dbinterface.h"
#include "projectselectorwidget.h"
#include "ramstatus.h"
#include "ramstage.h"
#include "ramproject.h"
#include "ramshot.h"
#include "addshotsdialog.h"
#include "ramstagestatus.h"
#include "shotstatuswidget.h"
#include "ramasset.h"
#include "assetstatuswidget.h"
#include "helpdialog.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
private:
    //general methods
    void updateCSS();
    void login(); // store log in infos (username and hashed password)
    void logout();
    void connection(); //DBI connection (both local and remote)
    void showPage(int page = 0);
    //admin - status
    void getStatuses();
    QList<RAMStatus*> statusesList;
    //admin - stages
    void getStages();
    QList<RAMStage*> stagesList;
    //admin - projects
    void getProjects();
    QList<RAMProject*> projectsList;
    //admin - shots
    void getShots();
    QList<RAMShot*> shotsList;
    //login
    QString username;
    QString passHash;
    RAMProject *currentProject;
    RAMStage *currentStage;
    QList<RAMStage *> currentStages;
    QList<RAMShot *> currentShots;
    //used to drag window grabing the toolbar
    QPoint dragPosition;
    bool toolBarClicked;
    //settings
    QSqlDatabase settingsDB;
    //database interface
    DBInterface *dbi;
    void setWaiting(bool w = true);
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
    //help panel
    HelpDialog *helpDialog;
    bool helpDialogDocked;
    //desktop
    QDesktopWidget *desktop;
public slots:
    // IDLE
    void idle();
private slots:
    // =======DEV AND DEBUG
    void on_updateCSSButton_clicked();
    // =======BUTTONS
    //login
    void on_loginButton_clicked();
    void on_usernameEdit_returnPressed();
    void on_passwordEdit_returnPressed();
    //selectors
    void selectorProjectChanged(int i);
    //settings
    void on_serverSettingsButton_clicked();
    void on_serverAddressEdit_editingFinished();
    void on_sslCheckBox_clicked(bool checked);
    void on_updateFreqSpinBox_editingFinished();
    void on_timeOutEdit_editingFinished();
    //admin
    void on_adminWidget_currentChanged(int index);
    //admin - status
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
    void gotShots(bool success, QString message, QJsonValue shots);
    void on_shotsAdminList_itemClicked(QListWidgetItem *item);
    void shotUpdated(bool success,QString message);
    void shotStatusUpdated(bool success,QString message);
    void on_shotApplyButton_clicked();
    void on_removeShotButton_clicked();
    void shotRemoved(bool,QString);
    void updateShotStatus(RAMStatus*status, RAMStage*stage, RAMShot*shot);
    void shotsAdminReset();
    //admin - assets
    void assetAdded(bool success,QString message);
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
    void connecting();
    void connected(bool available, QString err);
    void on_addStatusButton_clicked();
    void stopWaiting();
    void showMessage(QString m, int i = 0);

protected:
    //events
    bool eventFilter(QObject *obj, QEvent *event);
    bool event(QEvent *event);
};

#endif // MAINWINDOW_H
