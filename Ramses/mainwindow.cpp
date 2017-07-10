#include "mainwindow.h"
#ifdef QT_DEBUG
    #include <QtDebug>
#endif


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    setupUi(this);

    //========= INITIALIZE ==========

    //dbinterface
    dbi = new DBInterface(this);

    //updater
    updater = new Updater(dbi,this);

    // Get the desktop (to manage windows locations)
    desktop = qApp->desktop();

    // Help Dialog
    helpDialog = new HelpDialog();
    helpDialogDocked = true;

    //settings
    settingsWidget = new SettingsWidget(dbi,updater);
    settingsPageLayout->addWidget(settingsWidget);

    // Feedback
    showMessage("Let's start!","general");

#ifdef QT_DEBUG
    // Test mode (auto login)
    usernameEdit->setText("Duduf");
    passwordEdit->setText("tp");
#endif

    // INIT UI

    setToolBarStyle(settingsWidget->getToolButtonStyle());
    helpDialog->setToolButtonStyle(settingsWidget->getToolButtonStyle());

    //center login widget and server error widget
    loginPageLayout->setAlignment(loginWidget, Qt::AlignHCenter);
    loginPageLayout->setAlignment(serverWidget, Qt::AlignHCenter);

    //Add project and stage selector
    projectSelector = new ProjectSelectorWidget(updater,this);
    mainToolBar->insertWidget(actionSettings,projectSelector);

    //Add window buttons
    maximizeButton = new QPushButton(QIcon(":/icons/maximize"),"",this);
    minimizeButton = new QPushButton(QIcon(":/icons/minimize"),"",this);
    quitButton = new QPushButton(QIcon(":/icons/close"),"",this);
    mainToolBar->addWidget(minimizeButton);
    mainToolBar->addWidget(maximizeButton);
    mainToolBar->addWidget(quitButton);

    //Add admin
    adminWidget = new AdminWidget(dbi,updater,this);
    adminPageLayout->addWidget(adminWidget);

    //Add main table
    mainTable = new MainTable(dbi,updater,this);
    mainPageLayout->addWidget(mainTable);

    //statusbar
    mainStatusProgress = new QProgressBar(this);
    mainStatusProgress->setTextVisible(false);
    mainStatusProgress->setMaximum(0);
    mainStatusProgress->setMinimum(0);
    mainStatusProgress->setMaximumWidth(100);
    mainStatusBar->addPermanentWidget(mainStatusProgress);
    mainStatusProgress->hide();

    //drag window using the toolbar
    toolBarClicked = false;
    mainToolBar->installEventFilter(this);

    //hider server error widget
    serverWidget->hide();

    mainStack->setCurrentIndex(0);
    loginButton->setFocus();

    //Connections
    mapEvents();

    showMessage("Ready!","general");
}

// ========= GENERAL METHODS ========

void MainWindow::mapEvents()
{
    // general
    connect(qApp,SIGNAL(aboutToQuit()),this,SLOT(quit()));
    connect(qApp,SIGNAL(idle()),this,SLOT(idle()));

    // settings
    connect(settingsWidget,SIGNAL(loggedOut()),this,SLOT(logout()));
    connect(settingsWidget,SIGNAL(setToolButtonStyle(int)),this,SLOT(setToolBarStyle(int)));
    connect(settingsWidget,SIGNAL(setToolButtonStyle(int)),helpDialog,SLOT(setToolButtonStyle(int)));

    // helpDialog
    connect(helpDialog,SIGNAL(dock(bool)),this,SLOT(dockHelpDialog(bool)));
    connect(helpDialog,SIGNAL(visibilityChanged(bool)),actionHelp,SLOT(setChecked(bool)));

    // mainTable
    connect(mainTable,SIGNAL(working(bool)),this,SLOT(setWaiting(bool)));
    connect(mainTable,SIGNAL(showProgress()),this,SLOT(showProgress()));
    connect(mainTable,&MainTable::progressMax,mainStatusProgress,&QProgressBar::setMaximum);
    connect(mainTable,&MainTable::progress,mainStatusProgress,&QProgressBar::setValue);
    connect(mainTable,&MainTable::progressMax,waitingProgressBar,&QProgressBar::setMaximum);
    connect(mainTable,&MainTable::progress,waitingProgressBar,&QProgressBar::setValue);
    connect(mainTable,SIGNAL(message(QString,QString)),this,SLOT(showMessage(QString,QString)));

    //admin
    connect(adminWidget,SIGNAL(message(QString,QString)),this,SLOT(showMessage(QString,QString)));
    connect(adminWidget,SIGNAL(working(bool)),this,SLOT(setWaiting(bool)));

    // window buttons
    connect(maximizeButton,SIGNAL(clicked()),this,SLOT(maximizeButton_clicked()));
    connect(minimizeButton,SIGNAL(clicked()),this,SLOT(showMinimized()));
    connect(quitButton,SIGNAL(clicked()),qApp,SLOT(quit()));

    // DBI GENERAL
    connect(dbi,SIGNAL(connected(bool, QString)),this,SLOT(connected(bool, QString)));
    connect(dbi,SIGNAL(connecting()),this,SLOT(connecting()));
    connect(dbi,SIGNAL(message(QString,QString)),this,SLOT(showMessage(QString,QString)));
    connect(dbi,SIGNAL(data(QJsonObject)),this,SLOT(dataReceived(QJsonObject)));

    // UPDATER
    connect(updater,SIGNAL(message(QString,QString)),this,SLOT(showMessage(QString,QString)));
    connect(updater,SIGNAL(working(bool)),this,SLOT(setWaiting(bool)));
}

