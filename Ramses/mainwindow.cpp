#include "mainwindow.h"
#ifdef QT_DEBUG
    #include <QtDebug>
#endif


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    setupUi(this);

    // Get the desktop (to manage windows locations)
    desktop = qApp->desktop();

    // Help Dialog
    helpDialog = new HelpDialog();
    helpDialogDocked = true;

    // Feedback
    showMessage("Let's start!");

    freezeSelectors = true;

#ifdef QT_DEBUG
    // Test mode (auto login)
    usernameEdit->setText("Duduf");
    passwordEdit->setText("tp");
#endif

    //======== SETUP UI ============

    //load stylesheet
    //QApplication::setStyle(QStyleFactory::create("Fusion"));
    updateCSS();

    //center login widget and server error widget
    loginPageLayout->setAlignment(loginWidget, Qt::AlignHCenter);
    loginPageLayout->setAlignment(serverWidget, Qt::AlignHCenter);

    //Add project and stage selector
    ProjectSelectorWidget *projectSelectorW = new ProjectSelectorWidget();
    mainToolBar->insertWidget(actionSettings,projectSelectorW);
    projectSelector = projectSelectorW->projectsBox();
    stageSelector = projectSelectorW->stagesBox();

    //Add window buttons
    maximizeButton = new QPushButton(QIcon(":/icons/maximize"),"");
    minimizeButton = new QPushButton(QIcon(":/icons/minimize"),"");
    quitButton = new QPushButton(QIcon(":/icons/close"),"");
    mainToolBar->addWidget(minimizeButton);
    mainToolBar->addWidget(maximizeButton);
    mainToolBar->addWidget(quitButton);

    //statusbar
    mainStatusStopButton = new QPushButton("X");

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

    //========= INITIALIZE ==========

    //dbinterface
    showMessage("Starting DBI");
    dbi = new DBInterface(this);

    //========= LOAD SETTINGS ========

    showMessage("Loading settings");
    settingsDB = QSqlDatabase::addDatabase("QSQLITE");

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
    if (settingsDB.open()) showMessage("Settings Opened");
    else showMessage("Settings could not be opened");
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

    //Connections
    mapEvents();

    freezeSelectors = false;

    showMessage("Ready!");
}

// ========= GENERAL METHODS ========

void MainWindow::mapEvents()
{
    // helpDialog
    connect(helpDialog,SIGNAL(dock(bool)),this,SLOT(dockHelpDialog(bool)));
    connect(helpDialog,SIGNAL(visibilityChanged(bool)),actionHelp,SLOT(setChecked(bool)));

    // project selector
    connect(projectSelector,SIGNAL(currentIndexChanged(int)),this,SLOT(selectorProjectChanged(int)));

    // window buttons
    connect(maximizeButton,SIGNAL(clicked()),this,SLOT(maximizeButton_clicked()));
    connect(minimizeButton,SIGNAL(clicked()),this,SLOT(showMinimized()));
    connect(quitButton,SIGNAL(clicked()),qApp,SLOT(quit()));

    // status bar
    connect(mainStatusStopButton, SIGNAL(clicked()), this, SLOT(stopWaiting()));

    // DBI GENERAL
    connect(dbi,SIGNAL(connected(bool, QString)),this,SLOT(connected(bool, QString)));
    connect(dbi,SIGNAL(connecting()),this,SLOT(connecting()));
    connect(dbi,SIGNAL(message(QString,int)),this,SLOT(showMessage(QString,int)));
    connect(dbi,SIGNAL(data(QJsonObject)),this,SLOT(dataReceived(QJsonObject)));
}

