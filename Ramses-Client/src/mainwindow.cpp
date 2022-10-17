#include "mainwindow.h"

#include "itemmanagerwidget.h"
#include "progressmanager.h"
#include "progressbar.h"
#include "docks/consolewidget.h"
#include "daemonsettingswidget.h"
#include "loginpage.h"
#include "userprofilepage.h"
#include "usermanagerwidget.h"
#include "projectmanagerwidget.h"
#include "pipeline-editor/pipelinewidget.h"
#include "templatestepmanagerwidget.h"
#include "templateassetgroupmanagerwidget.h"
#include "statemanagerwidget.h"
#include "filetypemanagerwidget.h"
#include "applicationmanagerwidget.h"
#include "schedulemanagerwidget.h"
#include "docks/statisticswidget.h"
#include "docks/timelinewidget.h"
#include "dbinterface.h"
#include "daemon.h"
#include "projectselectorwidget.h"
#include "dbmanagerwidget.h"
#include "duqf-widgets/duqftoolbarspacer.h"
#include "duqf-widgets/duqflogtoolbutton.h"
#include "duqf-widgets/duqfupdatedialog.h"
#include "duqf-widgets/duqfupdatesettingswidget.h"
#include "duqf-widgets/appearancesettingswidget.h"
#include "duqf-app/app-version.h"
#include "duqf-app/app-style.h"
#include "duqf-app/app-utils.h"
#include "duqf-widgets/duqftoolbarspacer.h"

