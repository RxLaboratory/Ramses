#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QScrollBar>
#include <QSysInfo>

#include "ramses.h"

#include "databasecreatewidget.h"
#include "databaseeditwidget.h"

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);

private slots:
    void createDatabase();
    void openDatabase();
    void editDatabase();

    void updateDatabaseRecentList();
    void databaseChanged(int i);

    void loginButton_clicked();

    void userChanged(RamUser *u);

private:
    void setupUi();
    void connectEvents();

    QWidget *ui_loginWidget;
    QLabel *ui_statusLabel;
    QPushButton *ui_bigOpenButton;
    QPushButton *ui_bigCreateButton;
    QComboBox *ui_dataBaseBox;
    QPushButton *ui_createDBButton;
    QPushButton *ui_openDBButton;
    QPushButton *ui_settingsDBButton;
    QPushButton *ui_loginButton;
    DatabaseCreateWidget *ui_databaseCreateWidget = nullptr;
    DatabaseEditWidget *ui_databaseEditWidget = nullptr;

    QString m_hashedPassword;

    Ramses *m_ramses;
};

#endif // LOGINPAGE_H
