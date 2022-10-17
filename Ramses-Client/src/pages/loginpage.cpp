#include "loginpage.h"

#include "duqf-utils/guiutils.h"
#include "mainwindow.h"

LoginPage::LoginPage(QWidget *parent) :
    QWidget(parent)
{
    setupUi();

    //ui_capsLockLabel->hide(); // TODO implement CAPS Lock detection

    m_ramses = Ramses::instance();
    //m_failedTimer = new QTimer(this);
    //m_failedTimer->setSingleShot(true);
    //m_uiTimer = new QTimer(this);
    //m_failedAttempts = 0;

    connectEvents();
}

void LoginPage::createDatabase()
{
    if (!ui_databaseCreateWidget)
    {
        ui_databaseCreateWidget = new DatabaseCreateWidget();
    }

    MainWindow *mw = (MainWindow*)GuiUtils::appMainWindow();
    mw->setPropertiesDockWidget( ui_databaseCreateWidget, tr("Create Ramses Database"), ":/icons/storage" );
}

void LoginPage::openDatabase()
{
    QString p = QFileDialog::getOpenFileName(this, tr("Open Ramses Database"), "", "Ramses DB (*.ramses);;SQLite (*.sqlite);;All Files (*.*)");
    if (p == "") return;

    // Add to combobox and select
    ui_dataBaseBox->insertItem(0, QFileInfo(p).fileName(), p);
    ui_dataBaseBox->setCurrentIndex(0);
}

void LoginPage::editDatabase()
{
    MainWindow *mw = (MainWindow*)GuiUtils::appMainWindow();

    if (!ui_databaseEditWidget)
    {
        ui_databaseEditWidget = new DatabaseEditWidget();
        connect(ui_databaseEditWidget, &DatabaseEditWidget::applied, mw, &MainWindow::hidePropertiesDock);
    }

    ui_databaseEditWidget->setDbFile( ui_dataBaseBox->itemData( ui_dataBaseBox->currentIndex() ).toString() );
    mw->setPropertiesDockWidget( ui_databaseEditWidget, tr("Edit %1").arg(ui_dataBaseBox->currentText()), ":/icons/storage" );
}

void LoginPage::updateDatabaseRecentList()
{
    ui_dataBaseBox->clear();

    QSettings settings;
    int n = settings.beginReadArray("database/recent");
    for (int i = 0; i < n; i++)
    {
        settings.setArrayIndex(i);
        QString p = settings.value("path").toString();
        QFileInfo f(p);
        if (f.exists()) ui_dataBaseBox->addItem( f.baseName(), p);
    }
    settings.endArray();

    databaseChanged(ui_dataBaseBox->currentIndex());
}

void LoginPage::databaseChanged(int i)
{
    ui_settingsDBButton->setEnabled(i >= 0);
}

