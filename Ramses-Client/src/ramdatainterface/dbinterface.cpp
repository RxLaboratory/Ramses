#include "dbinterface.h"
#include "duqf-utils/guiutils.h"
#include "progressmanager.h"

DBInterface *DBInterface::_instance = nullptr;

DBInterface *DBInterface::instance()
{
    if (!_instance) _instance = new DBInterface();
    return _instance;
}
/*
bool DBInterface::isReady() const
{
    return m_ldi->isReady();
}*/

void DBInterface::setOffline()
{
    int timeOut = m_rsi->timeOut();

    ProgressManager *pm = ProgressManager::instance();
    pm->start();
    pm->setMaximum(timeOut + 2);

    pm->setTitle(tr("Disconnecting from the Ramses Server..."));

    // One last sync
    if (m_rsi->isOnline() && !m_suspended)
    {
        pm->setText(tr("One last sync..."));
        pm->increment();

        fullSync();

        // Wait for server timeout to be able to sync
        QDeadlineTimer t( timeOut );
        while (true)
        {
            pm->setProgress( timeOut - t.remainingTime() );
            qApp->processEvents();
            if (t.hasExpired()) break;
        }
    }

    pm->setText(tr("Disconnecting..."));
    pm->increment();
    // Disconnects from the Ramses Server
    m_rsi->setOffline();
    pm->setText(tr("Ready"));
    pm->finish();
}

void DBInterface::setOnline()
{
    if (m_ldi->dataFile() == "")
    {
        QMessageBox::information(
                    GuiUtils::appMainWindow(),
                    tr("No database"),
                    tr("There's no open database.\n\n"
                       "I'm sorry, you need to open or create a database to get online.")
                    );
        return;
    }

    // Connects to the Ramses Server and change connection status
    m_rsi->setOnline();
}

void DBInterface::setRamsesPath(QString p)
{
    m_ldi->setRamsesPath(p);
}

QString DBInterface::ramsesPath()
{
    return m_ldi->ramsesPath();
}

QSet<QString> DBInterface::tableUuids(QString table)
{
    return m_ldi->tableUuids(table);
}

bool DBInterface::contains(QString uuid, QString table)
{
    return m_ldi->contains(uuid, table);
}

void DBInterface::createObject(QString uuid, QString table, QString data)
{
    m_ldi->createObject(uuid, table, data);
}

QString DBInterface::objectData(QString uuid, QString table)
{
    return m_ldi->objectData(uuid, table);
}

void DBInterface::setObjectData(QString uuid, QString table, QString data)
{
    m_ldi->setObjectData(uuid, table, data);
}

void DBInterface::removeObject(QString uuid, QString table)
{
    m_ldi->removeObject(uuid, table);
}

void DBInterface::restoreObject(QString uuid, QString table)
{
    m_ldi->restoreObject(uuid, table);
}

bool DBInterface::isRemoved(QString uuid, QString table)
{
    return m_ldi->isRemoved(uuid, table);
}

void DBInterface::setUsername(QString uuid, QString username)
{
    m_ldi->setUsername(uuid, username);
    // Setting the username must trigger an instant sync (if it's not new)
    if (username.toLower() != "new") sync();
}

bool DBInterface::isUserNameAavailable(const QString &userName)
{
    return m_ldi->isUserNameAavailable(userName);
}

const QString &DBInterface::dataFile() const
{
    return m_ldi->dataFile();
}

void DBInterface::setDataFile(const QString &file, bool ignoreUser)
{
    ProgressManager *pm = ProgressManager::instance();
    pm->start();
    pm->setMaximum(15);

    ServerConfig config = m_ldi->setDataFile(file);
    // Set the new server params
    if (config.address != "")
    {
        m_rsi->setServerAddress(config.address);
        m_rsi->setTimeout(config.timeout);
        m_rsi->setSsl(config.useSsl);
        m_updateFrequency = config.updateDelay;

        // Check the user
        QString userUuid = m_ldi->currentUserUuid();

        emit userChanged( userUuid );
        setOnline();

        pm->setText(tr("Ready!"));
        pm->finish();
    }
    else
    {
        setOffline();
        m_rsi->setServerAddress("");
        if (file == "")
        {
            pm->setText(tr("Ready!"));
            pm->finish();
            return;
        }

        // Check the user
        QString userUuid = m_ldi->currentUserUuid();

        // Check if it exists in the database
        if (!m_ldi->contains(userUuid, "RamUser"))
        {
            userUuid = "";
        }

        if (userUuid != "")
        {
            emit userChanged( userUuid );
            pm->setText(tr("Ready!"));
            pm->finish();
            return;
        }

        if (ignoreUser)
        {
            emit userChanged( userUuid );
            pm->setText(tr("Ready!"));
            pm->finish();
            return;
        }

        // We need to show a list of users
        QVector<QStringList> users = m_ldi->users();
        QStringList names;
        for (int i = 0; i < users.count(); i++)
        {
            names << users[i][1];
        }
        bool ok;
        QString name = QInputDialog::getItem(
                    GuiUtils::appMainWindow(),
                    tr("Unknown user"),
                    tr("Who are you?"),
                    names,
                    0,
                    false,
                    &ok );

        if (ok && !name.isEmpty())
        {
            QString uuid;
            for (int i = 0; i < users.count(); i++)
            {
                if (users[i][1] == name)
                {
                    uuid = users[i][0];
                    break;
                }
            }
            if (uuid != "") setCurrentUserUuid(uuid);
            emit userChanged( userUuid );
            pm->setText(tr("Ready!"));
            pm->finish();
            return;
        }
    }
}

