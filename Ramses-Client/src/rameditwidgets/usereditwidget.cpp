#include "usereditwidget.h"

#include "ramses.h"
#include "ramdatainterface/datacrypto.h"

UserEditWidget::UserEditWidget(RamUser *user, QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();

    setObject(user);

     m_dontRename << "Ramses" << "Removed" << "Duduf";
}

UserEditWidget::UserEditWidget(QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();
    m_dontRename << "Ramses" << "Removed" << "Duduf";
}

RamUser *UserEditWidget::user() const
{
    return m_user;
}

void UserEditWidget::reInit(RamObject *o)
{
    m_user = qobject_cast<RamUser*>(o);
    RamUser *current = Ramses::instance()->currentUser();
    if (m_user && current)
    {
        ui_cpasswordEdit->setText("");
        ui_npassword1Edit->setText("");
        ui_npassword2Edit->setText("");

        ui_folderWidget->setPath( m_user->path() );
        ui_roleBox->setCurrentIndex(m_user->role());
        ui_colorSelector->setColor(m_user->color());

        if (m_dontRename.contains(m_user->shortName()) || m_user->shortName().toLower() == "new")
        {
            ui_npassword1Edit->setEnabled(false);
            ui_npassword2Edit->setEnabled(false);
            ui_cpasswordEdit->setEnabled(false);
        }
        else
        {
            ui_npassword1Edit->setEnabled(true);
            ui_npassword2Edit->setEnabled(true);
            if (m_user->is(current)) ui_cpasswordEdit->setEnabled(true);
        }

        // Current user
        if (m_user->is(current))
        {
            ui_roleBox->setEnabled(false);
            ui_roleBox->setToolTip("You cannot change your own role!");
            ui_passwordWidget->show();
            ui_cpasswordEdit->show();
            ui_currentPasswordLabel->show();
            this->setEnabled(true);
        }
        // Other non-admin user
        else if (current->role() != RamUser::Admin)
        {
            ui_roleBox->setEnabled(false);
            ui_passwordWidget->hide();
            this->setEnabled(false);
        }
        // Admin
        else
        {
            ui_roleBox->setEnabled(true);
            ui_passwordWidget->show();
            ui_cpasswordEdit->hide();
            ui_currentPasswordLabel->hide();
            this->setEnabled(true);
        }
    }
    else
    {
        ui_cpasswordEdit->setText("");
        ui_npassword1Edit->setText("");
        ui_npassword2Edit->setText("");
        ui_passwordWidget->hide();
        ui_roleBox->setCurrentIndex(0);
        ui_roleBox->setEnabled(false);
        ui_roleBox->setToolTip("");
        ui_cpasswordEdit->setEnabled(false);
        ui_folderWidget->setPath("");
        ui_colorSelector->setColor(QColor(200,200,200));
    }

    // Password field visible only if online
    ui_passwordWidget->setVisible( DBInterface::instance()->connectionStatus() == NetworkUtils::Online );
}

void UserEditWidget::changePassword()
{
    if (!m_user) return;
    if (!checkPasswordInput()) return;

    if (ui_npassword1Edit->text() != "")
    {
        DataCrypto *crypto = DataCrypto::instance();
        RamServerInterface *rsi = RamServerInterface::instance();
        QString newPassword = crypto->generatePassHash(
                    ui_npassword1Edit->text(),
                    rsi->serverAddress().replace("/", "")
                    );
        QString currentPassword = ui_cpasswordEdit->text();
        if (currentPassword != "")
        {
            currentPassword = crypto->generatePassHash(
                                currentPassword,
                                rsi->serverAddress().replace("/", "")
                                );
        }
        rsi->setUserPassword(m_user->uuid(), newPassword, currentPassword);
    }

    ui_npassword1Edit->setText("");
    ui_npassword2Edit->setText("");
    ui_cpasswordEdit->setText("");
}

bool UserEditWidget::checkPasswordInput()
{
    if (!m_user) return false;

    if (ui_npassword1Edit->text() != "")
    {
        if (ui_cpasswordEdit->text() == "" && m_user->is(Ramses::instance()->currentUser()) )
        {
            QMessageBox::warning(this, "What's your password?", "I'm sorry, you have to know your current password to change it.\nPlease try again.\n\n"
                                                                "If you've forgotten your password, you can ask an(other) administrator to change it.\n\n"
                                                                "If you're the only administrator, you have to ask the server provider to reset your account." );
            return false;
        }
        if (ui_npassword1Edit->text() != ui_npassword2Edit->text())
        {
            QMessageBox::warning(this, "Password mismatch", "I'm sorry, the two fields for the new password are different.\nPlease try again." );
            ui_npassword1Edit->setText("");
            ui_npassword2Edit->setText("");
            return false;
        }
    }

    return true;
}

