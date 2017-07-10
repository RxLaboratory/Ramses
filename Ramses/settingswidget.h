#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "ui_settingswidget.h"
#include "updater.h"
#include "dbinterface.h"
#include "application.h"
#include <QFileDialog>

class SettingsWidget : public QWidget, private Ui::SettingsWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(DBInterface *db,Updater *up,QWidget *parent = 0);
    void login();
    void logout();
    int getToolButtonStyle();

signals:
    void loggedOut();
    void setToolButtonStyle(int);

private slots:
    void on_serverAddressEdit_editingFinished();
    void on_sslCheckBox_clicked(bool checked);
    void on_updateFreqSpinBox_editingFinished();
    void on_timeOutEdit_editingFinished();
    void on_settingsLogoutButton_clicked();
    void on_updateCSSButton_clicked();
    void on_styleBox_currentIndexChanged(int index);
    void on_toolButtonStyleBox_currentIndexChanged(int index);

private:
    //app
    Application *app ;
    //settings
    QSqlDatabase settingsDB;
    //DBI
    DBInterface *dbi;
    //Updater
    Updater *updater;

    void updateCSS(QString cssPath);
};

#endif // SETTINGSWIDGET_H
