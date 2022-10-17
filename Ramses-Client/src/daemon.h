#ifndef DAEMON_H
#define DAEMON_H

#include <QObject>
#include <QTcpServer>
#include <QSettings>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>

#include "duqf-utils/duqflogger.h"

class Daemon : public DuQFLoggerObject
{
    Q_OBJECT
public:
    static Daemon *instance();
    void start();
    void stop();
    void restart();

public slots:
    void suspend();
    void resume();

signals:
    void raise();

protected:
    static Daemon *_instance;

private slots:
    void newConnection();
    void reply();
    void reply(QString req, QTcpSocket *client);

    // === API ===

    /**
     * @brief ping Sends the client version and the current user uuid.
     * {
     *  "query": "ping",
     *  "message": "Hi, this is the Ramses Daemon",
     *  "accepted": true,
     *  "success": true,
     *  "content": {
     *      "version": "1.0.0",
     *      "ramses": "Ramses",
     *      "userUuid": "unique-uid"
     *      }
     * }
     * @param client
     */
    void ping(QTcpSocket *client);

    // Current state
    /**
     * @brief ping Sends the current project uuid and data.
     * {
     *  "query": "getCurrentProject",
     *  "message": "\"Project Name\" is the current project.",
     *  "accepted": true,
     *  "success": true,
     *  "content": {
     *      "uuid": "unique-uid",
     *      "data": "{some JSON data}"
     *      }
     * }
     * @param client
     */
    void getCurrentProject(QTcpSocket *client);
    /**
     * @brief ping Sends the current project uuid and data.
     * {
     *  "query": "getCurrentProject",
     *  "message": "Current project set to: \"Project Name\".",
     *  "accepted": true,
     *  "success": true,
     *  "content": { }
     * }
     * @param client
     */
    void setCurrentProject(QString uuid, QTcpSocket *client);

    // DATA
    /**
     * @brief create Creates a new object
     * {
     *  "query": "create",
     *  "message": "I've created a new \"RamObject\".",
     *  "accepted": true,
     *  "success": true,
     *  "content": { }
     * }
     * @param uuid
     * @param data
     * @param type
     * @param client
     */
    void create(QString uuid, QString data, QString type, QTcpSocket *client);
    /**
     * @brief ping Sends the ramses absolute folder path.
     * {
     *  "query": "getRamsesFolder",
     *  "message": "I've got the list of \"%1\".",
     *  "accepted": true,
     *  "success": true,
     *  "content": {
     *      "objects": [
     *          {
     *              "uuid": "unique-uid1",
     *              "data": { "some JSON data" }
     *          },
     *          {
     *              "uuid": "unique-uid2",
     *              "data": { "some JSON data" }
     *          },
     *          {
     *              "uuid": "unique-uid3",
     *              "data": { "some JSON data" }
     *          }
     *          ]
     *      }
     * }
     * @param client
     */
    void getObjects(QString type, QTcpSocket *client);
    /**
     * @brief ping Sends the ramses absolute folder path.
     * {
     *  "query": "getRamsesFolder",
     *  "message": "I've got the Ramses folder.",
     *  "accepted": true,
     *  "success": true,
     *  "content": {
     *      "path": "path/to/ramses/data"
     *      }
     * }
     * @param client
     */
    void getRamsesFolder(QTcpSocket *client);
    /**
     * @brief ping Sends the list of existing projects (for the current user).
     * {
     *  "query": "getProjects",
     *  "message": "I've got the project list.",
     *  "accepted": true,
     *  "success": true,
     *  "content": {
     *      "projects": [
     *          {
     *              "uuid": "unique-uid1",
     *              "data": { "some JSON data" }
     *          },
     *          {
     *              "uuid": "unique-uid2",
     *              "data": { "some JSON data" }
     *          },
     *          {
     *              "uuid": "unique-uid3",
     *              "data": { "some JSON data" }
     *          }
     *          ]
     *      }
     * }
     * @param client
     */
    void getProjects(QTcpSocket *client);
    /**
     * @brief setData Updates the data of an object.
     * {
     *  "query": "setData",
     *  "message": "Data updated.",
     *  "accepted": true,
     *  "success": true,
     *  "content": { }
     * }
     * @param uuid
     * @param data
     * @param client
     */
    void setData(QString uuid, QString data, QTcpSocket *client);
    /**
     * @brief getData Gets the data of an object.
     * {
     *  "query": "getData",
     *  "message": "I've got some data.",
     *  "accepted": true,
     *  "success": true,
     *  "content": {
     *      "uuid": "unique-uid",
     *      "data": {"Some JSON Data"}
     *      }
     * }
     * @param uuid
     * @param client
     */
    void getData(QString uuid, QTcpSocket *client);
    /**
     * @brief uuidFromPath Gets the uuid of an object from its path.
     * {
     *  "query": "uuidFromPath",
     *  "message": "I've found an object.",
     *  "accepted": true,
     *  "success": true,
     *  "content": {
     *      "uuid": "unique-uid"
     *      }
     * }
     * @param uuid
     * @param client
     */
    void uuidFromPath(QString path, QString objectType, QTcpSocket *client);

    // PATHS
    /**
     * @brief getPath Gets the path of the object.
     * {
     *  "query": "getPath",
     *  "message": "I've got the path of the object.",
     *  "accepted": true,
     *  "success": true,
     *  "content": {
     *      "path": "path/to/the/object"
     *      }
     * }
     * @param uuid
     * @param client
     */
    void getPath(QString uuid, QTcpSocket *client);

private:
    //The daemon is a singleton
    explicit Daemon(QObject *parent = nullptr);

    // The server and its state
    QTcpServer *m_tcpServer;
    bool m_suspended = false;
    QStringList m_queue;
    QVector<QTcpSocket*> m_waitingClients;

    // Post a reply
    void post(QTcpSocket *client, QJsonObject content, QString query, QString message="", bool success = true, bool accepted = true);
};

#endif // DAEMON_H
