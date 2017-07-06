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

    // Feedback
    showMessage("Let's start!","general");

#ifdef QT_DEBUG
    // Test mode (auto login)
    usernameEdit->setText("Duduf");
    passwordEdit->setText("tp");
#endif

    //load stylesheet
    QString cssFile = "";
#ifdef QT_DEBUG
    cssFile = "E:/DEV SRC/Ramses/Ramses/needed/style.css";
#endif
    updateCSS(cssFile);


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
    mainStatusStopButton = new QPushButton("X",this);

    mainStatusProgress = new QProgressBar(this);
    mainStatusProgress->setTextVisible(false);
    mainStatusProgress->setMaximum(0);
    mainStatusProgress->setMinimum(0);
    mainStatusProgress->setMaximumWidth(100);

    mainStatusBar->addPermanentWidget(mainStatusProgress);
    mainStatusBar->addPermanentWidget(mainStatusStopButton);

    mainStatusProgress->hide();
    mainStatusStopButton->hide();

    //drag window using the toolbar
    toolBarClicked = false;
    mainToolBar->installEventFilter(this);

    //hide settings logout widget
    settingsLogoutWidget->hide();

    //hider server error widget
    serverWidget->hide();

    //========= LOAD SETTINGS ========

    showMessage("Loading settings","local");
    settingsDB = QSqlDatabase::addDatabase("QSQLITE","settings");

    //check if the file already exists, if not, extract it from resources
    QString settingsPath = "";
#ifdef Q_OS_MAC
    settingsPath = QDir::homePath() + "/Ramses/settings.s3db";
#else
    settingsPath = "settings.s3db";
#endif

    QFile dbFile(settingsPath);

    if (!dbFile.exists())
    {
        QFile dbResource(":/settings");
        //on mac, we can not write inside the app, so create folder at home
#ifdef Q_OS_MAC
        QDir home = QDir::home();
        home.mkdir("Ramses");
#endif
        //copy the default file from the resources
        dbResource.copy(settingsPath);
        QFile::setPermissions(settingsPath,QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ReadOther | QFileDevice::WriteOther);
    }

    settingsDB.setDatabaseName(settingsPath);
    settingsDB.setHostName("localhost");
    if (settingsDB.open()) showMessage("Settings Opened","local");
    else showMessage("Settings could not be opened","warning");
    //settings
    QString q = "SELECT networkSettings.serverAddress, networkSettings.ssl, networkSettings.updateFrequency, networkSettings.timeout FROM networkSettings JOIN users ON users.id = networkSettings.userID WHERE users.username = 'Default';";
    QSqlQuery networkSettingsQuery(q,settingsDB);
    networkSettingsQuery.next();
    serverAddressEdit->setText(networkSettingsQuery.value(0).toString());
    sslCheckBox->setChecked(networkSettingsQuery.value(1).toBool());
    updateFreqSpinBox->setValue(networkSettingsQuery.value(2).toInt());
    timeOutEdit->setValue(networkSettingsQuery.value(3).toInt());
    //dispatch settings
    dbi->setServerAddress(networkSettingsQuery.value(0).toString());
    dbi->setSsl(networkSettingsQuery.value(1).toBool());
    dbi->setUpdateFreq(networkSettingsQuery.value(2).toInt());

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

    // helpDialog
    connect(helpDialog,SIGNAL(dock(bool)),this,SLOT(dockHelpDialog(bool)));
    connect(helpDialog,SIGNAL(visibilityChanged(bool)),actionHelp,SLOT(setChecked(bool)));

    // window buttons
    connect(maximizeButton,SIGNAL(clicked()),this,SLOT(maximizeButton_clicked()));
    connect(minimizeButton,SIGNAL(clicked()),this,SLOT(showMinimized()));
    connect(quitButton,SIGNAL(clicked()),qApp,SLOT(quit()));

    // status bar
    connect(mainStatusStopButton, SIGNAL(clicked()), this, SLOT(stopWaiting()));

    // DBI GENERAL
    connect(dbi,SIGNAL(connected(bool, QString)),this,SLOT(connected(bool, QString)));
    connect(dbi,SIGNAL(connecting()),this,SLOT(connecting()));
    connect(dbi,SIGNAL(message(QString,QString)),this,SLOT(showMessage(QString,QString)));
    connect(dbi,SIGNAL(data(QJsonObject)),this,SLOT(dataReceived(QJsonObject)));

    // UPDATER
    connect(updater,SIGNAL(message(QString,QString)),this,SLOT(showMessage(QString,QString)));
}

