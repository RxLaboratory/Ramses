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


    // Get the desktop (to manage windows locations)
    desktop = qApp->desktop();

    // Help Dialog
    helpDialog = new HelpDialog(this);
    helpDialogDocked = true;

    // Feedback
    showMessage("Let's start!");

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
    projectSelector = new ProjectSelectorWidget(this);
    mainToolBar->insertWidget(actionSettings,projectSelector);

    //Add window buttons
    maximizeButton = new QPushButton(QIcon(":/icons/maximize"),"",this);
    minimizeButton = new QPushButton(QIcon(":/icons/minimize"),"",this);
    quitButton = new QPushButton(QIcon(":/icons/close"),"",this);
    mainToolBar->addWidget(minimizeButton);
    mainToolBar->addWidget(maximizeButton);
    mainToolBar->addWidget(quitButton);

    //Add admin
    adminWidget = new AdminWidget(dbi,this);
    adminPageLayout->addWidget(adminWidget);

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

    showMessage("Loading settings");
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

    //Connections
    mapEvents();

    showMessage("Ready!");
}

// ========= GENERAL METHODS ========

void MainWindow::mapEvents()
{
    // general
    connect(qApp,SIGNAL(aboutToQuit()),this,SLOT(logout()));
    connect(qApp,SIGNAL(idle()),this,SLOT(idle()));

    // helpDialog
    connect(helpDialog,SIGNAL(dock(bool)),this,SLOT(dockHelpDialog(bool)));
    connect(helpDialog,SIGNAL(visibilityChanged(bool)),actionHelp,SLOT(setChecked(bool)));

    // project selector
    connect(projectSelector,SIGNAL(currentProjectChanged(RAMProject*)),this,SLOT(currentProjectChanged(RAMProject*)));

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

    // ADMIN
    connect(adminWidget,SIGNAL(statusCreated(RAMStatus*)),this,SLOT(newStatus(RAMStatus*)));
    connect(adminWidget,SIGNAL(stageCreated(RAMStage*)),this,SLOT(newStage(RAMStage*)));
    connect(adminWidget,SIGNAL(projectCreated(RAMProject*)),this,SLOT(newProject(RAMProject*)));
    connect(adminWidget,SIGNAL(shotCreated(RAMShot*,int)),this,SLOT(newShot(RAMShot*,int)));

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

    clean();

    showPage(0);

    showMessage("Logged out.");
}