MainWindow::MainWindow(QStringList /*args*/, QWidget *parent) :
    QMainWindow(parent)
{
    // Build the form
    setupUi(this);

    qDebug() << "> Initiating threads and workers";

    // We instantiate all these objects to be sure it's done in the right order.

    // The database interface
    DBInterface::instance();
    // The Process manager
    ProgressManager::instance();
    // Ramses
    Ramses::instance();

    qDebug() << "> Loading settings";

    QSettings settings;

    qDebug() << "> Setting up menus";

    // Populate Toolbar
    actionAdmin->setVisible(false);
    actionProjectSettings->setVisible(false);
    actionPipeline->setVisible(false);
    actionShots->setVisible(false);
    actionAssets->setVisible(false);
    actionSchedule->setVisible(false);
    actionStatistics->setVisible(false);
    actionTimeline->setVisible(false);

    mainToolBar->addWidget(new DuQFToolBarSpacer(this));

    mainToolBar->addWidget(new ProjectSelectorWidget(this));

    //Populate status bar

    mainStatusBar->addPermanentWidget(new ProgressBar(this));

    ui_refreshMenu = new QMenu();
    ui_refreshMenu->addAction(actionSync);
    ui_refreshMenu->addAction(actionFullSync);

    ui_refreshButton = new QToolButton(this);
    ui_refreshButton->setObjectName("menuButton");
    ui_refreshButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui_refreshButton->setText("");
    ui_refreshButton->setIcon(QIcon(":/icons/reload"));
    ui_refreshButton->setMenu(ui_refreshMenu);
    ui_refreshButton->setPopupMode(QToolButton::InstantPopup);
    mainStatusBar->addPermanentWidget(ui_refreshButton);
    ui_refreshButton->hide();

    ui_consoleButton = new QToolButton(this);
    ui_consoleButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui_consoleButton->setText("");
    ui_consoleButton->setIcon(QIcon(":/icons/bash"));
    ui_consoleButton->setCheckable(true);
    mainStatusBar->addPermanentWidget(ui_consoleButton);

    mainStatusBar->addPermanentWidget(new DuQFLogToolButton(this));

    ui_databaseMenu = new QMenu();
    ui_databaseMenu->addAction(actionSetOffline);
    ui_databaseMenu->addAction(actionSetOnline);
    ui_databaseMenu->addAction(actionDatabaseSettings);
    actionSetOffline->setVisible(false);
    ui_networkButton = new DuQFAutoSizeToolButton(this);
    ui_networkButton->setObjectName("menuButton");
    ui_networkButton->setText("Offline");
    ui_networkButton->setIcon(QIcon(":/icons/storage"));
    ui_networkButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //ui_networkButton->setMinimumWidth(100);
    ui_networkButton->setMenu(ui_databaseMenu);
    ui_networkButton->setPopupMode(QToolButton::InstantPopup);
    mainStatusBar->addPermanentWidget(ui_networkButton);
    ui_networkButton->setVisible(false);

    ui_userMenu = new QMenu();
    ui_userMenu->addAction(actionLogIn);
    ui_userMenu->addAction(actionUserFolder);
    actionUserFolder->setVisible(false);
    ui_userMenu->addAction(actionUserProfile);
    actionUserProfile->setVisible(false);
    ui_userMenu->addAction(actionLogOut);
    actionLogOut->setVisible(false);
    ui_userButton = new DuQFAutoSizeToolButton(this);
    ui_userButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui_userButton->setText("Guest");
    //ui_userButton->setMinimumWidth(75);
    ui_userButton->setMenu(ui_userMenu);
    ui_userButton->setPopupMode(QToolButton::InstantPopup);
    mainStatusBar->addPermanentWidget(ui_userButton);

    // Add default stuff
    duqf_initUi();

    qDebug() << "> Loading setting pages";

    // Add settings
    DaemonSettingsWidget *dsw = new DaemonSettingsWidget(this);
    settingsWidget->addPage(dsw, "Daemon", QIcon(":/icons/daemon"));

    qDebug() << "> Loading pages";

    // Add pages
    // login
    LoginPage *lp = new LoginPage(this);
    mainLayout->addWidget(lp);

    // user profile
    UserProfilePage *up = new UserProfilePage(this);
    mainStack->addWidget(up);

    // admin
    ui_adminPage = new SettingsWidget("Administration", this);
    ui_adminPage->titleBar()->setObjectName("adminToolBar");
    ui_adminPage->showReinitButton(false);
    mainStack->addWidget(ui_adminPage);
    // Admin tabs
    qDebug() << "> Admin";
    UserManagerWidget *userManager = new UserManagerWidget(this);
    ui_adminPage->addPage(userManager,"Users", QIcon(":/icons/users"));
    ui_adminPage->titleBar()->insertLeft(userManager->menuButton());
    qDebug() << "  > users ok";
    ProjectManagerWidget *projectManager = new ProjectManagerWidget(this);
    ui_adminPage->addPage(projectManager, "Projects", QIcon(":/icons/projects"));
    ui_adminPage->titleBar()->insertLeft(projectManager->menuButton());
    qDebug() << "  > projects ok";
    TemplateStepManagerWidget *templateStepManager = new TemplateStepManagerWidget(this);
    ui_adminPage->addPage(templateStepManager, "Template Steps", QIcon(":/icons/steps"));
    ui_adminPage->titleBar()->insertLeft(templateStepManager->menuButton());
    qDebug() << "  > template steps ok";
    TemplateAssetGroupManagerWidget *templateAssetGroupManager = new TemplateAssetGroupManagerWidget(this);
    ui_adminPage->addPage(templateAssetGroupManager, "Template Asset Groups", QIcon(":/icons/asset-groups"));
    ui_adminPage->titleBar()->insertLeft(templateAssetGroupManager->menuButton());
    qDebug() << "  > template assets ok";
    StateManagerWidget *stateManager = new StateManagerWidget(this);
    ui_adminPage->addPage(stateManager, "States", QIcon(":/icons/state"));
    ui_adminPage->titleBar()->insertLeft(stateManager->menuButton());
    qDebug() << "  > states ok";
    FileTypeManagerWidget *fileTypeManager = new FileTypeManagerWidget(this);
    ui_adminPage->addPage(fileTypeManager, "File Types", QIcon(":/icons/files"));
    ui_adminPage->titleBar()->insertLeft(fileTypeManager->menuButton());
    qDebug() << "  > file types ok";
    ApplicationManagerWidget *applicationManager = new ApplicationManagerWidget(this);
    ui_adminPage->addPage(applicationManager, "Applications", QIcon(":/icons/applications"));
    ui_adminPage->titleBar()->insertLeft(applicationManager->menuButton());
    qDebug() << "  > applications ok";
    DBManagerWidget *dbManager = new DBManagerWidget(this);
    ui_adminPage->addPage(dbManager, "Database tools", QIcon(":/icons/applications"));
    qDebug() << "  > DB Manager ok";//*/

    // Project settings
    ui_projectSettingsPage = new ProjectPage(this);
    mainStack->addWidget(ui_projectSettingsPage);

    // Pipeline editor
#ifndef DEACTIVATE_PIPELINE
    PipelineWidget *pipelineEditor = new PipelineWidget(this);
    mainStack->addWidget(pipelineEditor);
    connect(pipelineEditor, SIGNAL(closeRequested()), this, SLOT(home()));
    qDebug() << "> Pipeline ready";//*/
#endif

#ifndef DEACTIVATE_ASSETSTABLE
    ItemManagerWidget *assetsTable = new ItemManagerWidget(RamStep::AssetProduction, this);
    mainStack->addWidget(assetsTable);
    connect(assetsTable, SIGNAL(closeRequested()), this, SLOT(home()));
    qDebug() << "> Assets table ready";
#endif

#ifndef DEACTIVATE_SHOTSTABLE
    ItemManagerWidget *shotsTable = new ItemManagerWidget(RamStep::ShotProduction, this);
    mainStack->addWidget(shotsTable);
    connect(shotsTable, SIGNAL(closeRequested()), this, SLOT(home()));
    qDebug() << "> Shots table ready";
#endif

#ifndef DEACTIVATE_SCHEDULE
    ScheduleManagerWidget *scheduleTable = new ScheduleManagerWidget(this);
    mainStack->addWidget(scheduleTable);
    connect(scheduleTable,SIGNAL(closeRequested()), this, SLOT(home()));
    qDebug() << "> Schedule ready";
#endif

    // Docks
#ifndef DEACTIVATE_STATS
    StatisticsWidget *statsTable = new StatisticsWidget(this);
    ui_statsDockWidget = new QDockWidget("Statistics");
    ui_statsDockWidget->setObjectName("statsDock");
    ui_statsTitle = new DuQFDockTitle("Statistics", this);
    ui_statsTitle->setObjectName("dockTitle");
    ui_statsTitle->setIcon(":/icons/stats");
    ui_statsDockWidget->setTitleBarWidget(ui_statsTitle);
    ui_statsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    ui_statsDockWidget->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    ui_statsDockWidget->setWidget( statsTable );
    Qt::DockWidgetArea area = static_cast<Qt::DockWidgetArea>( settings.value("ui/statsArea", Qt::LeftDockWidgetArea).toInt() );
    this->addDockWidget(area, ui_statsDockWidget);
    ui_statsDockWidget->hide();

    qDebug() << "> Statistics table ready";
#endif

    // A console in a tab
    ConsoleWidget *console = new ConsoleWidget(this);
    ui_consoleDockWidget = new QDockWidget("Console");
    ui_consoleDockWidget->setObjectName("consoleDock");
    DuQFDockTitle *consoleTitle = new DuQFDockTitle("Console", this);
    consoleTitle->setObjectName("dockTitle");
    consoleTitle->setIcon(":/icons/bash");
    ui_consoleDockWidget->setTitleBarWidget(consoleTitle);
    ui_consoleDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    ui_consoleDockWidget->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    ui_consoleDockWidget->setWidget( console );
    this->addDockWidget(Qt::LeftDockWidgetArea, ui_consoleDockWidget);
    ui_consoleDockWidget->hide();

    qDebug() << "> Console dock ready";

    // The timeline
    TimelineWidget *timeline = new TimelineWidget(this);
    ui_timelineDockWidget = new QDockWidget("Timeline");
    ui_timelineDockWidget->setObjectName("timelineDock");
    DuQFDockTitle *timelineTitle = new DuQFDockTitle("Timeline", this);
    timeline->setObjectName("dockTitle");
    timelineTitle->setIcon(":/icons/timeline");
    ui_timelineDockWidget->setTitleBarWidget(timelineTitle);
    ui_timelineDockWidget->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    ui_timelineDockWidget->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    ui_timelineDockWidget->setWidget( timeline );
    this->addDockWidget(Qt::BottomDockWidgetArea, ui_timelineDockWidget);
    ui_timelineDockWidget->hide();

    qDebug() << "> Timeline dock ready";

    // The properties dock
    ui_propertiesDockWidget = new QDockWidget("Properties");
    ui_propertiesDockWidget->setObjectName("propertiesDock");
    ui_propertiesTitle = new DuQFDockTitle("Properties", this);
    ui_propertiesTitle->setObjectName("dockTitle");
    ui_propertiesTitle->setIcon(":/icons/asset");
    ui_propertiesDockWidget->setTitleBarWidget(ui_propertiesTitle);
    ui_propertiesDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    ui_propertiesDockWidget->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    this->addDockWidget(Qt::RightDockWidgetArea, ui_propertiesDockWidget);
    ui_propertiesDockWidget->hide();

    qDebug() << "> Properties dock ready";   

    // Progress page
    progressPage = new ProgressPage(this);
    mainStack->addWidget(progressPage);

    // Set UI
    mainStack->setCurrentIndex(0);

    qDebug() << "> Connecting events";

    connectEvents();
    connectShortCuts();

    // Set style

    qDebug() << "Setting CSS";

    duqf_setStyle();

    // Restore UI state
    settings.beginGroup("ui");
    if (settings.value("maximized", false).toBool() )
        this->showMaximized();
    this->restoreState( settings.value("windowState").toByteArray() );
    settings.endGroup();
    // re-hide docks
    ui_propertiesDockWidget->hide();
    ui_statsDockWidget->hide();
    ui_timelineDockWidget->hide();
}

