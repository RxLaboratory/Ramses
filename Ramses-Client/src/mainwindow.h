#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMouseEvent>
#include <QLabel>
#include <QToolButton>
#include <QMenu>
#include <QDesktopServices>
#include <QtDebug>
#include <QSettings>
#include <QProcess>
#include <QMessageBox>
#include <QSystemTrayIcon>

#include "projectpage.h"
#include "ui_mainwindow.h"

#include "duqf-widgets/settingswidget.h"
#include "duqf-widgets/aboutdialog.h"
#include "duqf-utils/utils.h"
#include "duqf-utils/duqflogger.h"
#include "duqf-widgets/duqfdocktitle.h"
#include "duqf-widgets/duqfautosizetoolbutton.h"
#include "progresspage.h"
#include "ramproject.h"
#include "databaseeditwidget.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QStringList args, QWidget *parent = nullptr);

    void setPropertiesDockWidget(QWidget *w, QString title = "Properties", QString icon = ":/icons/asset");

public slots:
    void hidePropertiesDock();

private:
    void connectEvents();
    void connectShortCuts();

    // ========= RxOT UI ==============
    /**
     * @brief duqf_checkUpdate Called once to check if an update is available
     */
    void duqf_checkUpdate();
    /**
     * @brief initUi Called once to build the default RxOT UI
     */
    void duqf_initUi();
    /**
     * @brief duqf_setStyle Called once to set the UI Style after all ui have been created
     */
    void duqf_setStyle();
    /**
     * @brief Is the tool bar currently clicked or not
     */
    bool duqf_toolBarClicked;
    /**
     * @brief Drag position
     * Used for drag n drop feature
     */
    QPoint duqf_dragPosition;
    QToolButton *duqf_settingsButton;
    AboutDialog *duqf_aboutDialog;
    QAction *duqf_actionShowHide;
    QProgressBar *duqf_fundingBar = nullptr;
    QSystemTrayIcon *trayIcon;
    SettingsWidget *settingsWidget;
    QLabel *title;
    QMenu *helpMenu;
    ProgressPage *progressPage;
    QDockWidget *ui_statsDockWidget;
    QDockWidget *ui_consoleDockWidget;
    QDockWidget *ui_propertiesDockWidget;
    QDockWidget *ui_timelineDockWidget;
    DuQFDockTitle *ui_statsTitle;
    DuQFDockTitle *ui_propertiesTitle;

    SettingsWidget *ui_adminPage;
    ProjectPage *ui_projectSettingsPage;

    DatabaseEditWidget *ui_databaseEditWidget = nullptr;

    QMenu *ui_userMenu;
    QMenu *ui_databaseMenu;
    QMenu *ui_refreshMenu;
    DuQFAutoSizeToolButton *ui_userButton;
    DuQFAutoSizeToolButton *ui_networkButton;
    QToolButton *ui_refreshButton;
    QToolButton *ui_consoleButton;
    int m_currentPageIndex = 0;

    QMetaObject::Connection _currentUserConnection;

    bool m_shiftPressed = false;

    /**
     * @brief m_showUpdateAlerts is set to true after the auto-update check during startup
     */
    bool m_showUpdateAlerts = false;

private slots:
    void duqf_bugReport();
    void duqf_forum();
    void duqf_chat();
    void duqf_doc();
    void duqf_donate();
    void duqf_settings(bool checked = true);
    void duqf_closeSettings();
    void duqf_reinitSettings();
    void duqf_about();
    void duqf_trayClicked(QSystemTrayIcon::ActivationReason reason);
    void duqf_showHide();
    void duqf_askBeforeClose();
    void duqf_updateAvailable(QJsonObject updateInfo);

    void log(DuQFLog m);

    void pageChanged(int i);
    void serverSettings();
    void loginAction();
    void logoutAction();
    void setOfflineAction();
    void setOnlineAction();
    void databaseSettingsAction();
    void home();
    void userProfile();
    void revealUserFolder();
    void admin(bool show = true);
    void projectSettings(bool show = true);
    void pipeline(bool show = true);
    void shots(bool show = true);
    void assets(bool show = true);
    void schedule(bool show = true);
    void install(bool show = true);
    void currentUserChanged();
    void currentProjectChanged(RamProject *project);
    void freezeUI(bool f = true);
    void dbiConnectionStatusChanged(NetworkUtils::NetworkStatus s);

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *key) override;
    void keyReleaseEvent(QKeyEvent *key) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
};
#endif // MAINWINDOW_H