void MainWindow::updateCSS(QString cssPath)
{
    QFile cssFile(cssPath);
    cssFile.open(QFile::ReadOnly);
    QString css = QString(cssFile.readAll());
    cssFile.close();
    qApp->setStyleSheet(css);
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
    connectionStatusLabel->setText("Ready...");
    connectionStatusLabel->setEnabled(false);
    loginWidget->setEnabled(true);
    passwordEdit->setText("");

    actionMain->setEnabled(false);
    actionStage->setEnabled(false);
    actionStats->setEnabled(false);
    actionAdmin->setEnabled(false);
    actionLogout->setText("Login");
    actionLogout->setChecked(false);

    //enable network settings
    serverSettingsWidget->show();
    settingsLogoutWidget->hide();

    clean();

    showPage(0);

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
    default:
        break;
    }
}

void MainWindow::setWaiting(bool w)
{
    if (w)
    {
        mainStack->setEnabled(false);
        mainStatusProgress->show();
        mainStatusStopButton->show();
    }
    else
    {
        mainStack->setEnabled(true);
        mainStatusProgress->hide();
        mainStatusStopButton->hide();
    }
}

void MainWindow::quit()
{
    logout();
    helpDialog->hide();
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
    }
    else if (type == "remote")
    {
#ifdef QT_DEBUG
        //qDebug() << "REMOTE:";
        //qDebug() << m;
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
    logout();
}

// ========= DEV AND DEBUG ==========

void MainWindow::on_updateCSSButton_clicked()
{
    updateCSS();
}

// ========== DBI ===================