void MainWindow::connectEvents()
{
    // Connect events
    connect(ProgressManager::instance(), &ProgressManager::freezeUI, this, &MainWindow::freezeUI);

    // Toolbar and other tools
    connect(actionLogIn,SIGNAL(triggered()), this, SLOT(loginAction()));
    connect(actionLogOut,SIGNAL(triggered()), this, SLOT(logoutAction()));
    connect(actionSettings, SIGNAL(triggered(bool)), this, SLOT(duqf_settings(bool)));
    connect(actionSetOnline, &QAction::triggered, this, &MainWindow::setOnlineAction);
    connect(actionSetOffline, &QAction::triggered, this, &MainWindow::setOfflineAction);
    connect(actionDatabaseSettings, &QAction::triggered, this, &MainWindow::databaseSettingsAction);
    connect(actionUserProfile,SIGNAL(triggered()), this, SLOT(userProfile()));
    connect(actionUserFolder,SIGNAL(triggered()), this, SLOT(revealUserFolder()));
    connect(actionAdmin,SIGNAL(triggered(bool)), this, SLOT(admin(bool)));
    connect(actionProjectSettings,SIGNAL(triggered(bool)), this, SLOT(projectSettings(bool)));
    connect(actionPipeline, SIGNAL(triggered(bool)), this, SLOT(pipeline(bool)));
    connect(actionShots,SIGNAL(triggered(bool)), this, SLOT(shots(bool)));
    connect(actionAssets,SIGNAL(triggered(bool)), this, SLOT(assets(bool)));
    connect(actionSchedule,SIGNAL(triggered(bool)), this, SLOT(schedule(bool)));

    // Docks
    connect(actionStatistics,SIGNAL(triggered(bool)), ui_statsDockWidget, SLOT(setVisible(bool)));
    connect(ui_statsDockWidget,SIGNAL(visibilityChanged(bool)), actionStatistics, SLOT(setChecked(bool)));
    connect(ui_consoleDockWidget,SIGNAL(visibilityChanged(bool)), ui_consoleButton, SLOT(setChecked(bool)));
    connect(ui_consoleButton,SIGNAL(clicked(bool)), ui_consoleDockWidget, SLOT(setVisible(bool)));
    connect(actionTimeline,SIGNAL(triggered(bool)), ui_timelineDockWidget, SLOT(setVisible(bool)));
    connect(ui_timelineDockWidget,SIGNAL(visibilityChanged(bool)), actionTimeline, SLOT(setChecked(bool)));

    // Pages
    connect(ui_adminPage, SIGNAL(closeRequested()), this, SLOT(home()));
    connect(ui_projectSettingsPage, SIGNAL(closeRequested()), this, SLOT(home()));

    // Other buttons
    connect(actionSync, SIGNAL(triggered()), DBInterface::instance(),SLOT(sync()));
    connect(actionFullSync, SIGNAL(triggered()), DBInterface::instance(),SLOT(fullSync()));
    connect(mainStack,SIGNAL(currentChanged(int)), this, SLOT(pageChanged(int)));

    // Misc
    connect(DuQFLogger::instance(), &DuQFLogger::newLog, this, &MainWindow::log);
    connect(Daemon::instance(), &Daemon::raise, this, &MainWindow::raise);
    connect(Daemon::instance(), &Daemon::raise, this, &MainWindow::show);
    connect(Ramses::instance(),&Ramses::userChanged, this, &MainWindow::currentUserChanged);
    connect(Ramses::instance(), &Ramses::currentProjectChanged, this, &MainWindow::currentProjectChanged);
    connect(DBInterface::instance(),&DBInterface::connectionStatusChanged, this, &MainWindow::dbiConnectionStatusChanged);
    connect(DBInterface::instance(), SIGNAL(synced()), this, SLOT(update()));
}