void MainWindow::clean()
{
    mainTable->clearContents();
    mainTable->setRowCount(0);
    mainTable->setColumnCount(1);
    projectSelector->clear();

    //empty everything

    qDeleteAll(assetsList);
    assetsList.clear();

    qDeleteAll(allShots);
    allShots.clear();

    qDeleteAll(projectsList);
    projectsList.clear();

    qDeleteAll(stagesList);
    stagesList.clear();

    qDeleteAll(statusList);
    statusList.clear();

    qDeleteAll(removedItems);
    removedItems.clear();
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
        adminWidget->init(statusList,stagesList,projectsList,allShots,currentProject);
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
    //mainStatusBar->showMessage(m,i);
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
        if (success)
        {
            gotStatuses(content);
        }
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
        if (success)
        {
            gotStages(content);
        }
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
        if (success)
        {
            gotProjects(content);
        }
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

void MainWindow::currentProjectChanged(RAMProject *p)
{
    mainTable->clearContents();
    mainTable->setRowCount(0);
    mainTable->setColumnCount(1);

    qDeleteAll(allShots);
    allShots.clear();

    qDeleteAll(assetsList);
    assetsList.clear();

    if (p == 0) return;

    //set current project
    currentProject = p;
    adminWidget->setCurrentProject(currentProject);

    //set current stages
    QList<RAMStage*> stages;
    stages = currentProject->getStages();
    for (int i = 0 ; i < stages.count() ; i++)
    {
        RAMStage *rs = stages[i];
        QTableWidgetItem *item = new QTableWidgetItem(rs->getShortName());
        item->setToolTip(rs->getName());
        item->setData(Qt::UserRole,rs->getId());
        mainTable->setColumnCount(i+2);
        mainTable->setHorizontalHeaderItem(i+1,item);
    }

    //get shots
    dbi->getShots(currentProject->getId());
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

// STATUS

void MainWindow::newStatus(RAMStatus *rs)
{
    // Add the status to the list and the UI
    statusList << rs;
    rs->setParent(this);

    // connect the status
    connect(rs,SIGNAL(statusRemoved(RAMStatus*)),this,SLOT(removeStatus(RAMStatus*)));
}

void MainWindow::removeStatus(RAMStatus *rs)
{
    statusList.removeAll(rs);
    removedItems << rs;
}

RAMStatus* MainWindow::getStatus(int id)
{
    foreach(RAMStatus *rs,statusList)
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
    for (int rsI = 0 ; rsI < statusList.count() ; rsI++)
    {
        RAMStatus *rs = statusList[rsI];

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

                //remove from the new list
                statusesArray.removeAt(i);
                i--;
            }
        }
        // if the status is not in the new list, remove it
        if (!updated)
        {
            removedItems << statusList.takeAt(rsI);
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

    //get Stages
    dbi->getStages();
}

//ADMIN - STAGES

void MainWindow::newStage(RAMStage *rs)
{
    stagesList << rs;
    rs->setParent(this);

    // connect the stage
    connect(rs,SIGNAL(stageRemoved(RAMStage*)),this,SLOT(removeStage(RAMStage*)));
}

void MainWindow::removeStage(RAMStage *rs)
{
    stagesList.removeAll(rs);
    removedItems << rs;
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

                //remove from the new list
                stagesArray.removeAt(i);
                i--;
            }
        }
        // if the stage is not in the new list, remove it
        if (!updated)
        {
            removedItems << stagesList.takeAt(rsI);
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

    //get projects
    dbi->getProjects();
}

//ADMIN - PROJECTS

void MainWindow::newProject(RAMProject *rp)
{
    projectsList << rp;
    rp->setParent(this);

    // Add to selector
    projectSelector->addProject(rp);

    // connect the project
    connect(rp,SIGNAL(projectRemoved(RAMProject*)),this,SLOT(removeProject(RAMProject*)));
}

void MainWindow::removeProject(RAMProject* rp)
{
    projectsList.removeAll(rp);
    removedItems << rp;
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
            projectsList[rpI]->remove();
            removedItems << projectsList.takeAt(rpI);
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

    setWaiting(false);

}

//ADMIN - SHOTS

void MainWindow::newShot(RAMShot *rs,int row)
{
    allShots.insert(row,rs);
    rs->setParent(this);

    //add to table
    ShotWidget *widget= new ShotWidget(rs);
    mainTable->insertRow(row);
    mainTable->setCellWidget(row,0,widget);

    //add asset widgets
    QList<RAMStage*> stages;
    stages = currentProject->getStages();
    //get the assets list for this stage
    for (int i = 0 ; i < stages.count() ; i++)
    {
        AssetStatusWidget *assetWidget = new AssetStatusWidget(rs,stages[i],statusList,assetsList,dbi);
        connect(assetWidget,SIGNAL(editing(bool)),this,SLOT(setDisabled(bool)));
        connect(assetWidget,SIGNAL(newAsset(RAMAsset*)),this,SLOT(assetCreated(RAMAsset*)));
        connect(this,SIGNAL(assetsListUpdated(QList<RAMAsset*>)),assetWidget,SLOT(assetsListUpdated(QList<RAMAsset*>)));
        //add widget to cell
        mainTable->setCellWidget(row,i+1,assetWidget);
    }

    // connect the shot
    connect(rs,SIGNAL(shotRemoved(RAMShot*)),this,SLOT(removeShot(RAMShot*)));

}

void MainWindow::removeShot(RAMShot *rs)
{
    allShots.removeAll(rs);
    removedItems << rs;

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
            double duration = shot.value("duration").toDouble();
            int id = shot.value("shotId").toInt();

            if (rs->getId() == id)
            {
                //update shot
                rs->setName(name);
                rs->setDuration(duration);

                //remove from the new list
                shotsArray.removeAt(i);
                i--;
            }
        }
        // if the shot is not in the new list, remove it
        if (!updated)
        {
            removedItems << allShots.takeAt(rsI);
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


    //resize columns
    mainTable->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

    setWaiting(false);

    //get assets
    if (currentProject) dbi->getAssets(currentProject->getId());

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

void MainWindow::resetShotsOrder()
{
    QList<int> ids;
    foreach(RAMShot *rs,allShots)
    {
        ids << rs->getId();
    }
    dbi->resetShotsOrder(ids);
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
        int stageId = asset.value("stageId").toInt();
        QJsonArray assignments = asset.value("assignments").toArray();
        int id = asset.value("id").toInt();

        RAMAsset *testRa = new RAMAsset(dbi,name,shortName,getStatus(statusId),getStage(stageId),false,comment,id);
        RAMAsset *ra;
        //check if asset already exists
        int testI = assetsList.indexOf(testRa);
        if (testI >= 0)
        {
            delete testRa;
            ra = assetsList[testI];
        }
        else
        {
            ra = testRa;
        }
        for (int j = 0;j<assignments.count() ; j++)
        {
            int shotId = assignments[j].toInt();
            ra->assign(getShot(shotId));
        }

        //add to UI
        newAsset(ra);
    }

    //resize columns
    mainTable->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

    setWaiting(false);
}

void MainWindow::newAsset(RAMAsset *asset)
{
    assetCreated(asset);
    RAMStage *stage = asset->getStage();
    foreach(RAMShot *shot,asset->getAssignments())
    {
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
                col = i+1;
                break;
            }
        }

        if (col < 0) return;

        //add widget
        AssetStatusWidget* aw = (AssetStatusWidget*)mainTable->cellWidget(row,col);
        aw->addAsset(asset);
    }
}

void MainWindow::assetCreated(RAMAsset* asset)
{
    assetsList << asset;
    asset->setParent(this);
    emit assetsListUpdated(assetsList);
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