void LoginPage::loginButton_clicked()
{
    // Set database
    DBInterface::instance()->setDataFile( ui_dataBaseBox->currentData().toString() );

    qDebug() << "Adding DB to recent list";

    QSettings settings;
    QString dbPath = ui_dataBaseBox->currentData().toString();

    // Add current at first
    QStringList dbs;
    dbs << dbPath;

    // Load all other recent
    int historySize = settings.beginReadArray("database/recent");
    for (int i = 0; i < historySize; i++)
    {
        settings.setArrayIndex(i);
        QString p = settings.value("path", "-").toString();
        if (!QFileInfo::exists(p)) continue;
        if (p == dbPath) continue;
        dbs << p;
    }
    settings.endArray();

    settings.beginWriteArray("database/recent", dbs.count());
    for (int i = 0; i < dbs.count(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", dbs[i]);
    }
    settings.endArray();

    // Trigger a full sync
    if (RamServerInterface::instance()->isOnline()) DBInterface::instance()->fullSync();
}

void LoginPage::userChanged(RamUser *u)
{
    if (u)
    {
        ui_loginWidget->hide();
        ui_statusLabel->setText(tr("Welcome %1!").arg(u->name()));
    }
    else
    {
        ui_loginWidget->show();
        ui_statusLabel->setText(tr("Ready"));
    }
    updateDatabaseRecentList();
}

void LoginPage::setupUi()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addStretch();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(3);
    mainLayout->addLayout(layout);

    layout->addStretch();

    QLabel *logoLabel = new QLabel(this);
    logoLabel->setPixmap(QPixmap(":/icons/logo_large"));
    logoLabel->setMinimumSize(QSize(256, 256));
    logoLabel->setMaximumSize(QSize(256, 256));
    logoLabel->setScaledContents(true);
    logoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(logoLabel);

    layout->addStretch();

    ui_loginWidget = new QWidget(this);
    ui_loginWidget->setMaximumWidth(256);
    layout->addWidget(ui_loginWidget);
    QVBoxLayout *loginLayout = new QVBoxLayout(ui_loginWidget);

    ui_bigOpenButton = new QPushButton(this);
    ui_bigOpenButton->setText(tr("Open existing database..."));
    ui_bigOpenButton->setIcon(QIcon(":/icons/storage"));
    ui_bigOpenButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    loginLayout->addWidget(ui_bigOpenButton);

    ui_bigCreateButton = new QPushButton(this);
    ui_bigCreateButton->setText(tr("Create new database..."));
    ui_bigCreateButton->setIcon(QIcon(":/icons/add"));
    ui_bigCreateButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    loginLayout->addWidget(ui_bigCreateButton);

    loginLayout->addSpacing(25);

    QHBoxLayout *dataBaseLayout = new QHBoxLayout();
    dataBaseLayout->setSpacing(0);
    dataBaseLayout->setContentsMargins(0,0,0,0);
    loginLayout->addLayout(dataBaseLayout);

    dataBaseLayout->addWidget(new QLabel(tr("Recent:") + " "));

    ui_dataBaseBox = new QComboBox(this);
    dataBaseLayout->addWidget(ui_dataBaseBox);

    ui_settingsDBButton = new QPushButton(this);
    ui_settingsDBButton->setIcon(QIcon(":/icons/settings"));
    ui_settingsDBButton->setToolTip(tr("Database settings"));
    ui_settingsDBButton->setEnabled(false);
    dataBaseLayout->addWidget(ui_settingsDBButton);

    dataBaseLayout->setStretch(0, 1);
    dataBaseLayout->setStretch(1, 100);
    dataBaseLayout->setStretch(2, 0);

    ui_loginButton = new QPushButton(tr("Log in"), this);
    ui_loginButton->setIcon(QIcon(":/icons/login"));
    loginLayout->addWidget(ui_loginButton);

    loginLayout->addSpacing(25);

    ui_statusLabel = new QLabel(tr("Ready"));
    ui_statusLabel->setEnabled(false);
    ui_statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(ui_statusLabel);

    layout->addStretch();
    mainLayout->addStretch();

    updateDatabaseRecentList();
    databaseChanged(ui_dataBaseBox->currentIndex());
}

void LoginPage::connectEvents()
{
    connect(ui_bigCreateButton, &QPushButton::clicked, this, &LoginPage::createDatabase);
    connect(ui_bigOpenButton, &QPushButton::clicked, this, &LoginPage::openDatabase);
    connect(ui_settingsDBButton, &QPushButton::clicked, this, &LoginPage::editDatabase);

    connect(ui_dataBaseBox, SIGNAL(currentIndexChanged(int)), this, SLOT(databaseChanged(int)));

    connect(m_ramses,&Ramses::userChanged, this, &LoginPage::userChanged);
    //connect(m_ramses,&Ramses::loggedOut, this, &LoginPage::loggedOut);
    //connect(DBInterface::instance(), &DBInterface::log, this, &LoginPage::dbiLog);
    //connect(Daemon::instance(), &Daemon::log, this, &LoginPage::daemonLog);
    connect(ui_loginButton, SIGNAL(clicked()), this, SLOT(loginButton_clicked()));
    //connect(m_failedTimer, &QTimer::timeout, this, &LoginPage::unFreeze);
    //connect(m_uiTimer, &QTimer::timeout, this, &LoginPage::updateFreeze);
}