void MainWindow::connectShortCuts()
{
    QList<QKeySequence> syncSC;
    syncSC << QKeySequence("Ctrl+R");
    syncSC << QKeySequence("F5");
    actionSync->setShortcuts( syncSC );
    actionFullSync->setShortcut(QKeySequence("Ctrl+Shift+R"));
}

void MainWindow::setPropertiesDockWidget(QWidget *w, QString title, QString icon)
{
    ui_propertiesDockWidget->setWidget( w );
    ui_propertiesTitle->setTitle(title);
    ui_propertiesTitle->setIcon(icon);
    ui_propertiesDockWidget->show();
}

void MainWindow::hidePropertiesDock()
{
    ui_propertiesDockWidget->hide();
}

void MainWindow::duqf_checkUpdate()
{
    DuApplication *app = qobject_cast<DuApplication*>(qApp);
    connect(app, SIGNAL(newUpdateInfo(QJsonObject)), this, SLOT(duqf_updateAvailable(QJsonObject)));
    // Check for update
    QSettings settings;
    bool doCheckUpdate = settings.value("updates/checkUpdateAtStartup", true).toBool();
    // Just once a day
    if (doCheckUpdate)
    {
        qDebug() << "Update check...";
        QDateTime lastCheck = settings.value("updates/latestUpdateCheck").toDateTime();
        qDebug().noquote() << "Last check was on: " + lastCheck.toString("yyyy-MM-dd hh:mm:ss");
        int days = lastCheck.daysTo(QDateTime::currentDateTime());
        qDebug().noquote() << days << " days since last check.";
        if (days > 0 || !lastCheck.isValid())
        {
            app->checkUpdate();
            return;
        }
        else
        {
            qDebug() << "We'll check again tomorrow.";
            duqf_updateAvailable(app->updateInfo());
        }
    }
    m_showUpdateAlerts = true;
}