void UserEditWidget::setRole(int r)
{
    if (!m_user) return;
    if (r == 3) m_user->setRole(RamUser::Admin);
    else if (r == 2) m_user->setRole(RamUser::ProjectAdmin);
    else if (r == 1) m_user->setRole(RamUser::Lead);
    else m_user->setRole(RamUser::Standard);
}

void UserEditWidget::setColor(QColor c)
{
    if (!m_user) return;
    m_user->setColor(c);
}

void UserEditWidget::connectionStatusChanged(NetworkUtils::NetworkStatus status)
{
    if (status == NetworkUtils::Online)
    {
        ui_passwordWidget->setEnabled(true);
        ui_passwordWidget->setVisible(true);
    }
    else
    {
        ui_passwordWidget->setEnabled(false);
        RamServerInterface *rsi = RamServerInterface::instance();
        if (rsi->serverAddress() =="" || rsi->serverAddress() == "/")
        {
            ui_passwordWidget->setVisible(false);
        }
    }
}

void UserEditWidget::setupUi()
{
    QLabel *roleLabel = new QLabel("Current role", this);
    ui_mainFormLayout->addWidget(roleLabel, 3, 0);

    ui_roleBox = new QComboBox(this);
    ui_roleBox->addItem(QIcon(":/icons/user"), "Standard");
    ui_roleBox->addItem(QIcon(":/icons/lead"), "Lead");
    ui_roleBox->addItem(QIcon(":/icons/project-admin"), "Project Admin");
    ui_roleBox->addItem(QIcon(":/icons/admin"), "Administrator");
    ui_roleBox->setCurrentIndex(0);
    ui_mainFormLayout->addWidget(ui_roleBox, 3, 1);

    QLabel *colorLabel = new QLabel("Color", this);
    ui_mainFormLayout->addWidget(colorLabel, 4, 0);

    ui_colorSelector = new DuQFColorSelector(this);
    ui_mainFormLayout->addWidget(ui_colorSelector, 4, 1);

    QLabel *uFolderLabel = new QLabel("Personal folder", this);
    ui_mainFormLayout->addWidget(uFolderLabel, 9, 0);

    ui_folderWidget = new DuQFFolderDisplayWidget(this);
    ui_mainFormLayout->addWidget(ui_folderWidget, 9, 1);

    ui_passwordWidget = new QWidget(this);
    QGridLayout *passwordLayout = new QGridLayout(ui_passwordWidget);
    passwordLayout->setContentsMargins(0,0,0,0);
    passwordLayout->setSpacing(3);
    ui_mainLayout->addWidget(ui_passwordWidget);

    ui_currentPasswordLabel = new QLabel("Current password", this);
    passwordLayout->addWidget(ui_currentPasswordLabel, 0, 0);

    ui_cpasswordEdit = new QLineEdit(this);
    ui_cpasswordEdit->setEchoMode(QLineEdit::Password);
    passwordLayout->addWidget(ui_cpasswordEdit, 0, 1);

    QLabel *newPasswordLabel = new QLabel("New password", this);
    passwordLayout->addWidget(newPasswordLabel, 1, 0);

    ui_npassword1Edit = new QLineEdit(this);
    ui_npassword1Edit->setEchoMode(QLineEdit::Password);
    passwordLayout->addWidget(ui_npassword1Edit, 1, 1);

    QLabel *newPasswordLabel2 = new QLabel("Repeat new password", this);
    passwordLayout->addWidget(newPasswordLabel2, 2, 0);

    ui_npassword2Edit = new QLineEdit(this);
    ui_npassword2Edit->setEchoMode(QLineEdit::Password);
    passwordLayout->addWidget(ui_npassword2Edit, 2, 1);

    ui_passwordButton = new QToolButton(this);
    ui_passwordButton->setText("Change password");
    passwordLayout->addWidget(ui_passwordButton, 3, 1);

    ui_mainLayout->addStretch();

    connectionStatusChanged(RamServerInterface::instance()->status());
}

void UserEditWidget::connectEvents()
{
    connect(ui_roleBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setRole(int)));
    connect(ui_passwordButton, SIGNAL(clicked()), this, SLOT(changePassword()));
    //connect(Ramses::instance(), &Ramses::userChanged, this, &UserEditWidget::objectChanged);
    connect(ui_colorSelector, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
    connect(RamServerInterface::instance(), &RamServerInterface::connectionStatusChanged, this, &UserEditWidget::connectionStatusChanged);
}
