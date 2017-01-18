#include "mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    qDebug() << "Let's start!";
    setupUi(this);

    //resources
    resourcesFolder = "../";

    //======== UI ============

    //default stylesheet
    //QApplication::setStyle(QStyleFactory::create("Fusion"));
    updateCSS();

    //center login widget and server error widget
    loginPageLayout->setAlignment(loginWidget, Qt::AlignHCenter);
    loginPageLayout->setAlignment(serverWidget, Qt::AlignHCenter);

    //Add project and stage selector
    ProjectSelectorWidget *projectSelector = new ProjectSelectorWidget();
    mainToolBar->insertWidget(actionSettings,projectSelector);

    //Add window buttons
    QPushButton *maximizeButton = new QPushButton("∏");
    QPushButton *minimizeButton = new QPushButton("_");
    QPushButton *quitButton = new QPushButton("X");
    connect(maximizeButton,SIGNAL(clicked()),this,SLOT(maximizeButton_clicked()));
    connect(minimizeButton,SIGNAL(clicked()),this,SLOT(showMinimized()));
    connect(quitButton,SIGNAL(clicked()),qApp,SLOT(quit()));
    mainToolBar->addWidget(minimizeButton);
    mainToolBar->addWidget(maximizeButton);
    mainToolBar->addWidget(quitButton);

    //statusbar
    mainStatusStopButton = new QPushButton("X");
    connect(mainStatusStopButton, SIGNAL(clicked()), this, SLOT(stopWaiting()));

    mainStatusProgress = new QProgressBar();
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

    //TEMP =====  UI TESTS ==========
    QComboBox *test = new QComboBox();
    test->addItem("OK");
    test->addItem("CHK");
    test->addItem("RTK");
    test->setCurrentIndex(2);
    test->setStyleSheet("background-color:red;");
    mainTable->setCellWidget(1,2,test);

    //========= INITIALIZE ==========
    //dbinterface
    qDebug() << "Starting DBI";
    dbi = new DBInterface(this);
    connect(dbi,SIGNAL(connected(bool, QString)),this,SLOT(connected(bool, QString)));
    connect(dbi,SIGNAL(connecting()),this,SLOT(connecting()));
    connect(dbi,SIGNAL(message(QString,int)),this,SLOT(showMessage(QString,int)));

    //connect DBI status
    connect(dbi,SIGNAL(statusAdded(bool,QString)),this,SLOT(statusAdded(bool,QString)));
    connect(dbi,SIGNAL(gotStatuses(bool,QString,QJsonValue)),this,SLOT(gotStatuses(bool,QString,QJsonValue)));
    connect(dbi,SIGNAL(statusUpdated(bool,QString)),this,SLOT(statusUpdated(bool,QString)));
    connect(dbi,SIGNAL(statusRemoved(bool,QString)),this,SLOT(statusRemoved(bool,QString)));
    //connect DBI stages
    connect(dbi,SIGNAL(stageAdded(bool,QString)),this,SLOT(stageAdded(bool,QString)));
    connect(dbi,SIGNAL(gotStages(bool,QString,QJsonValue)),this,SLOT(gotStages(bool,QString,QJsonValue)));
    connect(dbi,SIGNAL(stageUpdated(bool,QString)),this,SLOT(stageUpdated(bool,QString)));
    connect(dbi,SIGNAL(stageRemoved(bool,QString)),this,SLOT(stageRemoved(bool,QString)));

    //========= LOAD SETTINGS ========
    qDebug() << "Loading settings";
    settingsDB = QSqlDatabase::addDatabase("QSQLITE");
    settingsDB.setDatabaseName(resourcesFolder + "settings.s3db");
    settingsDB.setHostName("localhost");
    settingsDB.open();
    qDebug() << "SettingsDB Opened";
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

    //detect inactivity
    connect(qApp,SIGNAL(idle()),this,SLOT(idle()));

    qDebug() << "Ready!";
}

// ========= GENERAL METHODS ========

