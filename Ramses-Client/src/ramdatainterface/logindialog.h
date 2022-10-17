#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    LoginDialog(QWidget *parent);
    bool setServerAddress(QString address);

    QString username;
    QString password;
    bool saveUserName;
    bool savePassword;

public slots:
    virtual void accept() override;
    virtual int exec() override;

signals:
    void loggedIn(QString, QString, bool, bool);

private slots:
    void toggleSaveUsername(bool enabled);
    void toggleSavePassword(bool enabled);

private:
    void setupUi();
    void connectEvents();

    QLabel *ui_serverLabel;
    QLabel *ui_connectionStatusLabel;
    QLineEdit *ui_usernameEdit;
    QCheckBox *ui_saveUsername;
    QLineEdit *ui_passwordEdit;
    QCheckBox *ui_savePassword;
    QPushButton *ui_cancelButton;
    QPushButton *ui_loginButton;

    QString m_hashedPassword;
    QString m_serverAddress;
};

#endif // LOGINDIALOG_H