void MainWindow::login()
{
    //check login in database, initiate
    if (usernameEdit->text() == "")
    {
        connectionStatusLabel->setText("Please fill your username in.");
        return;
    }
    if (passwordEdit->text() == "")
    {
        connectionStatusLabel->setText("Please fill your password in.");
        return;
    }

    //hash password
    QString salt = "4JZFyVWhJLEM8nan";
    QString passToHash = salt + passwordEdit->text();
    passHash = QCryptographicHash::hash(passToHash.toUtf8(), QCryptographicHash::Sha3_512).toHex();
    username = usernameEdit->text();

    showPage(0);

    loginWidget->setEnabled(false);
    connectionStatusLabel->setText("Initializing connection...");
    connectionStatusLabel->setEnabled(true);
    serverSettingsButton->hide();
    setWaiting();
    this->repaint();

    //test connexion
    dbi->connection(username,passHash);
}

void MainWindow::logout()
{
    //stop the updater
    updater->stop();

    connectionStatusLabel->setText("Ready...");
    connectionStatusLabel->setEnabled(false);
    loginWidget->setEnabled(true);
    passwordEdit->setText("");

    actionRefresh->setEnabled(false);
    actionMain->setEnabled(false);
    actionStage->setEnabled(false);
    actionStats->setEnabled(false);
    actionAdmin->setEnabled(false);
    actionLogout->setText("Login");
    actionLogout->setChecked(false);

    settingsWidget->logout();

    clean();

    showMessage("Logged out.","general");
}

void MainWindow::clean()
{
    mainTable->clean();
    projectSelector->clear();
    updater->clean();
}

void MainWindow::showPage(int page)
{
    mainStack->setCurrentIndex(page);

    if (page != 6) currentPage = page;

    actionMain->setChecked(false);
    actionStage->setChecked(false);
    actionStats->setChecked(false);
    actionAdmin->setChecked(false);
    actionSettings->setChecked(false);

    switch (page)
    {
    case 0:
        helpDialog->showHelp(0);
        break;
    case 1:
        actionMain->setChecked(true);
        break;
    case 2:
        actionStage->setChecked(true);
        break;
    case 3:
        actionStats->setChecked(true);
        break;
    case 4:
        adminWidget->init();
        actionAdmin->setChecked(true);
        break;
    case 5:
        actionSettings->setChecked(true);
        helpDialog->showHelp(1);
        break;
    case 6:
        break;
    default:
        break;
    }
}

void MainWindow::setWaiting(bool w)
{
    if (w)
    {
        mainStack->setEnabled(false);
        actionRefresh->setEnabled(false);
        mainStatusProgress->show();
        setCursor(Qt::WaitCursor);
        repaint();
    }
    else
    {
        actionRefresh->setEnabled(true);
        showPage(currentPage);
        mainStack->setEnabled(true);
        mainStatusProgress->hide();
        setCursor(Qt::ArrowCursor);
        mainStatusProgress->setValue(0);
        mainStatusProgress->setMaximum(0);
    }

}

