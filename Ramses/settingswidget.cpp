#include "settingswidget.h"

SettingsWidget::SettingsWidget(DBInterface *db, Updater *up, QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    dbi = db;
    updater = up;

    currentUser = 0;

    error = new QErrorMessage(this);
    error->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);


    //hide settings logout widget
    settingsLogoutWidget->hide();


    //========= LOAD SETTINGS ========

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

    app = (Application *)qApp;

    settingsDB.setDatabaseName(settingsPath);
    settingsDB.setHostName("localhost");
    settingsDB.open();

    //load NETWORK SETTINGS ============================

    QString qNetwork = "SELECT networkSettings.serverAddress, networkSettings.ssl, networkSettings.updateFrequency FROM networkSettings JOIN users ON users.id = networkSettings.userID WHERE users.username = 'Default';";
    QSqlQuery networkSettingsQuery(qNetwork,settingsDB);
    networkSettingsQuery.next();
    //set ui
    serverAddressEdit->setText(networkSettingsQuery.value(0).toString());
    sslCheckBox->setChecked(networkSettingsQuery.value(1).toBool());
    updateFreqSpinBox->setValue(networkSettingsQuery.value(2).toInt());
    //dispatch settings
    dbi->setServerAddress(networkSettingsQuery.value(0).toString());
    dbi->setSsl(networkSettingsQuery.value(1).toBool());
    updater->setUpdateFrequency(networkSettingsQuery.value(2).toInt()*1000*60);

    //load SECURITY SETTINGS ============================

    QString qSecurity = "SELECT securitySettings.timeOut FROM securitySettings JOIN users ON users.id = securitySettings.userId WHERE users.username = 'Default';";
    QSqlQuery securitySettingsQuery(qSecurity,settingsDB);
    securitySettingsQuery.next();
    //set ui
    timeOutEdit->setValue(securitySettingsQuery.value(0).toInt());
    //dispatch settings
    app->setTimeOut(securitySettingsQuery.value(0).toInt()*60*1000);

    //load UI SETTINGS ===================================

    QString qUI = "SELECT uiSettings.cssPath,uiSettings.toolButtonStyle FROM uiSettings JOIN users ON users.id = uiSettings.userId WHERE users.username = 'Default';";
    QSqlQuery uiSettingsQuery(qUI,settingsDB);
    uiSettingsQuery.next();
    //set ui
    QString cssFile = ":/styles/default";
    styleBox->setToolTip("Ramses default style");
    if (uiSettingsQuery.value(0).toString() == ":/styles/default")
    {
        styleBox->setCurrentIndex(0);

    }
    else if (uiSettingsQuery.value(0).toString() == "system")
    {
        styleBox->setCurrentIndex(1);
        cssFile = "";
    }
    else
    {
        styleBox->setCurrentIndex(2);
        cssFile = uiSettingsQuery.value(0).toString();
    }
    int toolButtonStyle = uiSettingsQuery.value(1).toInt();
    toolButtonStyleBox->setCurrentIndex(toolButtonStyle);
    //dispatch settings
#ifdef QT_DEBUG
    cssFile = "E:/DEV SRC/Ramses/Ramses/needed/style.css";
#endif
    updateCSS(cssFile);


}

void SettingsWidget::login(RAMUser *user)
{
    //disable network settings
    serverAddressWidget->setEnabled(false);
    sslCheckBox->setEnabled(false);
    settingsLogoutWidget->show();
    profileWidget->setEnabled(true);
    unameEdit->setText(user->getUsername());
    fnameEdit->setText(user->getFirstname());
    lnameEdit->setText(user->getLastname());
    currentUser = user;
}

void SettingsWidget::logout()
{
    //enable network settings
    serverAddressWidget->setEnabled(true);
    sslCheckBox->setEnabled(true);
    settingsLogoutWidget->hide();
    profileWidget->setEnabled(false);
}

int SettingsWidget::getToolButtonStyle()
{
    return toolButtonStyleBox->currentIndex();
}

void SettingsWidget::updateCSS(QString cssPath)
{
    QFile cssFile(cssPath);
    cssFile.open(QFile::ReadOnly);
    QString css = QString(cssFile.readAll());
    cssFile.close();
    qApp->setStyleSheet(css);
}

void SettingsWidget::on_serverAddressEdit_editingFinished()
{
    QString q = "UPDATE networkSettings ";
    q += "SET serverAddress = '" + serverAddressEdit->text() + "' ";
    q += "WHERE userID = (SELECT id FROM users WHERE username = 'Default');";
    QSqlQuery(q,settingsDB);
    dbi->setServerAddress(serverAddressEdit->text());
}

