#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSettings>
#include <QNetworkCookieJar>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDeadlineTimer>
#include <QApplication>
#include <QStringBuilder>
#include <QTimer>
#include <QThread>
#include <QProgressBar>
#include <QVersionNumber>

#include "duqf-utils/utils.h"
#include "duqf-utils/duqflogger.h"
#include "ramserverinterface.h"
#include "localdatainterface.h"

class DBInterface : public DuQFLoggerObject
{
    Q_OBJECT
public:
    /**
     * @brief instance Gets the unique DBInterface instance.
     * @return the unique instance.
     */
    static DBInterface *instance();

    //bool isReady() const;

    /**
     * @brief The current status (offline or online, maybe connecting)
     * @return
     */
    NetworkUtils::NetworkStatus connectionStatus() const;
    bool isSuspended();

    /**
     * @brief setRamsesPath sets the path to the local data for this database
     * @param p
     */
    void setRamsesPath(QString p);
    QString ramsesPath();

    // DATA INTERFACE //

    QSet<QString> tableUuids(QString table);
    bool contains(QString uuid, QString table);

    void createObject(QString uuid, QString table, QString data);

    QString objectData(QString uuid, QString table);
    void setObjectData(QString uuid, QString table, QString data);

    void removeObject(QString uuid, QString table);
    void restoreObject(QString uuid, QString table);
    bool isRemoved(QString uuid, QString table);

    void setUsername(QString uuid, QString username);
    bool isUserNameAavailable(const QString &userName);

    const QString &dataFile() const;
    void setDataFile(const QString &file, bool ignoreUser = false);

    void setCurrentUserUuid(QString uuid);

signals:
    /**
     * @brief Emitted if the mode has changed
     */
    void connectionStatusChanged(NetworkUtils::NetworkStatus, QString);
    /**
     * @brief dataUpdated is emitted when the whole data has changed.
     * e.g. when the local data file has been changed.
     */
    void dataReset();
    void userChanged(QString);
    void synced();

public slots:
    void suspendSync();
    void resumeSync();
    void sync();
    void fullSync(bool synchroneous = false);
    void quit();
    /**
     * @brief Changes to offline mode: data is stored locally until we get a connection to the server to sync.
     */
    void setOffline();
    /**
     * @brief Changes to online mode.
     */
    void setOnline();
    // MAINTENANCE //
    QString cleanDabaBase(int deleteDataOlderThan = -1);
    bool undoClean();
    void acceptClean();

protected:
    /**
     * @brief The unique DBInteface instance
     */
    static DBInterface *_instance;

private slots:
    /**
     * @brief Handles changes of status from the Ramses server connection
     * @param status
     */
    void serverConnectionStatusChanged(NetworkUtils::NetworkStatus status);
    /**
     * @brief Handles changes of connection status and emits connectionStatusChanged()
     * @param s The new status
     * @param reason A user friendly explanation
     */
    void setConnectionStatus(NetworkUtils::NetworkStatus s, QString reason = "");
    /**
     * @brief serverUserChanged Handles changes of users sent from the server.
     * @param userUuid
     */
    void serverUserChanged(QString userUuid);

private:
    /**
     * @brief This is a singleton, private constructor
     * @param parent
     */
    explicit DBInterface(QObject *parent = nullptr);
    /**
     * @brief Connects all member events (just at the end of constructor method)
     */
    void connectEvents();
    /**
     * @brief The current status (offline, connecting or online)
     */
    NetworkUtils::NetworkStatus m_connectionStatus = NetworkUtils::Offline;
    /**
     * @brief m_rsi is the interface with the Ramses server.
     */
    RamServerInterface *m_rsi;
    /**
     * @brief m_ldi is the interface with the local data.
     */
    LocalDataInterface *m_ldi;
    /**
     * @brief m_serverAddress The address of the server
     */
    QString m_serverAddress;
    /**
     * @brief m_updateFrequency 60s by default
     */
    int m_updateFrequency = 60000;
    QTimer *m_updateTimer;
    bool m_suspended = true;
};

#endif // DBINTERFACE_H