void MainWindow::duqf_initUi()
{
    // ===== SYSTRAY ======
    duqf_actionShowHide = new QAction("Hide " + QString(STR_INTERNALNAME), this);
    duqf_actionShowHide->setIcon(QIcon(":/icons/hide-dark"));
    bool useSysTray = QSystemTrayIcon::isSystemTrayAvailable() && USE_SYSTRAY;
    if (useSysTray)
    {
        QMenu *trayMenu = new QMenu(QString(STR_INTERNALNAME),this);
        QSettings settings;
#ifdef Q_OS_LINUX
        QString trayIconType = settings.value("appearance/trayIconType", "light").toString();
#else
        QString trayIconType = settings.value("appearance/trayIconType", "color").toString();
#endif
        trayIcon = new QSystemTrayIcon(QIcon(":/icons/tray-" + trayIconType),this);
        trayMenu->addAction(duqf_actionShowHide);
        QAction *actionQuit = new QAction("Quit");
        actionQuit->setIcon(QIcon(":/icons/close-dark"));
        trayMenu->addAction(actionQuit);
        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();
        connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(duqf_trayClicked(QSystemTrayIcon::ActivationReason)));
        connect(duqf_actionShowHide, &QAction::triggered, this, &MainWindow::duqf_showHide);
        connect(actionQuit, &QAction::triggered, this, &MainWindow::close);
    }

    // ===== ABOUT ========
    duqf_aboutDialog = new AboutDialog();

    // ===== TOOLBAR ======

    // remove right click on toolbar
    mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //drag window
    duqf_toolBarClicked = false;
    mainToolBar->installEventFilter(this);

    // ==== TOOLBAR BUTTONS
    mainToolBar->addWidget(new DuQFToolBarSpacer());
    title = new QLabel(STR_FILEDESCRIPTION);
    mainToolBar->addWidget(title);

    //hide
    QToolButton *hideButton = new QToolButton();
    if (useSysTray)
    {
        hideButton->setIcon(QIcon(":/icons/hide"));
        hideButton->setObjectName("windowButton");
        mainToolBar->addWidget(hideButton);
    }

    // ===== STATUSBAR ======

    // version in statusbar
    mainStatusBar->addPermanentWidget(new QLabel("v" + QString(STR_VERSION)));
    duqf_settingsButton = new QToolButton();
    duqf_settingsButton->setIcon(QIcon(":/icons/settings-w"));
    duqf_settingsButton->setToolTip("Go to Settings");
    duqf_settingsButton->setCheckable(true);
    mainStatusBar->addPermanentWidget(duqf_settingsButton);
    QToolButton *helpButton = new QToolButton();
    helpButton->setIcon(QIcon(":/icons/help"));
    helpButton->setToolTip("Get Help");
    helpButton->setPopupMode( QToolButton::InstantPopup );
    helpMenu = new QMenu(this);

    helpButton->setMenu(helpMenu);
    mainStatusBar->addPermanentWidget(helpButton);

    if (QString(URL_DOC) != "")
    {
        QAction *docAction = new QAction(QIcon(":/icons/documentation"), "Help");
        docAction->setToolTip("Read the documentation");
        docAction->setShortcut(QKeySequence("F1"));
        helpMenu->addAction(docAction);
        connect(docAction, SIGNAL(triggered()), this, SLOT(duqf_doc()));
    }
    QAction *aboutAction = new QAction(QIcon(":/icons/info"), "About");
    helpMenu->addAction(aboutAction);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(duqf_about()));
    helpMenu->addSeparator();
    bool chat = QString(URL_CHAT) != "";
    bool bugReport = QString(URL_BUGREPORT) != "";
    bool forum = QString(URL_FORUM) != "";
    bool donate = QString(URL_DONATION) != "";
    if (bugReport)
    {
        QAction *bugReportAction = new QAction(QIcon(":/icons/bug-report"), "Bug Report");
        bugReportAction->setToolTip("Report a bug");
        helpMenu->addAction(bugReportAction);
        if (!chat && !forum && !donate) helpMenu->addSeparator();
        connect(bugReportAction, SIGNAL(triggered()), this, SLOT(duqf_bugReport()));
    }
    if (chat)
    {
        QAction *chatAction = new QAction(QIcon(":/icons/chat"), "Chat");
        chatAction->setToolTip("Come and have a chat");
        helpMenu->addAction(chatAction);
        if (!forum && !donate) helpMenu->addSeparator();
        connect(chatAction, SIGNAL(triggered()), this, SLOT(duqf_chat()));
    }
    if (forum)
    {
        QAction *forumAction = new QAction(QIcon(":/icons/forum"), "Forum");
        forumAction->setToolTip("Join us on our forum");
        helpMenu->addAction(forumAction);
        if (!donate) helpMenu->addSeparator();
        connect(forumAction, SIGNAL(triggered()), this, SLOT(duqf_forum()));
    }
    if (donate)
    {
        QAction *donateAction = new QAction(QIcon(":/icons/donate"), "I ♥ " + QString(STR_FILEDESCRIPTION));
        donateAction->setToolTip("Help us, donate now!");
        helpMenu->addAction(donateAction);
        helpMenu->addSeparator();
        connect(donateAction, SIGNAL(triggered()), this, SLOT(duqf_donate()));

        /*QToolButton *donateButton = new QToolButton();
        donateButton->setIcon(QIcon(":/icons/donate"));
        donateButton->setToolTip("I ♥ " + QString(STR_FILEDESCRIPTION));
        mainStatusBar->addPermanentWidget(donateButton);
        connect(donateButton, SIGNAL(clicked()), this, SLOT(duqf_donate()));*/
    }
    QAction *aboutQtAction = new QAction(QIcon(":/icons/qt"), "About Qt");
    helpMenu->addAction(aboutQtAction);

    // Check for update
    duqf_checkUpdate();

    // ========= SETTINGS ========

    settingsWidget = new SettingsWidget();
    settingsWidget->titleBar()->setObjectName("settingsToolBar");
    duqf_settingsLayout->addWidget(settingsWidget);

    AppearanceSettingsWidget *asw = new AppearanceSettingsWidget();
    settingsWidget->addPage(asw, "Appearance", QIcon(":/icons/color"));

    DuQFUpdateSettingsWidget *usw = new DuQFUpdateSettingsWidget();
    settingsWidget->addPage(usw, "Updates", QIcon(":/icons/update-settings"));

    // ====== CONNECTIONS ======
    if (useSysTray)
    {
        connect(hideButton, SIGNAL(clicked()), this, SLOT(duqf_showHide()));
    }

    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(duqf_settingsButton, SIGNAL(clicked(bool)), this, SLOT(duqf_settings(bool)));
    connect(settingsWidget, SIGNAL(closeRequested()), this, SLOT(duqf_closeSettings()));
    connect(settingsWidget, SIGNAL(reinitRequested()), this, SLOT(duqf_reinitSettings()));
}