void DBInterface::setCurrentUserUuid(QString uuid)
{
    m_ldi->setCurrentUserUuid(uuid);
}

void DBInterface::suspendSync()
{
    m_updateTimer->stop();
    m_suspended = true;
}

void DBInterface::resumeSync()
{
    m_suspended = false;
    m_updateTimer->start(m_updateFrequency);
}

QString DBInterface::cleanDabaBase(int deleteDataOlderThan)
{
    qDebug() << ">>> Beginning database clean...";

    // Stop auto sync
    qDebug() << "Suspending sync!";
    suspendSync();

    // Clean !
    qDebug() << "--- Cleaning ---";
    return m_ldi->cleanDataBase( deleteDataOlderThan );
}

bool DBInterface::undoClean()
{
    bool ok = m_ldi->undoClean();
    if (!ok) {
        log(tr("Can't restore DB Backup!"), DuQFLog::Critical);
        return false;
    }

    AppUtils::restartApp();
    return true;
}

void DBInterface::acceptClean()
{
    // Get the data to delete,
    // and clean the server data
    QHash<QString, QSet<QString> > deletedUuids = m_ldi->deletedUuids();
    m_rsi->deleteData( deletedUuids );

    resumeSync();
    // Full sync
    fullSync();
}

void DBInterface::sync()
{
    if (m_suspended) {
        log(tr("Sync is suspended!"), DuQFLog::Warning);
        return;
    }
    if (m_connectionStatus != NetworkUtils::Online) return;

    ProgressManager *pm = ProgressManager::instance();
    pm->reInit();
    pm->setMaximum(3);
    pm->setTitle(tr("Quick data sync"));
    pm->setText(tr("Beginning quick data sync..."));
    pm->start();

    // Get modified rows from local
    QJsonObject syncBody = m_ldi->getSync( false );
    // Post to ramserver
    m_rsi->sync(syncBody);

    pm->finish();
}

void DBInterface::fullSync(bool synchroneous)
{
    if (m_suspended) {
        log(tr("Sync is suspended!"), DuQFLog::Warning);
        return;
    }
    if (m_connectionStatus != NetworkUtils::Online) return;

    ProgressManager *pm = ProgressManager::instance();
    pm->reInit();
    pm->setMaximum(3);
    pm->setTitle(tr("Full data sync"));
    pm->setText(tr("Beginning full data sync..."));
    pm->start();

    // Get modified rows from local
    QJsonObject syncBody = m_ldi->getSync( true );
    // Cheat the date
    syncBody.insert("previousSyncDate", "1818-05-05 00:00:00");
    // Post to ramserver
    m_rsi->sync(syncBody, synchroneous);

    pm->finish();
}

void DBInterface::quit()
{
    setOffline();
}

DBInterface::DBInterface(QObject *parent) : DuQFLoggerObject("Database Interface", parent)
{
    // LOCAL
    m_ldi = LocalDataInterface::instance();
    // REMOTE
    m_rsi = RamServerInterface::instance();

    m_updateTimer = new QTimer(this);

    connectEvents();
}

void DBInterface::connectEvents()
{
    connect(m_ldi, &LocalDataInterface::dataReset, this, &DBInterface::dataReset);
    connect(m_ldi, &LocalDataInterface::synced, this, &DBInterface::synced);
    connect(m_rsi, &RamServerInterface::connectionStatusChanged, this, &DBInterface::serverConnectionStatusChanged);
    connect(m_rsi, &RamServerInterface::syncReady, m_ldi, &LocalDataInterface::sync);
    connect(m_rsi, &RamServerInterface::userChanged, this, &DBInterface::serverUserChanged);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(sync()));

    connect(qApp, &QApplication::aboutToQuit, this, &DBInterface::quit);
}

NetworkUtils::NetworkStatus DBInterface::connectionStatus() const
{
    return m_connectionStatus;
}

bool DBInterface::isSuspended()
{
    return m_suspended;
}

void DBInterface::setConnectionStatus(NetworkUtils::NetworkStatus s, QString reason)
{
    if (s == m_connectionStatus) return;
    m_connectionStatus = s;
    emit connectionStatusChanged(s, reason);
}

void DBInterface::serverUserChanged(QString userUuid)
{
    // Check if we should be online
    ServerConfig config = m_ldi->serverConfig();
    if (config.address != "") // online
    {
        if (userUuid != "") // OK
        {
            m_ldi->setCurrentUserUuid(userUuid);
            emit userChanged(userUuid);
            return;
        }

        // The ldi doesn't have user either: can't do anything
        if (m_ldi->currentUserUuid() == "")
        {
            QMessageBox::warning(
                        GuiUtils::appMainWindow(),
                        tr("Can't log in"),
                        tr("I'm sorry, I can't log you in.\n\n"
                           "I don't know who you are, and the authentification failed on the server.\n"
                           "You have to connect to the server before you can use this database.")
                        );
            emit userChanged("");
            return;
        }
    }
}

void DBInterface::serverConnectionStatusChanged(NetworkUtils::NetworkStatus status)
{
    switch(status)
    {
    case NetworkUtils::Offline:
        setConnectionStatus(status, "Disconnected from the Ramses Server.");
        suspendSync();
        break;
    case NetworkUtils::Online:
        setConnectionStatus(status, "Connected to the Ramses Server.");
        resumeSync();
        break;
    default:
        return;
    }
}
