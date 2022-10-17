#ifndef RAMSERVERINTERFACE_H
#define RAMSERVERINTERFACE_H

#include <QStringBuilder>

#include "duqf-utils/duqflogger.h"
#include "duqf-utils/utils.h"

struct Request
{
    QNetworkRequest request;
    QString body;
    QString query;
};

class RamServerInterface : public DuQFLoggerObject
{
    Q_OBJECT

public:

    // STATIC METHODS //

    /**
     * @brief instance returns the unique instance of RamServerInterface.
     * @return the instance.
     */
    static RamServerInterface *instance();

    // METHODS //

    // Settings

    QString serverAddress() const;
    void setServerAddress(QString newServerAddress);

    bool ssl() const;
    void setSsl(bool useSsl);

    int timeOut() const;
    void setTimeout(int newTimeout);

    const QString &serverVersion() const;

    NetworkUtils::NetworkStatus status() const;
    /**
     * @brief isOnline Checks if we're online, waits a couple of seconds if still connecting
     * @return
     */
    bool isOnline() const;

    // API
    /**
     * @brief ping
     * @param wait when true, waits for the pong
     */
    void ping();
    void sync(QJsonArray tables, QDateTime prevSyncDate = QDateTime::fromString("1970-01-01 00:00:00", "yyyy-MM-dd hh:mm:00"), bool synchroneous = false);
    void sync(QJsonObject body, bool synchroneous = false);
    QJsonArray downloadData();

    const QString &currentUserUuid() const;

    void setUserPassword(QString uuid, QString newPassword, QString currentPassword);

    void deleteData(QHash<QString, QSet<QString> > uuidsToDelete);

public slots:
    /**
     * @brief setOnline posts a ping, and set the status to "Connecting"
     * Status will be changed to "Connected" if we get a valid pong
     */
    void setOnline();
    void setOffline();
    void login();
    QString doLogin(QString username, QString password, bool saveUsername = false, bool savePassword = false);
    void eraseUserPassword();

signals:
    void sslChanged(bool);
    void connectionStatusChanged(NetworkUtils::NetworkStatus, QString);
    void syncReady(QJsonObject);
    void newData(QJsonObject);
    void userChanged(QString);

protected:
    static RamServerInterface *_instance;

private slots:
    /**
     * @brief Changes the status and emits the corresponding signal
     * @param s The new status
     * @param reason User friendly explanation for the change
     */
    void setConnectionStatus(NetworkUtils::NetworkStatus s, QString reason = "");

    /**
     * @brief Parse and transmit the data we've just got
     * @param reply
     */
    void dataReceived(QNetworkReply *reply);
    /**
     * @brief Handles network errors. Connected to requests error()
     * @param err
     */
    void networkError(QNetworkReply::NetworkError err);
    /**
     * @brief Gathers errors with SSL protocol
     * @param reply
     * @param errs
     */
    void sslError(QNetworkReply *reply, QList<QSslError> errs);

    /**
     * @brief nextRequest Sends the next request from the requests queue.
     */
    void nextRequest();

private:

    // METHODS //

    // Constructor //

    RamServerInterface();
    void connectEvents();

    // Network //

    /**
     * @brief Returns either "http://" or "https://"
     * @return
     */
    const QString serverProtocol();

    // Requests and queue //

    /**
     * @brief Posts a request to the server
     * @param request
     */
    void postRequest(Request r);
    QNetworkReply *synchronousRequest(Request r);
    /**
     * @brief Adds a request to the queue
     * @param r the request to add
     */
    void queueRequest(QString query, QJsonObject body);
    void queueRequest(Request r);
    /**
     * @brief buildRequest Creates a request to be queued or sent to the server
     * @param query The name of the query (See the Ramses Server API reference)
     * @param body The body of the request
     */
    Request buildRequest(QString query, QJsonObject body);
    Request buildRequest(QString query);
    /**
     * @brief flushRequests posts all the requests in the queue as fast as possible.
     * This may be used just before quitting the application.
     */
    void flushRequests();
    /**
     * @brief Starts posting requests if and only if we're Online
     */
    void startQueue();
    void pauseQueue();
    /**
     * @brief parseData Checks for errors and parses the data received from the Ramses Server
     * @param reply
     * @return
     */
    QJsonObject parseData(QNetworkReply *reply);

    // ATTRIBUTES //

    // Network //

    /**
     * @brief Manages the remote connection
     */
    QNetworkAccessManager *m_network; // connected for asynchronous queries
    QNetworkAccessManager *m_synchronousNetwork; // used for synchronous queries
    /**
     * @brief Are we using a secured connection?
     */
    bool m_ssl = false;
    /**
     * @brief The current address for the server.
     */
    QString m_serverAddress = "";
    /**
     * @brief The version of the server we're connected to
     */
    QString m_serverVersion = "";
    /**
     * @brief m_timeout How long to wait for the server on ping, in seconds
     */
    int m_timeout = 3000;

    /**
     * @brief Online / Offline status
     */
    NetworkUtils::NetworkStatus m_status = NetworkUtils::Offline;

    /**
     * @brief The token given by the server when logging in.
     */
    QString m_sessionToken = "";

    // Requests and Queue //

    /**
     * @brief m_forbiddenWords A list of forbidden words in URLs
     */
    QStringList m_forbiddenWords;

    /**
     * @brief The waiting list for requests
     */
    QVector<Request> m_requestQueue;

    // Timers //

    /**
     * @brief A timer used to send requests to the server at regular intervals.
     * It prevents flooding the server by sending too many requests in a short time,
     * which could be considered as SPAM.
     */
    QTimer *m_requestQueueTimer;
    /**
     * @brief _requestDelay A delay between requests to avoid spamming the server (and being blacklisted)
     * @todo expose this as a user setting ?
     */
    int m_requestDelay = 500;

    // Authentication //
    QString m_currentUserUuid;
};

#endif // RAMSERVERINTERFACE_H