void MainWindow::duqf_setStyle()
{
    // ======== STYLE ========

    //Re-set StyleSheet
    QSettings settings;
    QString cssFile = settings.value("appearance/cssFile", ":/styles/default").toString();
    QString style = settings.value("appearance/style","Default").toString();
    if (cssFile != "")
    {
        DuUI::updateCSS(cssFile);
    }
    else
    {
        DuUI::updateCSS("");
        qApp->setStyle(QStyleFactory::create(style));
    }
    //and font
    DuUI::setFont(settings.value("appearance/font", "Ubuntu").toString());
    //and tool buttons
    int styleIndex = settings.value("appearance/toolButtonStyle", 2).toInt();
    DuUI::setToolButtonStyle(styleIndex);
}

void MainWindow::duqf_bugReport()
{
    QDesktopServices::openUrl ( QUrl( URL_BUGREPORT ) );
}

void MainWindow::duqf_forum()
{
    QDesktopServices::openUrl ( QUrl( URL_FORUM ) );
}

void MainWindow::duqf_chat()
{
    QDesktopServices::openUrl ( QUrl( URL_CHAT ) );
}

void MainWindow::duqf_doc()
{
    QDesktopServices::openUrl ( QUrl( URL_DOC ) );
}

void MainWindow::duqf_donate()
{
    QDesktopServices::openUrl ( QUrl( URL_DONATION ) );
}

void MainWindow::duqf_settings(bool checked)
{
    duqf_settingsButton->setChecked(checked);
    if (checked)
    {
        mainStack->setCurrentIndex(1);
    }
    else
    {
        mainStack->setCurrentIndex(0);
    }
}

void MainWindow::duqf_closeSettings()
{
    duqf_settings(false);
}

