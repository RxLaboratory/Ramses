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
    /**
     * @brief instance returns the unique instance of RamServerInterface.
     * @return the instance.
     */
    static RamServerInterface *instance();
    /**
     * @brief The current server address
     * @return
     */
    const QString &serverAddress() const;
    /**
     * @brief Returns either "http://" or "https://"
     * @return
     */
    const QString serverProtocol();
    /**
     * @brief True if we're using a secured connection
     * @return
     */
    bool ssl() const;
    /**
     * @brief setTimeout sets how long to wait for the server on ping, in seconds
     * @param newTimeout the new timeout
     */
    void setTimeout(int newTimeout);
    /**
     * @brief The version of the Ramses Server (if we've been online at least once).
     * @return
     */
    const QString &serverVersion() const;
    int updateFrequency() const;
    void setUpdateFrequency(int newUpdateFrequency);

    QString login(QString username, QString password);

    /**
     * @brief Pings the server and waits for the pong
     * @param force Set to true to force the ping even if we're already online.
     * It's a way to reset the session token.
     * @return
     */
    bool waitPing(bool force=false);

public slots:
    void setOnline();
    void setOffline();
    void setServerAddress(const QString &newServerAddress);
    void setSsl(bool useSsl);

signals:
    /**
     * @brief Emitted when the SSL status changes.
     */
    void sslChanged(bool);
    /**
     * @brief Emitted when a request is posted to the server.
     * The QString is the query (method).
     */
    void queried(QString);
    /**
     * @brief Emitted when the status of the connection changed (online, offline, connecting, error...)
     */
    void connectionStatusChanged(NetworkUtils::NetworkStatus, QString);
    /**
     * @brief Emitted when soma data is received and validated
     * from dataReceived()
     */
    void data(QJsonObject);

protected:
    static RamServerInterface *_instance;

private slots:
    /**
     * @brief nextRequest Sends the next request from the requests queue.
     */
    void nextRequest();
    /**
     * @brief Goes online (pings the server) and check if the connection is valid.
     * Calls waitPing(true)
     */
    void checkConnection();
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
     * @brief flushRequests posts all the requests in the queue as fast as possible.
     * This may be used just before quitting the application.
     */
    void flushRequests();

private:
    /**
     * @brief This is a singleton, private constructor
     * @param parent
     */
    RamServerInterface();
    /**
     * @brief connectEvents Connects member events
     */
    void connectEvents();   
    /**
     * @brief Posts some data to the server
     * @param query The name of the query
     * @param data The associated data
     */
    void post(QString query, QJsonObject data);
    /**
     * @brief Posts a request to the server
     * @param request
     */
    QNetworkReply *postRequest(Request r);
    /**
     * @brief buildRequest Creates a request to be queued or sent to the server
     * @param query The name of the query (See the Ramses Server API reference)
     * @param body The body of the request
     */
    Request buildRequest(QString query, QJsonObject body);
    Request buildRequest(QString query);
    /**
     * @brief Adds a request to the queue
     * @param r the request to add
     */
    void queueRequest(QString query, QJsonObject body);
    void queueRequest(Request r);
    /**
     * @brief Starts posting requests if and only if we're Online
     */
    void startQueue();
    /**
     * @brief processReply gets the json data from the server's reply
     * @param reply
     * @return
     */
    QJsonObject processReply(QNetworkReply *reply);
    /**
     * @brief Manages the remote connection
     */
    QNetworkAccessManager m_network;
    /**
     * @brief The token given by the server when logging in.
     */
    QString m_sessionToken = "";
    /**
     * @brief m_forbiddenWords A list of forbidden words in URLs
     */
    QStringList m_forbiddenWords;
    /**
     * @brief A timer used to send requests to the server at regular intervals.
     * It prevents flooding the server by sending too many requests in a short time,
     * which could be considered as SPAM.
     */
    QTimer *m_requestQueueTimer;
    /**
     * @brief A timer to ping the server at regular intervals.
     * This is used to make sure the connection is alive,
     * and refresh the session token regularly for improved security.
     */
    QTimer *m_pingTimer;
    /**
     * @brief The waiting list for requests
     */
    QList<Request> m_requestQueue;
    /**
     * @brief Online / Offline status
     */
    NetworkUtils::NetworkStatus m_status = NetworkUtils::Offline;
    /**
     * @brief The delay between two pings (in ms) to check the connection, and refresh the session token.
     */
    int m_pingDelay = 30000;
    /**
     * @brief _requestDelay A delay between requests to avoid spamming the server (and being blacklisted)
     * @todo expose this as a user setting ?
     */
    int m_requestDelay = 500;
    /**
     * @brief m_updateFrequency Time between two syncs in ms
     */
    int m_updateFrequency = 60000;
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
    int m_timeout = 3;
};

#endif // RAMSERVERINTERFACE_H
