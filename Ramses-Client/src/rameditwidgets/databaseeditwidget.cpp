#include "databaseeditwidget.h"

#include "ramdatainterface/dbinterface.h"

DatabaseEditWidget::DatabaseEditWidget(QWidget *parent) :
    QScrollArea(parent)
{
    setupUi();
    connectEvents();
}

const QString &DatabaseEditWidget::dbFile() const
{
    return m_dbFile;
}

void DatabaseEditWidget::setDbFile(const QString &newDbFile)
{
    m_dbFile = newDbFile;

    ui_folderDisplay->setPath(m_dbFile);

    QString path = LocalDataInterface::getRamsesPath(m_dbFile);
    if (path == "" || path == "auto") path = QDir::homePath() + "/Ramses";
    ui_folderSelector->setPath(path);

    // Get settings
    ServerConfig s = LocalDataInterface::getServerSettings(m_dbFile);

    ui_serverEdit->setAddress(s.address);
    ui_serverEdit->setSsl(s.useSsl);
    ui_serverEdit->setTimeout(s.timeout);
    ui_serverEdit->setUpdateFreq(s.updateDelay);

    // Check if the address is valid
    bool sync = ui_serverEdit->address() != "";
    ui_syncBox->setChecked(sync);
    ui_serverEdit->setEnabled(sync);
}

void DatabaseEditWidget::apply()
{
    DBInterface *dbi = DBInterface::instance();
    // If this is the current db, restart app
    if (m_dbFile == dbi->dataFile())
    {
        QMessageBox::Button ok = QMessageBox::question(
                    this,
                    tr("Restart Ramses"),
                    tr("The application needs to be restarted in order to apply changes.\nAre you sure you want to continue?"));
        if (ok != QMessageBox::Yes) return;
    }

    ServerConfig s;

    if (ui_syncBox->isChecked())
    {
        s.address = ui_serverEdit->address();
        if (s.address == "") return;
        s.useSsl = ui_serverEdit->ssl();
        s.updateDelay = ui_serverEdit->updateFreq();
        s.timeout = ui_serverEdit->timeout();
    }

    LocalDataInterface::setRamsesPath(m_dbFile, ui_folderSelector->path());
    LocalDataInterface::setServerSettings(m_dbFile, s);

    // If this is the current db, restart app
    if (m_dbFile == dbi->dataFile())
    {
        dbi->setOffline();
        // Restart
        QString program = qApp->arguments()[0];
        QStringList arguments = qApp->arguments().mid(1); // remove the 1st argument - the program name
        qApp->quit();
        QProcess::startDetached(program, arguments);
    }

    emit applied();
}

void DatabaseEditWidget::reset()
{
    setDbFile(m_dbFile);
}

void DatabaseEditWidget::setupUi()
{
    QWidget *dummy = new QWidget(this);
    dummy->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setWidget(dummy);
    this->setWidgetResizable(true);
    this->setFrameStyle(QFrame::NoFrame);

    QVBoxLayout *mainLayout = new QVBoxLayout(dummy);
    mainLayout->setSpacing(3);
    mainLayout->setContentsMargins(3, 3, 3, 3);

    QGridLayout *topLayout = new QGridLayout();
    topLayout->setSpacing(3);
    topLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(topLayout);

    QLabel *localLabel = new QLabel(tr("Local Data"));
    localLabel->setEnabled(false);
    topLayout->addWidget(localLabel,0 , 0);

    topLayout->addWidget(new QLabel(tr("Database")),1 , 0);

    ui_folderDisplay = new DuQFFolderDisplayWidget();
    topLayout->addWidget(ui_folderDisplay,1 , 1);

    topLayout->addWidget(new QLabel(tr("Ramses path")),2 , 0);

    ui_folderSelector = new DuQFFolderSelectorWidget();
    topLayout->addWidget(ui_folderSelector,2 , 1);

    QLabel *serverLabel = new QLabel(tr("Ramses Server"));
    serverLabel->setEnabled(false);
    topLayout->addWidget(serverLabel,3 , 0);

    topLayout->addWidget(new QLabel(tr("Server")),4 , 0);

    ui_syncBox = new QCheckBox(tr("Online (Sync)"), dummy);
    topLayout->addWidget(ui_syncBox, 4, 1);

    ui_serverEdit = new ServerEditWidget();
    ui_serverEdit->setEnabled(false);
    mainLayout->addWidget(ui_serverEdit);

    QHBoxLayout *validButtons = new QHBoxLayout();
    validButtons->setSpacing(3);
    validButtons->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(validButtons);

    ui_resetButton = new QPushButton(tr("Reset"));
    ui_resetButton->setIcon(QIcon(":/icons/reinit"));
    validButtons->addWidget(ui_resetButton);

    ui_applyButton = new QPushButton(tr("Apply changes"));
    ui_applyButton->setIcon(QIcon(":/icons/apply"));
    validButtons->addWidget(ui_applyButton);

    mainLayout->addStretch();
}

void DatabaseEditWidget::connectEvents()
{
    connect(ui_syncBox, &QCheckBox::clicked, ui_serverEdit, &ServerEditWidget::setEnabled);
    connect(ui_resetButton, &QPushButton::clicked, this, &DatabaseEditWidget::reset);
    connect(ui_applyButton, &QPushButton::clicked, this, &DatabaseEditWidget::apply);
}
