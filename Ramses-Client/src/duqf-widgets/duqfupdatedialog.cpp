#include "duqfupdatedialog.h"

DuQFUpdateDialog::DuQFUpdateDialog(QJsonObject updateInfo, QWidget *parent) : QDialog(parent)
{
    setupUi(updateInfo);
}

void DuQFUpdateDialog::download()
{
    QDesktopServices::openUrl ( QUrl( m_downloadURL ) );
    this->close();
}

void DuQFUpdateDialog::changelog()
{
    QDesktopServices::openUrl ( QUrl( m_changelogURL ) );
    this->close();
}

void DuQFUpdateDialog::donate()
{
    QDesktopServices::openUrl ( QUrl( m_donateURL ) );
    this->close();
}

void DuQFUpdateDialog::setupUi(QJsonObject updateInfo)
{
    this->setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(3);
    this->setLayout(mainLayout);

    if (updateInfo.value("update").toBool())
    {
        this->setWindowTitle("New " % QString(STR_FILEDESCRIPTION) % " available!" );

        QLabel *latestVersionLabel = new QLabel("New version: " % updateInfo.value("version").toString(), this );
        mainLayout->addWidget(latestVersionLabel);

        QTextEdit *descriptionEdit = new QTextEdit(updateInfo.value("description").toString(), this);
        descriptionEdit->setReadOnly(true);
        mainLayout->addWidget(descriptionEdit);

        QLabel *currentVersionLabel = new QLabel("Current version: " % QString(STR_VERSION), this );
        currentVersionLabel->setEnabled(false);
        mainLayout->addWidget(currentVersionLabel);

        m_downloadURL = updateInfo.value("downloadURL").toString();
        if (m_downloadURL != "")
        {
            ui_downloadButton = new QPushButton("Download", this);
            ui_downloadButton->setIcon(QIcon(":/icons/download"));
            mainLayout->addWidget(ui_downloadButton);
            connect(ui_downloadButton, SIGNAL(clicked()), this, SLOT(download()));
        }

        m_changelogURL = updateInfo.value("changelogURL").toString();
        if (m_changelogURL != "")
        {
            ui_changelogButton = new QPushButton("Changelog", this);
            ui_changelogButton->setIcon(QIcon(":/icons/changelog"));
            mainLayout->addWidget(ui_changelogButton);
            connect(ui_changelogButton, SIGNAL(clicked()), this, SLOT(changelog()));
        }

        m_donateURL = updateInfo.value("donateURL").toString();
        if (m_donateURL != "")
        {
            ui_donateButton = new QPushButton("I ♥ " % QString(STR_FILEDESCRIPTION), this);
            ui_donateButton->setIcon(QIcon(":/icons/donate"));
            mainLayout->addWidget(ui_donateButton);
            connect(ui_donateButton, SIGNAL(clicked()), this, SLOT(donate()));
        }

        ui_okButton = new QPushButton("Close", this);
        ui_okButton->setIcon(QIcon(":/icons/close"));
        mainLayout->addWidget(ui_okButton);
        connect(ui_okButton, SIGNAL(clicked()), this, SLOT(reject()));
    }
    else if (updateInfo.value("accepted").toBool())
    {
        this->setWindowTitle( "Update" );

        QLabel *versionLabel = new QLabel("I'm already up-to-date!", this );
        mainLayout->addWidget(versionLabel);

        ui_okButton = new QPushButton("Close", this);
        ui_okButton->setIcon(QIcon(":/icons/close"));
        mainLayout->addWidget(ui_okButton);
        connect(ui_okButton, SIGNAL(clicked()), this, SLOT(reject()));
    }
    else if (updateInfo.value("success").toBool())
    {
        this->setWindowTitle( "Server error" );

        QLabel *label = new QLabel("Sorry, the server could not get update information.", this );
        mainLayout->addWidget(label);

        QTextEdit *descriptionEdit = new QTextEdit(updateInfo.value("description").toString(), this);
        descriptionEdit->setReadOnly(true);
        mainLayout->addWidget(descriptionEdit);

        ui_okButton = new QPushButton("Close", this);
        ui_okButton->setIcon(QIcon(":/icons/close"));
        mainLayout->addWidget(ui_okButton);
        connect(ui_okButton, SIGNAL(clicked()), this, SLOT(reject()));

    }
    else
    {
        this->setWindowTitle( "Server error" );

        QLabel *label = new QLabel("Sorry, there was a server error.", this );
        mainLayout->addWidget(label);

        ui_okButton = new QPushButton("Close", this);
        ui_okButton->setIcon(QIcon(":/icons/close"));
        mainLayout->addWidget(ui_okButton);
        connect(ui_okButton, SIGNAL(clicked()), this, SLOT(reject()));
    }
}