void MainWindow::showProgress()
{
    showPage(6);
}

void MainWindow::quit()
{
    logout();
    helpDialog->hide();
}

void MainWindow::setToolBarStyle(int s)
{
    if (s == 0)
    {
        this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    }
    else if (s == 1)
    {
        this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    }
    else if (s == 2)
    {
        this->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
    else if (s == 3)
    {
        this->setToolButtonStyle(Qt::ToolButtonTextOnly);
    }
}

void MainWindow::stopWaiting()
{
    setWaiting(false);
}

void MainWindow::showMessage(QString m,QString type)
{
    if (m == "") return;

    if (type == "general")
    {
        mainStatusBar->showMessage(m,5000);
        helpDialog->showDebug(m);
        waitingLabel->setText(m);
    }
    else if (type == "remote")
    {
#ifdef QT_DEBUG
        qDebug() << "REMOTE:";
        qDebug() << m;
#endif
    }
    else if (type == "local")
    {
#ifdef QT_DEBUG
        //qDebug() << "LOCAL:";
        //qDebug() << m;
#endif
    }
    else if (type == "connexion")
    {
#ifdef QT_DEBUG
        //qDebug() << "CONNEXION:";
        //qDebug() << m;
#endif
    }
    else if (type == "critical")
    {
        mainStatusBar->showMessage(m);
        helpDialog->showDebug(m);
#ifdef QT_DEBUG
    qDebug() << m;
#endif
    }
    else if (type == "warning")
    {
        mainStatusBar->showMessage(m,5000);
        helpDialog->showDebug(m);
#ifdef QT_DEBUG
    qDebug() << m;
#endif
    }
#ifdef QT_DEBUG
    else if (type == "debug")
    {
        qDebug() << m;
    }
#endif



}

void MainWindow::idle()
{
    showMessage("Session timed out, you have been logged out.","general");
    showPage(0);
    logout();
}

// ========== DBI ===================

void MainWindow::connected(bool available, QString err)
{
    setWaiting(false);
    if (available)
    {
        //enable actions
        actionRefresh->setEnabled(true);
        actionMain->setEnabled(true);
        actionStage->setEnabled(true);
        actionStats->setEnabled(true);
        actionAdmin->setEnabled(true);
        actionLogout->setEnabled(true);
        actionLogout->setText("Logout");
        actionLogout->setChecked(true);

        settingsWidget->login();

        actionLogout->setIcon(QIcon(":/icons/logout"));

        //load everything and starts the updater
        updater->updateAll();
        updater->start();

        //go to main page
        actionMain->setChecked(true);
        showPage(1); //show main page

        showMessage(err,"general");
    }
    else
    {
        logout();
        serverSettingsButton->show();
        serverWidget->show();
        loginWidget->setEnabled(true);
        connectionStatusLabel->setText(err);
        showMessage(err,"critical");
        connectionStatusLabel->setEnabled(true);
        showPage(0); //show login page
    }
}

void MainWindow::connecting()
{
    connectionStatusLabel->setText("Connecting...");
}

void MainWindow::dataReceived(QJsonObject data)
{
    //extract data
    QString message = data.value("message").toString();
    QString type = data.value("type").toString();
    bool accepted = data.value("accepted").toBool();
    bool success = data.value("success").toBool();

    //if not accepted or unsuccessful, set to offline
    if (!accepted || !success)
    {
        if (message == "") message = "The server could not compute the request.";
        connected(false,message);
        return;
    }

    // LOGIN
    if (type == "login")
    {
        connected(success,message);
        return;
    }
}

// ========= BUTTONS ================

//LOGIN PAGE

void MainWindow::on_loginButton_clicked()
{
    login();
}

void MainWindow::on_serverSettingsButton_clicked()
{
    actionSettings->setChecked(true);
    actionMain->setChecked(false);
    actionStage->setChecked(false);
    actionStats->setChecked(false);
    actionAdmin->setChecked(false);
    mainStack->setCurrentIndex(5); //show settings page
}

void MainWindow::on_usernameEdit_returnPressed()
{
    passwordEdit->setFocus();
}

void MainWindow::on_passwordEdit_returnPressed()
{
    login();
}

// ========= ACTIONS ================

void MainWindow::on_actionRefresh_triggered()
{
    updater->updateAll();
}

void MainWindow::on_actionMain_triggered(bool checked)
{
    showPage(1);
}

void MainWindow::on_actionStage_triggered(bool checked)
{
    showPage(2);
}

void MainWindow::on_actionStats_triggered(bool checked)
{
    showPage(3);
}

void MainWindow::on_actionAdmin_triggered(bool checked)
{
    showPage(4);
}

void MainWindow::on_actionSettings_triggered(bool checked)
{
    showPage(5);
}

void MainWindow::on_actionLogout_triggered(bool checked)
{
    actionMain->setChecked(false);
    actionStage->setChecked(false);
    actionStats->setChecked(false);
    actionAdmin->setChecked(false);
    actionSettings->setChecked(false);

    if (!checked)
    {
        logout();
        actionLogout->setIcon(QIcon(":/icons/login"));
        mainStack->setCurrentIndex(0); //show login page
        helpDialog->showHelp(0);
        showPage(0);
    }
    else
    {
        actionLogout->setChecked(false);
        if (mainStack->currentIndex() == 0) login();
        else
        {
            helpDialog->showHelp(0);
            showPage(0);
        }
    }
}

void MainWindow::on_actionHelp_triggered(bool checked)
{
    if (checked)
    {
        dockHelpDialog(helpDialogDocked);
        helpDialog->show();
    }
    else helpDialog->hide();
}

// ============ WINDOW BUTTONS AND UI ======

void MainWindow::maximizeButton_clicked()
{
    if (this->isMaximized())
    {
        maximizeButton->setIcon(QIcon(":/icons/maximize"));
        this->showNormal();
    }
    else
    {
        maximizeButton->setIcon(QIcon(":/icons/minimize2"));
        this->showMaximized();
    }
}

void MainWindow::dockHelpDialog(bool dock)
{
    helpDialogDocked = dock;
    if (!helpDialogDocked) return;
    QPoint topRight = this->geometry().topRight();
    QPoint topLeft = this->geometry().topLeft();
    QRect geo = desktop->screenGeometry(this);
    if (this->isMaximized())
    {
        helpDialog->move(topRight.x() - helpDialog->width(),topRight.y() + mainToolBar->height());
        helpDialog->activateWindow();
    }
    else if (topRight.x() <= geo.topRight().x() - helpDialog->width())
    {
        helpDialog->move(topRight.x(),topRight.y());
    }
    else if (topLeft.x() >= geo.topLeft().x() + helpDialog->width())
    {
        helpDialog->move(topLeft.x() - helpDialog->width(),topLeft.y());
    }
    else
    {
        helpDialog->move(topRight.x() - helpDialog->width(),topRight.y() + mainToolBar->height());
    }
}

// ============ EVENTS ==============

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseButtonPress)
  {
      QMouseEvent *mouseEvent = (QMouseEvent*)event;
      if (mouseEvent->button() == Qt::LeftButton)
      {
        toolBarClicked = true;
        dragPosition = mouseEvent->globalPos() - this->frameGeometry().topLeft();
        event->accept();
      }
      return true;
  }
  else if (event->type() == QEvent::MouseMove)
  {
    QMouseEvent *mouseEvent = (QMouseEvent*)event;
    if (mouseEvent->buttons() & Qt::LeftButton && toolBarClicked)
    {
        if (this->isMaximized()) this->showNormal();
        this->move(mouseEvent->globalPos() - dragPosition);
        event->accept();
    }
    return true;
  }
  else if (event->type() == QEvent::MouseButtonRelease)
  {
      toolBarClicked = false;
      return true;
  }
  else if (event->type() == QEvent::MouseButtonDblClick)
  {
      maximizeButton_clicked();
      event->accept();
      return true;
  }
  else
  {
      // standard event processing
      return QObject::eventFilter(obj, event);
  }
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::Resize || event->type() == QEvent::WindowStateChange || event->type() == QEvent::Move)
    {
        if (helpDialogDocked)
        {
            dockHelpDialog(true);
        }
        return true;
    }

    return QMainWindow::event(event);
}