void MainWindow::connected(bool available, QString err)
{
    setWaiting(false);
    if (available)
    {
        //enable actions
        actionMain->setEnabled(true);
        actionStage->setEnabled(true);
        actionStats->setEnabled(true);
        actionAdmin->setEnabled(true);
        actionLogout->setEnabled(true);
        actionLogout->setText("Logout");
        actionLogout->setChecked(true);

        //disable network settings
        serverSettingsWidget->hide();
        settingsLogoutWidget->show();

        actionLogout->setIcon(QIcon(":/icons/logout"));

        //load everything
        updater->updateAll();

        //go to main page
        actionMain->setChecked(true);
        mainStack->setCurrentIndex(1); //show main page

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
    settingsWidget->setCurrentIndex(0); //show network settings
}

void MainWindow::on_usernameEdit_returnPressed()
{
    passwordEdit->setFocus();
}

void MainWindow::on_passwordEdit_returnPressed()
{
    login();
}

//SETTINGS

void MainWindow::on_settingsLogoutButton_clicked()
{
    logout();
}

void MainWindow::on_serverAddressEdit_editingFinished()
{
    QString q = "UPDATE networkSettings ";
    q += "SET serverAddress = '" + serverAddressEdit->text() + "' ";
    q += "WHERE userID = (SELECT id FROM users WHERE username = 'Default');";
    QSqlQuery query(q,settingsDB);
    dbi->setServerAddress(serverAddressEdit->text());
}

void MainWindow::on_sslCheckBox_clicked(bool checked)
{
    QString q = "UPDATE networkSettings ";
    if (checked) q += "SET ssl = 1 ";
    else q += "SET ssl = 0 ";
    q += "WHERE userID = (SELECT id FROM users WHERE username = 'Default');";
    QSqlQuery query(q,settingsDB);
    dbi->setSsl(checked);
}

void MainWindow::on_updateFreqSpinBox_editingFinished()
{
    QString q = "UPDATE networkSettings ";
    q += "SET updateFrequency = " + QString::number(updateFreqSpinBox->value()) + " ";
    q += "WHERE userID = (SELECT id FROM users WHERE username = 'Default');";
    QSqlQuery query(q,settingsDB);
    dbi->setUpdateFreq(updateFreqSpinBox->value());
}

void MainWindow::on_timeOutEdit_editingFinished()
{
    QString q = "UPDATE networkSettings ";
    q += "SET timeout = " + QString::number(timeOutEdit->value()) + " ";
    q += "WHERE userID = (SELECT id FROM users WHERE username = 'Default');";
    QSqlQuery query(q,settingsDB);
}

//ADMIN - IO

void MainWindow::importEDL(QString f)
{
    showMessage("Importing EDL " + f + " (not yet implemented)","warning");
}

void MainWindow::importXML(QString f)
{
    /*setWaiting();
    showMessage("Importing XML " + f,"general);

     //open file and load data
    QFile *xmlFile = new QFile(f);
    if (!xmlFile->open(QFile::ReadOnly | QFile::Text))
    {
        showMessage("XML File could not be opened.","warning");
        return;
    }
    QByteArray xmlData = xmlFile->readAll();
    xmlFile->close();
    delete xmlFile;

    //create reader
    XMLReader xml(xmlData);

    //parse file to get needed informations
    QList<QStringList> shotsFound;
    QString timebase = "24";
    while(!xml.atEnd())
    {
        if (xml.readNext() == QXmlStreamReader::StartElement)
        {
            //find sequence
            if (xml.name().toString() == "sequence")
            {
                //find rate and media
                while(xml.readNextStartElement())
                {
                    if (xml.name().toString() == "rate")
                    {
                        //find timebase
                        while (xml.readNextStartElement())
                        {
                            if (xml.name() == "timebase") timebase = xml.readElementText();
                            else xml.skipCurrentElement();
                        }
                    }
                    else if (xml.name().toString() == "media")
                    {
                        showMessage("Media found");
                        //find video and audio
                        while (xml.readNextStartElement())
                        {
                            if (xml.name().toString() == "video" || xml.name() == "audio")
                            {
                                if (xml.name().toString() == "video") showMessage("Video found");
                                if (xml.name().toString() == "video") showMessage("Audio found");
                                //find track
                                while (xml.readNextStartElement())
                                {
                                    if (xml.name() == "track")
                                    {
                                        showMessage("Track found");
                                        //find clipitem
                                        while (xml.readNextStartElement())
                                        {
                                            if (xml.name() == "clipitem")
                                            {
                                                //find name, start, end
                                                QString name = "";
                                                QString start = "0";
                                                QString end = "0";
                                                while (xml.readNextStartElement())
                                                {
                                                    if (xml.name().toString() == "name")
                                                        name = xml.readElementText();
                                                    else if (xml.name().toString() == "start")
                                                        start = xml.readElementText();
                                                    else if (xml.name().toString() == "end")
                                                        end = xml.readElementText();
                                                    else xml.skipCurrentElement();
                                                }
                                                QStringList details;
                                                details << name << start << end;
                                                shotsFound << details;
                                            }
                                            else xml.skipCurrentElement();//<track>
                                        }
                                    }
                                    else xml.skipCurrentElement();//<video> OR <audio>
                                }
                            }
                            else xml.skipCurrentElement();//<media>
                        }
                    }
                    else xml.skipCurrentElement(); //<sequence>
                }
            }
        }
    }

    if (xml.hasError())
    {
          showMessage("XML error");
          showMessage(xml.errorString());
    }

    //Add shots

    //get order (if a row is selected, or else insert after the last row)
    int order = 0;
    if (shotsAdminList->currentItem())
    {
        order = shotsList[shotsAdminList->currentRow()]->getShotOrder()+1;
    }
    else if (shotsAdminList->count() > 0)
    {
        order = shotsList[shotsAdminList->count()-1]->getShotOrder()+1;
    }

    shotsAdminReset();

    //getProject
    int projectId = projectSelector->currentData().toInt();
    //get status
    int statusId = 0;
    foreach(RAMStatus *s,statusesList)
    {
        if (s->getShortName() == "STB")
        {
            statusId = s->getId();
            break;
        }
    }

    QList<QStringList> shotsReady;

    foreach(QStringList s,shotsFound)
    {
        float duration = (s[2].toFloat() - s[1].toFloat()) / timebase.toFloat();
        QString name = s[0];
        QStringList currentShot;
        currentShot << name << QString::number(duration);
        shotsReady << currentShot;
    }

    dbi->addShots(projectId,statusId,shotsReady,order);*/
}

// ========= ACTIONS ================

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
    }
    else
    {
        actionLogout->setChecked(false);
        if (mainStack->currentIndex() == 0) login();
        else
        {
            helpDialog->showHelp(0);
            mainStack->setCurrentIndex(0);
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

