#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QScrollBar>
#include <QSysInfo>

#include "duqf-widgets/duqfservercombobox.h"
#include "duqf-widgets/duqfsslcheckbox.h"

#include "ramses.h"

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);

signals:
    void serverSettings();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void loggedIn(RamUser *user);
    void loggedOut();

    void loginButton_clicked();
    void serverSettingsButton_clicked();
    void serverAddressChanged(QString address);

    void toggleSaveUsername(bool enabled);
    void toggleSavePassword(bool enabled);

    void setSSL(bool ssl);

private:
    void setupUi();
    void connectEvents();

    QWidget *ui_loginWidget;
    DuQFServerComboBox *ui_serverBox;
    DuQFSSLCheckbox *ui_sslBox;
    QLineEdit *ui_keyEdit;
    QLineEdit *ui_usernameEdit;
    QCheckBox *ui_saveUsername;
    QLineEdit *ui_passwordEdit;
    QCheckBox *ui_savePassword;
    QLabel *ui_capsLockLabel;
    QPushButton *ui_loginButton;
    QLabel *ui_connectionStatusLabel;

    QString m_hashedPassword;

    Ramses *m_ramses;
    QTimer *m_failedTimer;
    QTimer *m_uiTimer;
    int m_failedAttempts;
    void freeze();
    void unFreeze();
    void updateFreeze();
};

#endif // LOGINPAGE_H
