#include "logindialog.h"

#include "datacrypto.h"

LoginDialog::LoginDialog(QWidget *parent):
    QDialog(parent)
{
    this->setModal(true);
    setupUi();
    connectEvents();
}

bool LoginDialog::setServerAddress(QString address)
{
    ui_serverLabel->setText(tr("Connecting to: %1").arg(address));
    m_serverAddress = address;

    // Reset login fields
    ui_usernameEdit->setText("");
    ui_passwordEdit->setText("");

    // Load saved credentials
    QSettings settings;

    int historySize = settings.beginReadArray("servers");
    for (int i = 0; i < historySize; i++)
    {
        settings.setArrayIndex(i);
        // Get adress in settings
        QString settingsAddress = settings.value("url").toString();
        if (settingsAddress == address )
        {
            // Decrypt
            DataCrypto *crypto = DataCrypto::instance();

            QString username = settings.value("username", "").toString();

            if (username == "")
            {
                ui_saveUsername->setChecked(false);
                ui_usernameEdit->setText("");
                ui_passwordEdit->setText("");
                ui_passwordEdit->setPlaceholderText("Password");
            }
            else
            {
                username = crypto->machineDecrypt( username );
                ui_usernameEdit->setText( username );
                ui_saveUsername->setChecked(true);

                QString password = settings.value("password", "").toString();
                if (password == "")
                {
                    ui_savePassword->setChecked(false);
                    ui_passwordEdit->setText("");
                    ui_passwordEdit->setPlaceholderText("Password");
                    m_hashedPassword = "";
                }
                else
                {
                    ui_savePassword->setChecked(true);
                    m_hashedPassword = crypto->machineDecrypt( password );
                    ui_passwordEdit->setPlaceholderText("Use saved password.");
                    ui_passwordEdit->setText("");
                    return true;
                }
            }

            break;
        }
    }
    settings.endArray();

    return false;
}

void LoginDialog::accept()
{
    QString username = ui_usernameEdit->text();
    if (username == "")
    {
        ui_connectionStatusLabel->setText("Please fill your username in.");
        return;
    }
    QString password = ui_passwordEdit->text();
    if (password != "")
    {
        // Hash password
        QString address = m_serverAddress;
        address.replace("/", "");
        password = DataCrypto::instance()->generatePassHash(password, address);
    }
    else if (password == "" && (ui_passwordEdit->placeholderText() != "Use saved password." || m_hashedPassword == ""))
    {
        ui_connectionStatusLabel->setText("Please fill your password in.");
        return;
    }
    else
    {
        password = m_hashedPassword;
    }

    // Hide
    this->hide();

    // Emit
    emit loggedIn(username, password, ui_saveUsername->isChecked(), ui_savePassword->isChecked());

    QDialog::accept();
}

int LoginDialog::exec()
{
    // Move to mouse
    // Doesn't seem to work
    /*QPoint globalCursorPos = QCursor::pos();
    if (this->parentWidget())
    {
        this->move( this->parentWidget()->mapFromGlobal(globalCursorPos) );
    }
    else
    {
        this->move( globalCursorPos );
    }*/
    return QDialog::exec();
}

void LoginDialog::toggleSaveUsername(bool enabled)
{
    if (enabled) {
        ui_savePassword->setEnabled(true);
    }
    else {
        ui_savePassword->setEnabled(false);
        ui_savePassword->setChecked(false);
    }
}

void LoginDialog::toggleSavePassword(bool enabled)
{
    if (!enabled) return;

    // Confirm that !

    QMessageBox::StandardButton result = QMessageBox::question(
                this,
                "Please confirm",
                "Saving the password is unsafe.\nAre you sure you want to save your password?",
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No
                );
    if (result == QMessageBox::No) ui_savePassword->setChecked(false);
}

void LoginDialog::setupUi()
{
    this->setMaximumWidth(256);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(3);

    //: %1 is the address of the server the user is connecting to.
    ui_serverLabel = new QLabel(tr("Connecting to: %1"));
    layout->addWidget(ui_serverLabel);

    QHBoxLayout *usernameLayout = new QHBoxLayout();
    usernameLayout->setSpacing(3);
    usernameLayout->setContentsMargins(0,0,0,0);
    layout->addLayout(usernameLayout);

    ui_usernameEdit = new QLineEdit(this);
    ui_usernameEdit->setPlaceholderText(tr("Username"));
    usernameLayout->addWidget(ui_usernameEdit);

    ui_saveUsername = new QCheckBox(tr("Save"), this);
    usernameLayout->addWidget(ui_saveUsername);

    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->setSpacing(3);
    passwordLayout->setContentsMargins(0,0,0,0);
    layout->addLayout(passwordLayout);

    ui_passwordEdit = new QLineEdit(this);
    ui_passwordEdit->setEchoMode(QLineEdit::Password);
    ui_passwordEdit->setPlaceholderText(tr("Password"));
    passwordLayout->addWidget(ui_passwordEdit);

    ui_savePassword = new QCheckBox(tr("Save"), this);
    ui_savePassword->setEnabled(false);
    passwordLayout->addWidget(ui_savePassword);

    ui_connectionStatusLabel = new QLabel(tr("Ready."));
    layout->addWidget(ui_connectionStatusLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0,0,0,0);
    buttonLayout->setSpacing(3);
    layout->addLayout(buttonLayout);

    ui_cancelButton = new QPushButton(tr("Cancel"), this);
    ui_cancelButton->setIcon(QIcon(":/icons/close"));
    ui_cancelButton->setAutoDefault(false);
    buttonLayout->addWidget(ui_cancelButton);

    ui_loginButton = new QPushButton(tr("Log in"), this);
    ui_loginButton->setIcon(QIcon(":/icons/login"));
    buttonLayout->addWidget(ui_loginButton);

    // Tab order
    QWidget::setTabOrder(ui_usernameEdit, ui_passwordEdit);
}

void LoginDialog::connectEvents()
{
    connect(ui_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui_loginButton, &QPushButton::clicked, this, &LoginDialog::accept);

    connect(ui_saveUsername, SIGNAL(toggled(bool)), this, SLOT(toggleSaveUsername(bool)));
    connect(ui_savePassword, SIGNAL(clicked(bool)), this, SLOT(toggleSavePassword(bool)));
}