void MainWindow::updateCSS()
{
    QFile cssFile(":/styles/default");
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

    //empty everything

    qDeleteAll(statusesList);
    statusesList.clear();
    statusAdminList->clear();

    qDeleteAll(stagesList);
    stagesList.clear();
    stagesAdminList->clear();

    qDeleteAll(projectsList);
    projectsList.clear();
    projectAdminList->clear();
    projectSelector->clear();

    qDeleteAll(allShots);
    allShots.clear();
    shotsAdminList->clear();

    qDeleteAll(assetsList);
    assetsList.clear();

    mainTable->clear();
    mainTable->setRowCount(0);
    mainTable->setColumnCount(0);

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

void MainWindow::stopWaiting()
{
    setWaiting(false);
}

void MainWindow::showMessage(QString m, int i)
{
    #ifdef QT_DEBUG
        qDebug() << m;
    #endif
    mainStatusBar->showMessage(m,i);
    helpDialog->showDebug(m);
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

        actionLogout->setIcon(QIcon(":/icons/logout"));

        //load everything
        dbi->getStatuses();
        dbi->getStages();
        dbi->getProjects();

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

void MainWindow::dataReceived(QJsonObject data)
{
    //extract data
    QString message = data.value("message").toString();
    QString type = data.value("type").toString();
    bool accepted = data.value("accepted").toBool();
    bool success = data.value("success").toBool();
    QJsonValue content = data.value("content");

    //show feedback
    showMessage(message);

    //if not accepted, set to offline
    if (!accepted)
    {
        if (message == "") message = "The server could not compute the request.";
        connected(false,message);
        return;
    }

    //check type and compute

    // LOGIN
    if (type == "login")
    {
        connected(success,message);
        return;
    }

    // STATUSES
    else if (type == "addStatus")
    {
        if (!success) connected(false,message);
        return;
    }
    else if (type == "getStatuses")
    {
        if (success) gotStatuses(content);
        else showMessage("Warning: Status list was not correctly updated from remote server.");
        return;
    }
    else if (type == "updateStatus")
    {
        if (!success) connected(false,message);
        return;
    }
    else if (type == "removeStatus")
    {
        if (!success) connected(false,message);
        return;
    }

    // STAGES
    else if (type == "addStage")
    {
        if (!success) connected(false,message);
        return;
    }
    else if (type == "getStages")
    {
        if (success) gotStages(content);
        else showMessage("Warning: Stages list was not correctly updated from remote server.");
        return;
    }
    else if (type == "updateStage")
    {
        if (!success) connected(false,message);
        return;
    }
    else if (type == "removeStage")
    {
        if (!success) connected(false,message);
        return;
    }

    // PROJECTS
    else if (type == "addProject")
    {
        if (!success) connected(false,message);
        return;
    }
    else if (type == "getProjects")
    {
        if (success) gotProjects(content);
        else showMessage("Warning: Projects list was not correctly updated from remote server.");
        return;
    }
    else if (type == "updateProject")
    {
        if (!success) connected(false,message);
        return;
    }
    else if (type == "removeProject")
    {
        if (!success) connected(false,message);
        return;
    }
    else if (type == "addProjectStage")
    {
        if (!success) connected(false,message);
        return;
    }
    else if (type == "removeProjectStage")
    {
        if (!success) connected(false,message);
        return;
    }

    //SHOTS
    else if (type == "addShots")
    {
        if (!success) connected(false,message);
        return;
    }
    else if (type == "getShots")
    {
        if (success) gotShots(content);
        else showMessage("Warning: Shots list was not correctly updated from remote server.");
        return;
    }
    else if (type == "updateShot")
    {
        if (!success) connected(false,message);
        return;
    }
    else if (type == "removeShots")
    {
        if (!success) connected(false,message);
        return;
    }
    else if (type == "resetShotsOrder")
    {
        if (!success) connected(false,message);
        return;
    }
    else if (type == "moveShotsUp")
    {
        if (!success) connected(false,message);
        return;
    }
    else if (type == "moveShotsDown")
    {
        if (!success) connected(false,message);
        return;
    }


    //ASSETS
    else if (type == "addAsset")
    {
        if (!success) connected(false,message);
        return;
    }
    else if (type == "getAssets")
    {
        if (success) gotAssets(content);
        else showMessage("Warning: Assets list was not correctly updated from remote server.");
        return;
    }
    else if (type == "assignAsset")
    {
        if (!success) connected(false,message);
        return;
    }
    else if (type == "setAssetStatus")
    {
        if (!success) connected(false,message);
        return;
    }

    // If the data was not handled, just display message
    if (message != "") showMessage(message);
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

//SELECTORS

void MainWindow::selectorProjectChanged(int i)
{
    if (freezeSelectors) return;

    stageSelector->clear();
    mainTable->clear();
    mainTable->setRowCount(0);
    mainTable->setColumnCount(0);

    qDeleteAll(allShots);
    allShots.clear();
    shotsAdminList->clear();


    if (i<0) return;

    //set current project
    currentProject = getProject(projectSelector->itemData(i).toInt());

    //get stages
    QList<RAMStage*> stages;
    stages = currentProject->getStages();
    for (int i = 0 ; i < stages.count() ; i++)
    {
        RAMStage *rs = stages[i];
        stageSelector->addItem(rs->getShortName(),rs->getId());
        QTableWidgetItem *item = new QTableWidgetItem(rs->getShortName());
        item->setToolTip(rs->getName());
        item->setData(Qt::UserRole,rs->getId());
        mainTable->setColumnCount(i+1);
        mainTable->setHorizontalHeaderItem(i,item);
    }

    //get shots
    dbi->getShots(currentProject->getId());

    //get assets
    dbi->getAssets(currentProject->getId());
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
    projectsAdminReset();
    shotsAdminReset();
}

//ADMIN - STATUS

void MainWindow::on_addStatusButton_clicked()
{
    // Create a new Default Status
    QString name = "New Status";
    QString shortName = "New";
    QColor color = QColor("#6d6d6d");
    QString description = "";

    // find higher id
    int id = 1;
    foreach(RAMStatus *rs,statusesList)
    {
        if (rs->getId() >= id) id = rs->getId()+1;
    }

    RAMStatus *rs = new RAMStatus(dbi,id,name,shortName,color,description,true);
    newStatus(rs);

    // Select item
    statusAdminList->setCurrentRow(statusAdminList->count()-1);
    on_statusAdminList_itemClicked(statusAdminList->item(statusAdminList->count()-1));
}

void MainWindow::newStatus(RAMStatus *rs)
{
    // Add the status to the list and the UI
    statusesList << rs;
    // Create UI item
    QListWidgetItem *item = new QListWidgetItem(rs->getShortName() + " | " + rs->getName());
    item->setBackgroundColor(rs->getColor());
    item->setToolTip(rs->getDescription());
    statusAdminList->addItem(item);
}

RAMStatus* MainWindow::getStatus(int id)
{
    foreach(RAMStatus *rs,statusesList)
    {
        if (rs->getId() == id) return rs;
    }
    return 0;
}

void MainWindow::gotStatuses(QJsonValue statuses)
{
    setWaiting(true);

    QJsonArray statusesArray = statuses.toArray();

    // update statuses in the current list
    for (int rsI = 0 ; rsI < statusesList.count() ; rsI++)
    {
        RAMStatus *rs = statusesList[rsI];

        //search for status in new list
        bool updated = false;
        for(int i = 0 ; i < statusesArray.count();i++)
        {
            //new status
            QJsonObject status = statusesArray[i].toObject();
            QString name = status.value("name").toString();
            QString shortName = status.value("shortName").toString();
            QColor color(status.value("color").toString());
            QString description = status.value("description").toString();
            int id = status.value("id").toInt();

            if (rs->getId() == id)
            {
                //update
                rs->setName(name);
                rs->setShortName(shortName);
                rs->setColor(color);
                rs->setDescription(description);
                QListWidgetItem *item = statusAdminList->item(rsI);
                item->setText(shortName + " | " + name);
                item->setToolTip(description);
                item->setBackgroundColor(color);

                //remove from the new list
                statusesArray.removeAt(i);
                i--;
            }
        }
        // if the status is not in the new list, remove it
        if (!updated)
        {
            statusesList.removeAt(rsI);
            QListWidgetItem *item = statusAdminList->takeItem(rsI);
            delete item;
            rsI--;
        }
    }

    //add the remaining new statuses
    for (int i = 0 ; i < statusesArray.count() ; i++)
    {
        //new status
        QJsonObject status = statusesArray[i].toObject();
        QString name = status.value("name").toString();
        QString shortName = status.value("shortName").toString();
        QColor color(status.value("color").toString());
        QString description = status.value("description").toString();
        int id = status.value("id").toInt();

        //add to UI
        RAMStatus *rs = new RAMStatus(dbi,id,name,shortName,color,description,false);
        newStatus(rs);
    }

    setWaiting(false);
}

void MainWindow::on_statusColorButton_clicked()
{
    this->setEnabled(false);
    QColorDialog cd(QColor("#" + statusColorEdit->text()));
    cd.setOptions(QColorDialog::DontUseNativeDialog);
    cd.setWindowFlags(Qt::FramelessWindowHint);
    cd.move(this->geometry().center().x()-cd.geometry().width()/2,this->geometry().center().y()-cd.geometry().height()/2);
    if (cd.exec())
    {
        QColor color = cd.selectedColor();
        QString colorHex = color.name();
        statusColorEdit->setText(colorHex.right(6));
    }
    this->setEnabled(true);
}

void MainWindow::on_statusAdminList_itemClicked(QListWidgetItem *i)
{
    int currentRow = statusAdminList->currentRow();

    RAMStatus *s = statusesList[currentRow];

    statusNameEdit->setText(s->getName());
    statusShortNameEdit->setText(s->getShortName());
    if (s->getShortName() == "STB" || s->getShortName() == "OK" || s->getShortName() == "TODO")
    {
        statusShortNameEdit->setEnabled(false);
    }
    else
    {
        statusShortNameEdit->setEnabled(true);
    }
    statusColorEdit->setText(s->getColor().name().right(6));
    statusDescriptionEdit->setPlainText(s->getDescription());

    statusConfigWidget->setEnabled(true);
}

void MainWindow::on_statusApplyButton_clicked()
{
    int currentRow = statusAdminList->currentRow();
    if (currentRow < 0) return;

    QString name = statusNameEdit->text();
    QString shortName = statusShortNameEdit->text();
    QString description = statusDescriptionEdit->toPlainText();
    QColor color("#" + statusColorEdit->text());

    RAMStatus *rs = statusesList[currentRow];
    rs->setColor(color);
    rs->setName(name);
    rs->setShortName(shortName);
    rs->setDescription(description);
    rs->update();
    //update UI
    QListWidgetItem *item = statusAdminList->item(currentRow);
    item->setText(shortName + " | " + name);
    item->setBackgroundColor(color);
    item->setToolTip(description);
}

void MainWindow::on_removeStatusButton_clicked()
{
    int currentRow = statusAdminList->currentRow();
    if (currentRow < 0) return;

    QString sN = statusesList[currentRow]->getShortName();
    if (sN == "OK" || sN == "STB" || sN == "TODO")
    {
        showMessage("Cannot remove OK, STB and TODO statuses.");
        return;
    }

    statusesList[currentRow]->remove();
    statusesList.removeAt(currentRow);
    QListWidgetItem *item = statusAdminList->takeItem(currentRow);
    delete item;

    statusesAdminReset();
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

void MainWindow::on_addStageButton_clicked()
{
    // Create a new default stage
    QString name = "New Stage";
    QString shortName = "New";

    // find higher id
    int id = 1;
    foreach(RAMStage *rs,stagesList)
    {
        if (rs->getId() >= id) id = rs->getId()+1;
    }

    RAMStage *rs = new RAMStage(dbi,name,shortName,id,true);
    newStage(rs);

    //select item
    stagesAdminList->setCurrentRow(stagesAdminList->count()-1);
    on_stagesAdminList_itemClicked(stagesAdminList->item(stagesAdminList->count()-1));
}

void MainWindow::newStage(RAMStage *rs)
{
    stagesList << rs;
    // Create UI item
    QListWidgetItem *item = new QListWidgetItem(rs->getShortName() + " | " + rs->getName());
    stagesAdminList->addItem(item);
}

RAMStage* MainWindow::getStage(int id)
{
    foreach(RAMStage *rs,stagesList)
    {
        if (rs->getId() == id) return rs;
    }
    return 0;
}

void MainWindow::gotStages(QJsonValue stages)
{
    setWaiting(true);

    QJsonArray stagesArray = stages.toArray();

    // update statuses in the current list
    for (int rsI = 0 ; rsI < stagesList.count() ; rsI++)
    {
        RAMStage *rs = stagesList[rsI];

        //search for stage in new list
        bool updated = false;
        for(int i = 0 ; i < stagesArray.count();i++)
        {
            //new status
            QJsonObject stage = stagesArray[i].toObject();
            QString name = stage.value("name").toString();
            QString shortName = stage.value("shortName").toString();
            int id = stage.value("id").toInt();

            if (rs->getId() == id)
            {
                //update
                rs->setName(name);
                rs->setShortName(shortName);
                QListWidgetItem *item = stagesAdminList->item(rsI);
                item->setText(shortName + " | " + name);
                //remove from the new list
                stagesArray.removeAt(i);
                i--;
            }
        }
        // if the stage is not in the new list, remove it
        if (!updated)
        {
            stagesList.removeAt(rsI);
            QListWidgetItem *item = stagesAdminList->takeItem(rsI);
            delete item;
            rsI--;
        }
    }

    //add the remaining new stages
    for (int i = 0 ; i < stagesArray.count() ; i++)
    {
        //new stage
        QJsonObject stage = stagesArray[i].toObject();
        QString name = stage.value("name").toString();
        QString shortName = stage.value("shortName").toString();
        int id = stage.value("id").toInt();

        //add to UI
        RAMStage *rs = new RAMStage(dbi,name,shortName,id,false);
        newStage(rs);
    }
    setWaiting(false);
}

void MainWindow::on_stagesAdminList_itemClicked(QListWidgetItem *item)
{
    int currentRow = stagesAdminList->currentRow();

     RAMStage *s = stagesList[currentRow];

     stageNameEdit->setText(s->getName());
     stageShortNameEdit->setText(s->getShortName()); 
     stageConfigWidget->setEnabled(true);
}

void MainWindow::on_stageApplyButton_clicked()
{
    int currentRow = stagesAdminList->currentRow();
    if (currentRow < 0) return;

    RAMStage *s = stagesList[currentRow];

    QString name = stageNameEdit->text();
    QString shortName = stageShortNameEdit->text();

    s->setName(name);
    s->setShortName(shortName);
    s->update();

    //update UI
    QListWidgetItem *item = stagesAdminList->item(currentRow);
    item->setText(shortName + " | " + name);
}

void MainWindow::on_removeStageButton_clicked()
{
    int currentRow = stagesAdminList->currentRow();
    if (currentRow < 0) return;

    stagesList[currentRow]->remove();
    stagesList.removeAt(currentRow);
    QListWidgetItem *item = stagesAdminList->takeItem(currentRow);
    delete item;

    stagesAdminReset();
}

void MainWindow::stagesAdminReset()
{
    stagesAdminList->setCurrentRow(-1);
    stageNameEdit->setText("");
    stageShortNameEdit->setText("");
    stageConfigWidget->setEnabled(false);
}

//ADMIN - PROJECTS

void MainWindow::on_addProjectButton_clicked()
{
    // Create a new default project
    QString name = "New Project";
    QString shortName = "New";

    // Find the biggest id
    int id = 1;
    foreach(RAMProject *rp,projectsList)
    {
        if (rp->getId() >= id) id = rp->getId()+1;
    }

    RAMProject *rp = new RAMProject(dbi,id,name,shortName,true);
    newProject(rp);

    //select item
    projectAdminList->setCurrentRow(projectAdminList->count()-1);
    on_projectAdminList_itemClicked(projectAdminList->item(projectAdminList->count()-1));
}

void MainWindow::newProject(RAMProject *rp)
{
    projectsList << rp;
    // Create UI item
    QListWidgetItem *item = new QListWidgetItem(rp->getShortName() + " | " + rp->getName());
    item->setData(Qt::UserRole,rp->getId());
    projectAdminList->addItem(item);
    // Add to selector
    projectSelector->addItem(rp->getShortName(),rp->getId());
}

RAMProject* MainWindow::getProject(int id)
{
    foreach(RAMProject *rp,projectsList)
    {
        if (rp->getId() == id) return rp;
    }
}

void MainWindow::gotProjects(QJsonValue projects)
{
    setWaiting(true);

    QJsonArray projectsArray = projects.toArray();

    freezeSelectors = true;

    // update projects in the current list
    for (int rpI = 0 ; rpI < projectsList.count() ; rpI++)
    {
        RAMProject *rp = projectsList[rpI];

        //search for project in new list
        bool updated = false;
        for(int i = 0 ; i < projectsArray.count();i++)
        {
            //new project
            QJsonObject project = projectsArray[i].toObject();
            QString name = project.value("name").toString();
            QString shortName = project.value("shortName").toString();
            int id = project.value("id").toInt();
            QJsonArray projectStagesArray = project.value("stages").toArray();

            if (rp->getId() == id)
            {
                //update project
                rp->setName(name);
                rp->setShortName(shortName);
                QListWidgetItem *item = projectAdminList->item(rpI);
                item->setText(shortName + " | " + name);

                //update stages list
                foreach(QJsonValue proStage,projectStagesArray)
                {
                    RAMStage *stage = getStage(proStage.toInt());
                    rp->addStage(stage);
                }

                //remove from the new list
                projectsArray.removeAt(i);
                i--;
            }
        }
        // if the project is not in the new list, remove it
        if (!updated)
        {
            projectsList.removeAt(rpI);
            QListWidgetItem *item = projectAdminList->takeItem(rpI);
            delete item;
            rpI--;
        }
    }

    //add the remaining new projects
    for (int i = 0 ; i < projectsArray.count() ; i++)
    {
        //new project
        QJsonObject project = projectsArray[i].toObject();
        QString name = project.value("name").toString();
        QString shortName = project.value("shortName").toString();
        int id = project.value("id").toInt();
        QJsonArray projectStagesArray = project.value("stages").toArray();

        RAMProject *rp = new RAMProject(dbi,id,name,shortName,false);

        //update stages list
        foreach(QJsonValue proStage,projectStagesArray)
        {
            RAMStage *stage = getStage(proStage.toInt());
            rp->addStage(stage);
        }

        //add to UI
        newProject(rp);
    }

    freezeSelectors = false;

    if (projectSelector->count() > 0) selectorProjectChanged(0);

    setWaiting(false);
}

void MainWindow::on_projectAdminList_itemClicked(QListWidgetItem *item)
{
    int currentRow = projectAdminList->currentRow();

     RAMProject *p = projectsList[currentRow];

     projectNameEdit->setText(p->getName());
     projectShortNameEdit->setText(p->getShortName());

     //populate stages combo box and list
     projectStagesList->clear();
     projectStagesComboBox->clear();
     QList<RAMStage*> pStages = p->getStages();

     foreach(RAMStage*s,stagesList)
     {
         //check if it is used
         bool usedByProject = false;
         foreach(RAMStage *ps,pStages)
         {
             if (ps == s)
             {
                 //add to list
                 QListWidgetItem *i = new QListWidgetItem(s->getShortName());
                 i->setToolTip(s->getName());
                 i->setData(Qt::UserRole,s->getId());
                 projectStagesList->addItem(i);
                 usedByProject = true;
                 break;
             }
         }
         //add to combo box
         if (!usedByProject)
         {
            projectStagesComboBox->addItem(s->getShortName(),s->getId());
         }
     }

     projectConfigWidget->setEnabled(true);
}

void MainWindow::on_projectApplyButton_clicked()
{
    int currentRow = projectAdminList->currentRow();
    if (currentRow < 0) return;

    RAMProject *rp = getProject(projectAdminList->currentItem()->data(Qt::UserRole).toInt());

    QString name = projectNameEdit->text();
    QString shortName = projectShortNameEdit->text();

    rp->setName(name);
    rp->setShortName(shortName);
    rp->update();

    //update UI
    QListWidgetItem *item = projectAdminList->item(currentRow);
    item->setText(shortName + " | " + name);
    projectSelector->setItemText(currentRow,shortName);
}

void MainWindow::on_removeProjectButton_clicked()
{
    int currentRow = projectAdminList->currentRow();
    if (currentRow < 0) return;

    RAMProject *rp = getProject(projectAdminList->currentItem()->data(Qt::UserRole).toInt());

    rp->remove();
    projectsList.removeAll(rp);
    QListWidgetItem *item = projectAdminList->takeItem(currentRow);
    delete item;
    projectSelector->removeItem(currentRow);

    projectsAdminReset();
}

//project stages
void MainWindow::on_projectAddStageButton_clicked()
{
    int sId = projectStagesComboBox->currentData().toInt();
    int pId = projectAdminList->currentItem()->data(Qt::UserRole).toInt();
    RAMProject *rp = getProject(pId);
    RAMStage *rs = getStage(sId);
    rp->addStage(rs,true);

    //update UI
    //remove from combobox
    projectStagesComboBox->removeItem(projectStagesComboBox->currentIndex());
    //add to list
    QListWidgetItem *i = new QListWidgetItem(rs->getShortName());
    i->setToolTip(rs->getName());
    i->setData(Qt::UserRole,rs->getId());
    projectStagesList->addItem(i);
}

void MainWindow::on_removeStageProjectButton_clicked()
{
    if (projectStagesList->currentRow() < 0) return;

    RAMStage *rs = getStage(projectStagesList->currentIndex().data(Qt::UserRole).toInt());
    RAMProject *rp = getProject(projectAdminList->currentItem()->data(Qt::UserRole).toInt());

    rp->removeStage(rs,true);

    //update UI
    //remove from list
    QListWidgetItem *item = projectStagesList->takeItem(projectStagesList->currentRow());
    delete item;
    //add to combobox
    projectStagesComboBox->addItem(rs->getShortName(),rs->getId());
}

void MainWindow::projectsAdminReset()
{
    projectAdminList->setCurrentRow(-1);
    projectNameEdit->setText("");
    projectShortNameEdit->setText("");
    projectConfigWidget->setEnabled(false);
    projectStagesList->clear();
    //reload stages into combo box
    projectStagesComboBox->clear();
    projectStagesList->clear();
}

//ADMIN - SHOTS

void MainWindow::on_addShotButton_clicked()
{
    //only if new
    bool ok = true;
    int row = 0;
    foreach(RAMShot *rs,allShots)
    {
        if (rs->getName() == "000")
        {
            ok = false;
            row = allShots.indexOf(rs);
        }
    }

    if (ok)
    {
        if (shotsAdminList->currentItem())
        {
            row = shotsAdminList->currentRow() + 1;
        }
        else if (shotsAdminList->count() > 0)
        {
            row = shotsAdminList->count();
        }

        //getProject
        int projectId = projectSelector->currentData().toInt();


        //get higher id, prefix with project id
        int id = 1;
        foreach(RAMShot *rs,allShots)
        {
            if (rs->getId() >= id) id = rs->getId()+1;
        }
        QString idString = QString::number(currentProject->getId()) + QString::number(id);
        id = idString.toInt();

        RAMShot *rs = new RAMShot(dbi,projectId,id,"000",0.0,true);

        //update UI
        newShot(rs,row);
    }


    //select shot
    shotsAdminList->clearSelection();
    shotsAdminList->setCurrentRow(row);
    on_shotsAdminList_itemClicked(shotsAdminList->item(row));

    resetShotsOrder();
}

void MainWindow::newShot(RAMShot *rs,int row)
{
    allShots.insert(row,rs);

    //update list
    QListWidgetItem *item = new QListWidgetItem(rs->getName());
    item->setData(Qt::UserRole,rs->getId());
    shotsAdminList->insertItem(row,item);

    //add to table
    QTableWidgetItem *rowHeader = new QTableWidgetItem(rs->getName());
    rowHeader->setToolTip(QString::number(rs->getDuration()) + "s");
    rowHeader->setData(Qt::UserRole,rs->getId());
    mainTable->insertRow(row);
    mainTable->setVerticalHeaderItem(row,rowHeader);

    //add asset widgets
    QList<RAMStage*> stages;
    stages = currentProject->getStages();
    for (int i = 0 ; i < stages.count() ; i++)
    {
        AssetStatusWidget *assetWidget = new AssetStatusWidget(rs,stages[i],statusesList,assetsList,dbi);
        connect(assetWidget,SIGNAL(editing(bool)),this,SLOT(setDisabled(bool)));
        connect(this,SIGNAL(assetsListUpdated(QList<RAMAsset*>)),assetWidget,SLOT(assetsListUpdated(QList<RAMAsset*>)));
        //add widget to cell
        mainTable->setCellWidget(row,i,assetWidget);
    }
}

RAMShot* MainWindow::getShot(int id)
{
    foreach(RAMShot *rs,allShots)
    {
        if (rs->getId() == id) return rs;
    }
    return 0;
}

void MainWindow::gotShots(QJsonValue shots)
{
    setWaiting(true);

    QJsonArray shotsArray = shots.toArray();

    // update shots in the current list
    for (int rsI = 0 ; rsI < allShots.count() ; rsI++)
    {
        RAMShot *rs = allShots[rsI];

        //search for shot in new list
        bool updated = false;
        for(int i = 0 ; i < shotsArray.count();i++)
        {
            //new shot
            QJsonObject shot = shotsArray[i].toObject();
            QString name = shot.value("shotName").toString();
            int id = shot.value("shotId").toInt();

            if (rs->getId() == id)
            {
                //update shot
                rs->setName(name);
                QListWidgetItem *item = shotsAdminList->item(rsI);
                item->setText(name);
                item->setData(Qt::UserRole,rs->getId());

                QTableWidgetItem *rowHeader = new QTableWidgetItem(rs->getName());
                rowHeader->setToolTip(QString::number(rs->getDuration()) + "s");
                rowHeader->setData(Qt::UserRole,rs->getId());
                mainTable->setVerticalHeaderItem(rsI,rowHeader);

                //remove from the new list
                shotsArray.removeAt(i);
                i--;
            }
        }
        // if the shot is not in the new list, remove it
        if (!updated)
        {
            allShots.removeAt(rsI);
            QListWidgetItem *item = shotsAdminList->takeItem(rsI);
            delete item;
            mainTable->removeRow(rsI);
            rsI--;
        }
    }

    //add the remaining new shots
    for (int i = 0 ; i < shotsArray.count() ; i++)
    {
        //new shot
        QJsonObject shot = shotsArray[i].toObject();
        QString name = shot.value("shotName").toString();
        double duration = shot.value("duration").toDouble();
        int order = shot.value("shotOrder").toInt();
        int id = shot.value("shotId").toInt();

        RAMShot *rs = new RAMShot(dbi,currentProject->getId(),id,name,duration,false);

        //add to UI
        newShot(rs,order);
    }


    setWaiting(false);

    /*setWaiting(false);
    if (!success) return;

    showMessage("Loading shots...");

    //clear UI and stored list
    shotsAdminList->clear();
    qDeleteAll(assetsList);
    assetsList.clear();
    mainTable->clearContents();
    mainTable->setRowCount(0);

    QJsonArray shotsJsonArray = shots.toArray();

    //LOAD DATA
    foreach (QJsonValue shotJson, shotsJsonArray)
    {
        //get values
        QJsonObject shot = shotJson.toObject();
        QString shotName = shot.value("shotName").toString();
        double shotDuration = shot.value("duration").toDouble();
        QString shotComment = shot.value("comment").toString();
        int shotStageId = shot.value("stageId").toInt();
        int shotStatusId = shot.value("statusId").toInt();
        int shotId = shot.value("shotId").toInt();
        int shotOrder = shot.value("shotOrder").toInt();

        RAMShot *ramShot;
        //if shot is not already existing, create it
        bool found = false;
        foreach(RAMShot *testShot,shotsList)
        {
            if (testShot->getId() == shotId)
            {
                found = true;
                ramShot = testShot;
                break;
            }
        }

        if (!found)
        {
            ramShot = new RAMShot(dbi,shotId,shotName,shotDuration,shotOrder);
            shotsList << ramShot;
            //connections
            connect(ramShot,SIGNAL(stageStatusUpdated(RAMStatus*,RAMStage*,RAMShot*)),this,SLOT(updateStageStatus(RAMStatus*,RAMStage*,RAMShot*)));
            connect(ramShot,SIGNAL(assetAdded(RAMAsset*,RAMShot*)),this,SLOT(assetAssigned(RAMAsset*,RAMShot*)));
            connect(ramShot,SIGNAL(statusAdded(RAMStageStatus*,RAMShot*)),this,SLOT(shotStatusAdded(RAMStageStatus*,RAMShot*)));

            //Add to maintable

            //create Table row
            QTableWidgetItem *rowHeader = new QTableWidgetItem(shotName);
            rowHeader->setToolTip(QString::number(shotDuration) + "s");
            mainTable->setRowCount(mainTable->rowCount() + 1);
            mainTable->setVerticalHeaderItem(mainTable->rowCount()-1,rowHeader);

            //for each asset stage, add widgets
            for(int i = 0 ; i < stagesList.count() ; i++)
            {
                RAMStage *stage = stagesList[i];
                if (stage->getType() == "a")
                {
                    //create asset widget
                    AssetStatusWidget *assetWidget = new AssetStatusWidget(ramShot,stage,statusesList,assetsList,dbi);
                    connect(assetWidget,SIGNAL(editing(bool)),this,SLOT(setDisabled(bool)));
                    connect(this,SIGNAL(assetsListUpdated(QList<RAMAsset*>)),assetWidget,SLOT(assetsListUpdated(QList<RAMAsset*>)));
                    //add widget to cell
                    mainTable->setCellWidget(mainTable->rowCount()-1,i,assetWidget);
                }

            }

            //Add to admin tab
            shotsAdminList->addItem(ramShot->getName());

        }

        //find stage
        RAMStage *shotStage;
        foreach(RAMStage *stage,stagesList)
        {
            if (stage->getId() == shotStageId)
            {
                shotStage = stage;
                break;
            }
        }

        //find status
        RAMStatus *shotStatus;
        foreach(RAMStatus *status,statusesList)
        {
            if (status->getId() == shotStatusId)
            {
                shotStatus = status;
                break;
            }
        }

        if (shotStage)
        {
            //if shot production stage, add status
            if (shotStage->getType() == "s")
            {
                //add status
                RAMStageStatus *shotStageStatus = new RAMStageStatus(shotStatus,shotStage,shotComment);
                ramShot->addStatus(shotStageStatus,false);
            }
            //if asset production stage, add asset
            else if (shotStage->getType() == "a")
            {
                QString assetName = shot.value("assetName").toString();
                QString assetShortName = shot.value("assetShortName").toString();
                QString assetComment = shot.value("comment").toString();
                int assetId = shot.value("assetId").toInt();
                //check if asset is already created
                bool assetFound = false;
                RAMAsset *shotAsset;
                foreach(RAMAsset *a,assetsList)
                {
                    if (a->getId() == assetId)
                    {
                        shotAsset = a;
                        assetFound = true;
                        break;
                    }
                }
                if (!assetFound)
                {
                    QList<RAMStage*> stages;
                    stages << shotStage;
                    shotAsset = new RAMAsset(assetId, assetName, assetShortName, stages, shotStatus);
                    shotAsset->setComment(assetComment);
                    connect(shotAsset,SIGNAL(statusChanged(RAMAsset *)),this,SLOT(updateAssetStatus(RAMAsset *)));
                    loadAsset(shotAsset);
                }
                ramShot->addAsset(shotAsset,false);
            }
        }
    }

    mainTable->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
    mainTable->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);*/
}

void MainWindow::on_importShotsButton_clicked()
{
    /*this->setEnabled(false);
    QString file = QFileDialog::getOpenFileName(this,"Please choose the editing file you want to import.","","All supported edits (*.edl *.xml);;EDL (*.edl);;Final Cut / XML (*.xml);;All Files (*.*)");
    if (file == "")
    {
        this->setEnabled(true);
        return;
    }

    if (file.toLower().endsWith(".edl")) importEDL(file);
    else if (file.toLower().endsWith(".xml")) importXML(file);
    else
    {
        //TODO open file to try to find what it is
    }
    this->setEnabled(true);*/
}

void MainWindow::importEDL(QString f)
{
    showMessage("Importing EDL " + f + " (not yet implemented)");
}

void MainWindow::importXML(QString f)
{
    /*setWaiting();
    showMessage("Importing XML " + f);

     //open file and load data
    QFile *xmlFile = new QFile(f);
    if (!xmlFile->open(QFile::ReadOnly | QFile::Text))
    {
        showMessage("XML File could not be opened.");
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

void MainWindow::on_batchAddShotButton_clicked()
{
    /*this->setEnabled(false);
    AddShotsDialog as;
    as.move(this->geometry().center().x()-as.geometry().width()/2,this->geometry().center().y()-as.geometry().height()/2);
    if (as.exec())
    {
        QStringList shotNames = as.getShots();
        setWaiting();
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
        dbi->addShots(projectId,statusId,shotNames,order);
    }
    this->setEnabled(true);*/
}

void MainWindow::on_shotsAdminList_itemClicked(QListWidgetItem *item)
{
     RAMShot *rs = getShot(shotsAdminList->currentItem()->data(Qt::UserRole).toInt());

     shotNameEdit->setText(rs->getName());
     shotDurationSpinBox->setValue(rs->getDuration());
     shotConfigWidget->setEnabled(true);
}

void MainWindow::on_shotApplyButton_clicked()
{
    int currentRow = shotsAdminList->currentRow();
    if (currentRow < 0) return;

    RAMShot *rs = getShot(shotsAdminList->currentItem()->data(Qt::UserRole).toInt());

    QString name = shotNameEdit->text();
    double duration = shotDurationSpinBox->value();

    rs->setName(name);
    rs->setDuration(duration);
    rs->update();

    //update UI
    QListWidgetItem *item = shotsAdminList->item(currentRow);
    item->setText(name);

    QTableWidgetItem *header = mainTable->verticalHeaderItem(currentRow);
    header->setText(name);
    header->setToolTip(QString::number(duration) + "s");
}

void MainWindow::on_removeShotButton_clicked()
{
    QList<int> ids;
    foreach(QListWidgetItem *item,shotsAdminList->selectedItems())
    {
        int id = item->data(Qt::UserRole).toInt();
        ids << id;
        RAMShot *rs = getShot(id);

        int row = shotsAdminList->row(item);
        mainTable->removeRow(row);

        allShots.removeAll(rs);
        delete item;
    }

    dbi->removeShots(ids);
    resetShotsOrder();

    shotsAdminReset();
}

void MainWindow::shotsAdminReset()
{
    shotsAdminList->setCurrentRow(-1);
    shotNameEdit->setText("");
    shotDurationSpinBox->setValue(0.0);
    shotConfigWidget->setEnabled(false);
}

void MainWindow::resetShotsOrder()
{
    QList<int> ids;
    foreach(RAMShot *rs,allShots)
    {
        ids << rs->getId();
    }
    dbi->resetShotsOrder(ids);
}

void MainWindow::on_moveShotUpButton_clicked()
{
    QList<QListWidgetItem *> items;

    //sort selection
    QList<int> rows;
    foreach(QListWidgetItem *item,shotsAdminList->selectedItems())
    {
        rows << shotsAdminList->row(item);
    }
    std::sort(rows.begin(),rows.end());

    foreach(int row,rows)
    {
        items << shotsAdminList->item(row);
    }

    foreach(QListWidgetItem *item,items)
    {
        int id = item->data(Qt::UserRole).toInt();

        RAMShot *rs = getShot(id);
        int index = allShots.indexOf(rs)-1;

        allShots.removeAll(rs);
        allShots.insert(index,rs);

        shotsAdminList->takeItem(index+1);
        shotsAdminList->insertItem(index,item);

        mainTable->verticalHeader()->moveSection(index+1,index);
    }

    //reselect items
    foreach(QListWidgetItem *item,items)
    {
        item->setSelected(true);
    }

    //update db
    resetShotsOrder();
}

void MainWindow::on_moveShotDownButton_clicked()
{
    QList<QListWidgetItem *> items;

    //sort selection
    QList<int> rows;
    foreach(QListWidgetItem *item,shotsAdminList->selectedItems())
    {
        rows << shotsAdminList->row(item);
    }
    std::sort(rows.begin(),rows.end());

    for(int i = rows.count() -1 ; i >= 0 ; i--)
    {
        items << shotsAdminList->item(rows[i]);
    }

    foreach(QListWidgetItem *item,items)
    {
        int id = item->data(Qt::UserRole).toInt();

        RAMShot *rs = getShot(id);
        int index = allShots.indexOf(rs)+1;

        allShots.removeAll(rs);
        allShots.insert(index,rs);

        shotsAdminList->takeItem(index-1);
        shotsAdminList->insertItem(index,item);
    }

    //reselect items
    foreach(QListWidgetItem *item,items)
    {
        item->setSelected(true);
    }

    //update db
    resetShotsOrder();
}

//ADMIN - ASSETS

void MainWindow::gotAssets(QJsonValue assets)
{
    setWaiting(true);

    QJsonArray assetsArray = assets.toArray();

    // update assets in the current list
    for (int raI = 0 ; raI < assetsList.count() ; raI++)
    {
        RAMAsset *ra = assetsList[raI];

        //search for asset in new list
        bool updated = false;
        for(int i = 0 ; i < assetsArray.count();i++)
        {
            //new asset
            QJsonObject asset = assetsArray[i].toObject();
            QString name = asset.value("name").toString();
            QString shortName = asset.value("shortName").toString();
            QString comment = asset.value("comment").toString();
            int statusId = asset.value("statusId").toInt();
            QJsonArray assignments = asset.value("assignments").toArray();
            int id = asset.value("id").toInt();

            if (ra->getId() == id)
            {
                //update asset
                ra->setName(name);
                ra->setShortName(shortName);
                ra->setComment(comment);
                ra->setStatus(getStatus(statusId));

                //TODO check assignments

                //remove from the new list
                assetsArray.removeAt(i);
                i--;
            }
        }
        // if the asset is not in the new list, remove it
        if (!updated)
        {
            assetsList.removeAt(raI);
            raI--;
        }
    }

    //add the remaining new assets
    for (int i = 0 ; i < assetsArray.count() ; i++)
    {
        //new asset
        QJsonObject asset = assetsArray[i].toObject();
        QString name = asset.value("name").toString();
        QString shortName = asset.value("shortName").toString();
        QString comment = asset.value("comment").toString();
        int statusId = asset.value("statusId").toInt();
        QJsonArray assignments = asset.value("assignments").toArray();
        int id = asset.value("id").toInt();

        RAMAsset *ra = new RAMAsset(dbi,id,name,shortName,getStatus(statusId),comment,false);
        for (int j = 0;j<assignments.count() ; j++)
        {
            QJsonObject assignment = assignments[j].toObject();
            int stageId = assignment.value("stageId").toInt();
            int shotId = assignment.value("shotId").toInt();
            ra->assign(getStage(stageId),getShot(shotId));
        }

        //add to UI
        newAsset(ra);
    }


    setWaiting(false);
}

void MainWindow::newAsset(RAMAsset *asset)
{
    assetsList << asset;
    emit assetsListUpdated(assetsList);
    foreach(RAMAssignment assignment,asset->getAssignments())
    {
        RAMShot *shot = assignment.getShot();
        RAMStage *stage = assignment.getStage();

        //find row
        int row = -1;
        for (int i = 0;i < allShots.count();i++)
        {
            if (shot == allShots[i])
            {
                row = i;
                break;
            }
        }

        if (row < 0) return;

        //find column
        int col = -1;
        for (int i = 0;i < currentProject->getStages().count();i++)
        {
            if (stage == currentProject->getStages()[i])
            {
                col = i;
                break;
            }
        }

        if (col < 0) return;

        //add widget
        AssetStatusWidget* aw = (AssetStatusWidget*)mainTable->cellWidget(row,col);
        aw->addAsset(asset);
    }
}

//asset statuses
void MainWindow::updateAssetStatus(RAMAsset *asset)
{
    showMessage("Updateing status of the asset " + asset->getName() + " to " + asset->getStatus()->getShortName());
    dbi->setAssetStatus(asset->getStatus()->getId(),asset->getId());
}

void MainWindow::assetStatusUpdated(bool success,QString message)
{
    if (!success) connected(false,message);
}

void MainWindow::assetAssigned(bool success,QString message)
{
    setWaiting(false);
    //refresh shots list
}

void MainWindow::assetAssigned(RAMAsset *a,RAMShot *s)
{/*
    //update UI
    //find row
    int row = -1;
    for(int i = 0 ; i < shotsList.count() ; i++)
    {
        if (shotsList[i] == s)
        {
            row = i;
            break;
        }
    }

    if (row < 0) return;

    //find column
    int col = -1;
    for(int i = 0 ; i < stagesList.count() ; i++)
    {
        if (a->getStages()[0] == stagesList[i]) //TODO stages list
        {
            col = i;
            break;
        }
    }

    if (col < 0) return;


    AssetStatusWidget* aw = (AssetStatusWidget*)mainTable->cellWidget(row,col);
    aw->addAsset(a);

    mainTable->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
    mainTable->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);*/
}

void MainWindow::loadAsset(RAMAsset *a)
{
    assetsList.append(a);
    emit assetsListUpdated(assetsList);
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

// ============ WINDOW BUTTONS ======

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