void MainWindow::updateCSS()
{
    QFile cssFile(resourcesFolder + "style.css");
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

    connection();
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

    //empty everything
    qDeleteAll(statusesList);
    statusesList.clear();
    qDeleteAll(stagesList);
    stagesList.clear();

    showPage(0);
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
        actionAdmin->setChecked(true);
        break;
    case 5:
        actionSettings->setChecked(true);
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

void MainWindow::stopWaiting()
{
    setWaiting(false);
}

void MainWindow::connection()
{
    //test connexion
    dbi->connection(username,passHash);
}

void MainWindow::showMessage(QString m, int i)
{
    mainStatusBar->showMessage(m,i);
    qDebug() << m;
}

void MainWindow::idle()
{
    showMessage("Session timed out, you have been logged out.");
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

        //load everything
        getStatuses();
        getStages();

        //go to main page
        actionMain->setChecked(true);
        mainStack->setCurrentIndex(1); //show main page


    }
    else
    {
        logout();
        serverSettingsButton->show();
        serverWidget->show();
        loginWidget->setEnabled(true);
        connectionStatusLabel->setText(err);
        connectionStatusLabel->setEnabled(true);
    }
}

void MainWindow::connecting()
{
    connectionStatusLabel->setText("Connecting...");
}

// ========= BUTTONS ================

//LOGIN
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

//ADMIN GENERAL
void MainWindow::on_adminWidget_currentChanged(int index)
{
    statusesAdminReset();

    stagesAdminReset();
}

//ADMIN - STATUS
void MainWindow::on_statusColorButton_clicked()
{
    this->setEnabled(false);
    QColorDialog cd(QColor("#" + statusColorEdit->text()));
    cd.setOptions(QColorDialog::DontUseNativeDialog);
    cd.setWindowFlags(Qt::FramelessWindowHint);
    cd.setWindowTitle("Select Status Color");
    if (cd.exec())
    {
        QColor color = cd.selectedColor();
        QString colorHex = color.name();
        statusColorEdit->setText(colorHex.right(6));
    }
    this->setEnabled(true);
}

//add a new default status
void MainWindow::on_addStatusButton_clicked()
{
    setWaiting();
    statusesAdminReset();
    dbi->addStatus();
}

void MainWindow::statusAdded(bool success,QString message)
{
    setWaiting(false);
    if (!success) return;
    //refresh status list
    getStatuses();
}

//get all statuses
void MainWindow::getStatuses()
{
    setWaiting();
    statusesAdminReset();
    dbi->getStatuses();
}

void MainWindow::gotStatuses(bool success,QString message,QJsonValue statuses)
{
    setWaiting(false);
    if (!success) return;

    //clear UI and stored list
    statusAdminList->clear();
    qDeleteAll(statusesList);
    statusesList.clear();

    QJsonArray statusesArray = statuses.toArray();
    int newRow = -1;
    foreach (QJsonValue sta, statusesArray)
    {
        //get values
        QJsonObject status = sta.toObject();
        QString name = status.value("name").toString();
        QString shortName = status.value("shortName").toString();
        QColor color = "#" + status.value("color").toString();
        QString description = status.value("description").toString();
        int id = status.value("id").toInt();
        //create UI item
        QListWidgetItem *item = new QListWidgetItem(shortName + " | " + name);
        item->setBackgroundColor(color);
        item->setToolTip(description);
        statusAdminList->addItem(item);
        if (shortName == "New" && name == "New status")
        {
            newRow = statusAdminList->count()-1;
        }
        //add status to stored list
        RAMStatus *rs = new RAMStatus(id,name,shortName,color,description);
        statusesList << rs;
    }
    if (newRow > -1)
    {
        statusAdminList->setCurrentRow(newRow);
        on_statusAdminList_itemClicked(statusAdminList->item(newRow));
    }
}

//edit status
void MainWindow::on_statusAdminList_itemClicked(QListWidgetItem *i)
{
   int currentRow = statusAdminList->currentRow();

    RAMStatus *s = statusesList[currentRow];

    statusNameEdit->setText(s->getName());
    statusShortNameEdit->setText(s->getShortName());
    statusColorEdit->setText(s->getColor().name().right(6));
    statusDescriptionEdit->setPlainText(s->getDescription());

    statusConfigWidget->setEnabled(true);
}

void MainWindow::on_statusApplyButton_clicked()
{
    int currentRow = statusAdminList->currentRow();
    if (currentRow < 0) return;

    setWaiting();

    int id = statusesList[currentRow]->getId();
    dbi->updateStatus(id,statusNameEdit->text(),statusShortNameEdit->text(),statusColorEdit->text(),statusDescriptionEdit->toPlainText());

    statusesAdminReset();
}

void MainWindow::statusUpdated(bool success,QString message)
{
    setWaiting(false);
    if (!success) return;
    getStatuses();
}

//remove status
void MainWindow::on_removeStatusButton_clicked()
{
    int currentRow = statusAdminList->currentRow();
    if (currentRow < 0) return;

    setWaiting();

    int id = statusesList[currentRow]->getId();
    dbi->removeStatus(id);
    statusesAdminReset();
}