void SettingsWidget::on_sslCheckBox_clicked(bool checked)
{
    QString q = "UPDATE networkSettings ";
    if (checked) q += "SET ssl = 1 ";
    else q += "SET ssl = 0 ";
    q += "WHERE userID = (SELECT id FROM users WHERE username = 'Default');";
    QSqlQuery(q,settingsDB);
    dbi->setSsl(checked);
}

void SettingsWidget::on_updateFreqSpinBox_editingFinished()
{
    QString q = "UPDATE networkSettings ";
    q += "SET updateFrequency = " + QString::number(updateFreqSpinBox->value()) + " ";
    q += "WHERE userID = (SELECT id FROM users WHERE username = 'Default');";
    QSqlQuery(q,settingsDB);
    updater->setUpdateFrequency(updateFreqSpinBox->value()*1000*60);
}

void SettingsWidget::on_timeOutEdit_editingFinished()
{
    QString q = "UPDATE securitySettings ";
    q += "SET timeout = " + QString::number(timeOutEdit->value()) + " ";
    q += "WHERE userID = (SELECT id FROM users WHERE username = 'Default');";
    QSqlQuery(q,settingsDB);
    app->setTimeOut(timeOutEdit->value()*60*1000);
}

void SettingsWidget::on_settingsLogoutButton_clicked()
{
    logout();
    emit loggedOut();
}

void SettingsWidget::on_updateCSSButton_clicked()
{

}

void SettingsWidget::on_styleBox_currentIndexChanged(int index)
{
    if (index == 0)
    {
        updateCSSButton->setEnabled(false);
        styleBox->setToolTip("Ramses default style");
        updateCSS(":/styles/default");
        //update prefs database
        QSqlQuery("UPDATE uiSettings SET cssPath = ':/styles/default' WHERE userID = (SELECT id FROM users WHERE username = 'Default');",settingsDB);
    }
    else if (index == 1)
    {
        updateCSSButton->setEnabled(false);
        styleBox->setToolTip("Current system style");
        updateCSS("");
        //update prefs database
        QSqlQuery ("UPDATE preferences SET css = 'system' WHERE userID = (SELECT id FROM users WHERE username = 'Default');",settingsDB);
    }
    else if (index == 2)
    {
        updateCSSButton->setEnabled(true);
        //get file
        QString cssFileName = QFileDialog::getOpenFileName(this,"Open a stylesheet file","","CSS (*.css);;Text files (*.txt);;All files (*.*)");

        QFile checkFile(cssFileName);
        if (checkFile.exists()) styleBox->setItemData(2,cssFileName);
        updateCSS(cssFileName);
        styleBox->setToolTip("Your own style");
        //update prefs database
        QSqlQuery("UPDATE preferences SET css = \"" + cssFileName + "\" WHERE userID = (SELECT id FROM users WHERE username = 'Default');",settingsDB);
    }
}

void SettingsWidget::on_toolButtonStyleBox_currentIndexChanged(int index)
{
    QString q = "UPDATE uiSettings ";
    q += "SET toolButtonStyle = " + QString::number(toolButtonStyleBox->currentIndex()) + " ";
    q += "WHERE userID = (SELECT id FROM users WHERE username = 'Default');";
    QSqlQuery(q,settingsDB);
    emit setToolButtonStyle(index);
}

void SettingsWidget::on_profileUpdateButton_clicked()
{
    this->setEnabled(false);
    //check if everything is alright
    if (unameEdit->text() == "")
    {
        error->showMessage("You must choose a user name!");
        this->setEnabled(true);
        return;
    }

    if (npassword1Edit->text() != "")
    {
        if (cpasswordEdit->text() == "")
        {
            error->showMessage("You must specify your current password to be able to modify it.");
            this->setEnabled(true);
            return;
        }
        if (npassword1Edit->text() != npassword2Edit->text())
        {
            QMessageBox warning(QMessageBox::Warning,"Cannot create new password","The two fields for the new password are different.",QMessageBox::Ok,this,Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
            warning.exec();
            this->setEnabled(true);
            return;
        }

        QString currentHash = currentUser->generatePassHash(cpasswordEdit->text());
        if (currentHash != currentUser->getPassHash())
        {
            QMessageBox warning(QMessageBox::Warning,"Cannot create new password","The current password is incorrect.",QMessageBox::Ok,this,Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
            warning.exec();
            this->setEnabled(true);
            return;
        }

        currentUser->setPassword(npassword1Edit->text());
    }

    currentUser->setFirstname(fnameEdit->text());
    currentUser->setLastname(lnameEdit->text());
    currentUser->setUsername(unameEdit->text());
    currentUser->update();

    npassword1Edit->setText("");
    npassword2Edit->setText("");
    cpasswordEdit->setText("");

    QMessageBox info(QMessageBox::Information,"Profile updated","Profile updated.",QMessageBox::Ok,this,Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
    info.exec();
    this->setEnabled(true);
}