void MainWindow::duqf_reinitSettings()
{
    QMessageBox::StandardButton choice = QMessageBox::question(this, "Reset settings", "This will reset all settings to their default values and restart the application.\nAre you sure you want to continue?" );
    if (choice == QMessageBox::Yes)
    {
        QSettings settings;
        settings.clear();
        settings.sync();
        this->close();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
}

void MainWindow::duqf_about()
{
    duqf_aboutDialog->show();
}

void MainWindow::duqf_trayClicked(QSystemTrayIcon::ActivationReason reason)
{

    if (reason == QSystemTrayIcon::Trigger)
    {
        duqf_showHide();
    }
}

void MainWindow::duqf_showHide()
{
    if (this->isVisible())
    {
        this->hide();
        duqf_actionShowHide->setIcon(QIcon(":/icons/show-dark"));
        duqf_actionShowHide->setText("Show " + QString(STR_INTERNALNAME));
    }
    else
    {
        this->show();
        duqf_actionShowHide->setIcon(QIcon(":/icons/hide-dark"));
        duqf_actionShowHide->setText("Hide " + QString(STR_INTERNALNAME));
    }
}

void MainWindow::duqf_askBeforeClose()
{
    QMessageBox::StandardButton r = QMessageBox::question(this, "Quitting Ramses", "Are you sure you want to quit Ramses?");
    if (r == QMessageBox::Yes) this->close();
}

void MainWindow::duqf_updateAvailable(QJsonObject updateInfo)
{
    // Check funding
    bool donate = QString(URL_DONATION) != "";
    if (donate)
    {
        double month = updateInfo.value("monthlyFund").toDouble(0.0);
        double goal = updateInfo.value("fundingGoal").toDouble(4000);
        if (goal > 0) {
            double ratio = month / goal * 100;
            if (!duqf_fundingBar)
            {
                duqf_fundingBar = new QProgressBar(this);
                duqf_fundingBar->setObjectName("fundingBar");
                duqf_fundingBar->setMaximumWidth(75);
                duqf_fundingBar->setFormat("♥ Donate");
                duqf_fundingBar->installEventFilter(this);
                mainStatusBar->addPermanentWidget(duqf_fundingBar);
            }
            duqf_fundingBar->setMaximum(goal);
            duqf_fundingBar->setValue(month);
            duqf_fundingBar->setToolTip( tr("This month, we've collected $%1.\n"
                                            "That's %2 % of our monthly goal.\n"
                                            "Thanks for your support!\n\n"
                                            "Click to Donate now!\nor go to: %3").arg(
                                             QString::number(month),
                                             QString::number(ratio, 'f', 0),
                                             QString(URL_DONATION)
                                             ));
        }
    }

    if (!updateInfo.value("update").toBool() && !m_showUpdateAlerts)
    {
        m_showUpdateAlerts = true;
        return;
    }

    QSettings settings;
    QDate latestUpdateCheck = settings.value("updates/latestUpdateCheck", QDate(1970,1,1)).toDate();
    if (latestUpdateCheck == QDate::currentDate()) return;

    DuQFUpdateDialog *dialog = new DuQFUpdateDialog(updateInfo, this);
    dialog->show();
}

void MainWindow::log(DuQFLog m)
{
    QString message = m.message();
    if (message == "") return;

    DuQFLog::LogType type = m.type();

    if (type == DuQFLog::Information) mainStatusBar->showMessage(message,5000);
    else if (type == DuQFLog::Warning) mainStatusBar->showMessage(message,10000);
    else if (type == DuQFLog::Critical) mainStatusBar->showMessage(message);
    else if (type == DuQFLog::Fatal) mainStatusBar->showMessage(message);
}

void MainWindow::pageChanged(int i)
{
    actionAdmin->setChecked(i == 3);
    actionProjectSettings->setChecked(i == 4);
    actionPipeline->setChecked(i == 5);
    actionAssets->setChecked(i == 6);
    actionShots->setChecked(i == 7);
    actionSchedule->setChecked(i == 8);
    duqf_settingsButton->setChecked(i == 1);
    actionSettings->setChecked(i == 1);
    actionLogIn->setChecked(i == 0);
    ui_propertiesDockWidget->hide();
}

void MainWindow::serverSettings()
{
    mainStack->setCurrentIndex(1);
    settingsWidget->setCurrentIndex(2);
}

void MainWindow::loginAction()
{
    home();
}

void MainWindow::logoutAction()
{
    RamServerInterface::instance()->eraseUserPassword();
    DBInterface::instance()->setOffline();
    Ramses::instance()->setUser(nullptr);
    DBInterface::instance()->setDataFile("");
    home();

    AppUtils::restartApp();
}

void MainWindow::setOfflineAction()
{
    DBInterface::instance()->setOffline();
}

void MainWindow::setOnlineAction()
{
    DBInterface::instance()->setOnline();

    // Trigger a full sync
    if (RamServerInterface::instance()->isOnline()) DBInterface::instance()->fullSync();
}

void MainWindow::databaseSettingsAction()
{
    QString dataFilePath = DBInterface::instance()->dataFile();
    if (dataFilePath == "") return;

    QFileInfo dataFile(dataFilePath);
    if (!dataFile.exists()) return;

    if (!ui_databaseEditWidget)
    {
        ui_databaseEditWidget = new DatabaseEditWidget();
        connect(ui_databaseEditWidget, &DatabaseEditWidget::applied, this, &MainWindow::hidePropertiesDock);
    }

    ui_databaseEditWidget->setDbFile( dataFilePath );
    this->setPropertiesDockWidget( ui_databaseEditWidget, tr("Edit %1").arg(dataFile.baseName()), ":/icons/storage" );
}

void MainWindow::home()
{
    mainStack->setCurrentIndex(0);
}

void MainWindow::userProfile()
{
    mainStack->setCurrentIndex(2);
}

void MainWindow::revealUserFolder()
{
    RamUser *current = Ramses::instance()->currentUser();
    if (current) FileUtils::openInExplorer( current->path() );
}

void MainWindow::admin(bool show)
{
    if (show) mainStack->setCurrentIndex(3);
    else home();
}

void MainWindow::projectSettings(bool show)
{
    if (show) mainStack->setCurrentIndex(4);
    else home();
}

void MainWindow::pipeline(bool show)
{
    if (show) mainStack->setCurrentIndex(5);
    else home();
}

void MainWindow::shots(bool show)
{
    if (show) mainStack->setCurrentIndex(7);
    else home();
}

void MainWindow::assets(bool show)
{
    if (show) mainStack->setCurrentIndex(6);
    else home();
}

void MainWindow::schedule(bool show)
{
    if (show) mainStack->setCurrentIndex(8);
    else home();
}

void MainWindow::install(bool show)
{
    if (show) mainStack->setCurrentIndex(9);
    else home();
}

void MainWindow::currentUserChanged()
{
    disconnect(_currentUserConnection);

    //defaults
    ui_userButton->setText("Guest");
    ui_userButton->setIcon(QIcon(""));
    actionAdmin->setVisible(false);
    actionAdmin->setChecked(false);
    actionUserProfile->setVisible(false);
    actionUserFolder->setVisible(false);

    RamUser *user = Ramses::instance()->currentUser();
    if (!user)
    {
        actionLogIn->setVisible(true);
        actionLogOut->setVisible(false);
        actionSettings->setVisible(true);
        ui_networkButton->setVisible(false);
        home();
        return;
    }

    actionLogIn->setVisible(false);
    actionLogOut->setVisible(true);
    ui_networkButton->setVisible(true);
    actionSettings->setVisible(false);

    _currentUserConnection = connect(user, &RamUser::dataChanged, this, &MainWindow::currentUserChanged);

    ui_userButton->setText(user->shortName());
    actionUserProfile->setVisible(true);
    actionUserFolder->setVisible(true);

    if (user->role() == RamUser::Admin)
    {
        actionAdmin->setVisible(true);
        ui_userButton->setIcon(QIcon(":/icons/admin"));
    }
    else if (user->role() == RamUser::ProjectAdmin)
    {
        ui_userButton->setIcon(QIcon(":/icons/project-admin"));
    }
    else if (user->role() == RamUser::Lead)
    {
        ui_userButton->setIcon(QIcon(":/icons/lead"));
    }
    else
    {
        ui_userButton->setIcon(QIcon(":/icons/user"));
    }
}

void MainWindow::currentProjectChanged(RamProject *project)
{
    actionProjectSettings->setVisible(false);
    actionProjectSettings->setVisible(false);
    actionPipeline->setVisible(false);
    actionPipeline->setChecked(false);
    actionShots->setVisible(false);
    actionShots->setChecked(false);
    actionAssets->setVisible(false);
    actionAssets->setChecked(false);
    actionSchedule->setVisible(false);
    actionStatistics->setVisible(false);
    actionTimeline->setVisible(false);

    if (!project)
    {
        ui_statsTitle->setTitle( "Project" );
        ui_statsDockWidget->hide();
        home();
    }
    else
    {
        RamUser *user = Ramses::instance()->currentUser();
        if (!user) return;

        ui_statsTitle->setTitle( project->name() );

        actionShots->setVisible(true);
        actionAssets->setVisible(true);
        actionSchedule->setVisible(true);
        actionStatistics->setVisible(true);
        actionTimeline->setVisible(true);


        if (user->role() == RamUser::Admin)
        {
            actionProjectSettings->setVisible(true);
            actionPipeline->setVisible(true);
        }
        else if (user->role() == RamUser::ProjectAdmin)
        {
            actionProjectSettings->setVisible(true);
            actionPipeline->setVisible(true);
        }
    }
}

void MainWindow::freezeUI(bool f)
{
    if (f)
    {
        m_currentPageIndex = mainStack->currentIndex();
        mainStack->setCurrentIndex(8);
    }
    else
    {
        mainStack->setCurrentIndex(m_currentPageIndex);
    }
    this->repaint();
}

void MainWindow::dbiConnectionStatusChanged(NetworkUtils::NetworkStatus s)
{
    QString address =  RamServerInterface::instance()->serverAddress();
    if (s == NetworkUtils::Online)
    {
        ui_refreshButton->setVisible(true);
        ui_networkButton->setText(address);
        actionSetOnline->setVisible(false);
        actionSetOffline->setVisible(true);

        if (RamServerInterface::instance()->ssl())
        {
            ui_networkButton->setIcon(QIcon(":/icons/shield"));
            ui_networkButton->setToolTip("Connected.");
            ui_networkButton->setStatusTip("");
        }
        else
        {
            ui_networkButton->setIcon(QIcon(":/icons/no-shield"));
            ui_networkButton->setToolTip("WARNING: Connection is not secured!");
            ui_networkButton->setStatusTip("WARNING: Connection is not secured!");
        }
    }
    else if (s == NetworkUtils::Connecting) ui_networkButton->setText("Connecting to " + address);
    else if (s == NetworkUtils::Offline)
    {
        ui_refreshButton->setVisible(false);
        ui_networkButton->setText("Offline");
        ui_networkButton->setIcon(QIcon(":/icons/storage"));
        ui_networkButton->setToolTip("Offline.");
        ui_networkButton->setStatusTip("");
        actionSetOnline->setVisible(true);
        actionSetOffline->setVisible(false);
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj->objectName() == "mainToolBar")
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = (QMouseEvent*)event;
            if (mouseEvent->button() == Qt::LeftButton)
            {
                duqf_toolBarClicked = true;
                duqf_dragPosition = mouseEvent->globalPos() - this->frameGeometry().topLeft();
                event->accept();
            }
            return true;
        }

        if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent *mouseEvent = (QMouseEvent*)event;
            if (mouseEvent->buttons() & Qt::LeftButton)
            {
                if (duqf_toolBarClicked)
                {
                    if (this->isMaximized()) return false;
                    this->move(mouseEvent->globalPos() - duqf_dragPosition);
                    event->accept();
                }
                return true;
            }
            return false;
        }

        if (event->type() == QEvent::MouseButtonRelease)
        {
            duqf_toolBarClicked = false;
            return false;
        }
    }
    else if (obj->objectName() == "fundingBar")
    {
        if (event->type() == QEvent::MouseButtonPress) {
            duqf_donate();
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Get to the home page first to make sure all toolbars are hidden
    home();

    // Let's save the ui state
    QSettings settings;
    settings.beginGroup("ui");
    settings.setValue("maximized", this->isMaximized());
    settings.setValue("windowState", this->saveState());
    settings.endGroup();

    QFontDatabase::removeAllApplicationFonts();
    trayIcon->hide();
    QMainWindow::closeEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *key)
{
    if(key->key() == Qt::Key_Shift)
    {
        m_shiftPressed = true;
    }
    QMainWindow::keyPressEvent(key);
}

void MainWindow::keyReleaseEvent(QKeyEvent *key)
{
    if(key->key() == Qt::Key_Shift)
    {
        m_shiftPressed = false;
    }
    QMainWindow::keyReleaseEvent(key);
}