void MainWindow::statusRemoved(bool success,QString message)
{
    setWaiting(false);
    if (!success) return;
    getStatuses();
}

void MainWindow::statusesAdminReset()
{
    statusAdminList->setCurrentRow(-1);
    statusNameEdit->setText("");
    statusShortNameEdit->setText("");
    statusColorEdit->setText("");
    statusDescriptionEdit->setPlainText("");
    statusConfigWidget->setEnabled(false);
}

//ADMIN - STAGES

//add a new default stage
void MainWindow::on_addStageButton_clicked()
{
    setWaiting();
    stagesAdminReset();
    dbi->addStage();
}

void MainWindow::stageAdded(bool success,QString message)
{
    setWaiting(false);
    if (!success) return;
    //refresh stages list
    getStages();
}

//get all stages
void MainWindow::getStages()
{
    setWaiting();
    stagesAdminReset();
    dbi->getStages();
}

void MainWindow::gotStages(bool success,QString message,QJsonValue stages)
{
    setWaiting(false);
    if (!success) return;

    //clear UI and stored list
    stagesAdminList->clear();
    qDeleteAll(stagesList);
    stagesList.clear();

    QJsonArray stagesArray = stages.toArray();
    int newRow = -1;
    foreach (QJsonValue sta, stagesArray)
    {
        //get values
        QJsonObject stage = sta.toObject();
        QString name = stage.value("name").toString();
        QString shortName = stage.value("shortName").toString();
        QString type = stage.value("type").toString();
        int id = stage.value("id").toInt();
        //create UI item
        QListWidgetItem *item = new QListWidgetItem(shortName + " | " + name);
        if (type == "s") item->setToolTip("Shots");
        else if (type == "a") item->setToolTip("Assets");

        stagesAdminList->addItem(item);
        if (shortName == "New" && name == "New stage")
        {
            newRow = stagesAdminList->count()-1;
        }
        //add status to stored list
        RAMStage *rs = new RAMStage(id,name,shortName,type);
        stagesList << rs;
    }
    if (newRow > -1)
    {
        stagesAdminList->setCurrentRow(newRow);
        on_stagesAdminList_itemClicked(stagesAdminList->item(newRow));
    }
}

//edit stage
void MainWindow::on_stagesAdminList_itemClicked(QListWidgetItem *item)
{
    int currentRow = stagesAdminList->currentRow();

     RAMStage *s = stagesList[currentRow];

     stageNameEdit->setText(s->getName());
     stageShortNameEdit->setText(s->getShortName());
     if (s->getType() == "s") stageTypeList->setCurrentIndex(0);
     else if (s->getType() == "a") stageTypeList->setCurrentIndex(1);
     stageConfigWidget->setEnabled(true);
}

void MainWindow::on_stageApplyButton_clicked()
{
    int currentRow = stagesAdminList->currentRow();
    if (currentRow < 0) return;

    setWaiting();

    int id = stagesList[currentRow]->getId();
    QString t = "";
    if (stageTypeList->currentIndex() == 0) t = "s";
    else t = "a";
    dbi->updateStage(id,stageNameEdit->text(),stageShortNameEdit->text(),t);
    stagesAdminReset();
}

void MainWindow::stageUpdated(bool success,QString message)
{
    setWaiting(false);
    if (!success) return;
    getStages();
}

//remove stage
void MainWindow::on_removeStageButton_clicked()
{
    int currentRow = stagesAdminList->currentRow();
    if (currentRow < 0) return;

    setWaiting();

    int id = stagesList[currentRow]->getId();
    dbi->removeStage(id);
    stagesAdminReset();
}

void MainWindow::stageRemoved(bool success,QString message)
{
    setWaiting(false);
    if (!success) return;
    getStages();
}

void MainWindow::stagesAdminReset()
{
    stagesAdminList->setCurrentRow(-1);
    stageNameEdit->setText("");
    stageShortNameEdit->setText("");
    stageTypeList->setCurrentIndex(0);
    stageConfigWidget->setEnabled(false);
}


// ========= ACTIONS ================

// MAIN PAGE
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
        mainStack->setCurrentIndex(0); //show login page
    }
    else
    {
        actionLogout->setChecked(false);
        if (mainStack->currentIndex() == 0) login();
        else mainStack->setCurrentIndex(0);
    }
}

// ============ WINDOW BUTTONS ======

void MainWindow::maximizeButton_clicked()
{
    if (this->isMaximized())
    {
        this->showNormal();
    }
    else
    {
        this->showMaximized();
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

