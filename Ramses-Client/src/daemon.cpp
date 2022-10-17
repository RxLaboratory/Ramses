#include "daemon.h"
#include "duqf-app/app-version.h"
#include "ramses.h"

Daemon *Daemon::_instance = nullptr;

Daemon* Daemon::instance()
{
    if (!_instance) _instance = new Daemon();
    return _instance;
}

void Daemon::start()
{
    QSettings settings;
    if (!m_tcpServer->listen( QHostAddress::LocalHost, settings.value("daemon/port", 18185).toInt() )) {
        qDebug() << m_tcpServer->errorString();
        log(tr("Unable to start the daemon server.") + "\n" + m_tcpServer->errorString(), DuQFLog::Warning);
    }
    else
    {
        qDebug() << "Daemon started and listening on port " + QString::number(m_tcpServer->serverPort());
        log(
            tr("Daemon started and listening on port %1.").arg(
                QString::number(m_tcpServer->serverPort() )
                ),
            DuQFLog::Information);
    }
}

void Daemon::stop()
{
    m_tcpServer->close();
    log("Daemon stopped.", DuQFLog::Information);
}

void Daemon::restart()
{
    stop();
    start();
}

void Daemon::suspend()
{
    qDebug() << "Suspending Daemon";
    m_suspended = true;
}

void Daemon::resume()
{
    qDebug() << "Resuming Daemon";
    m_suspended = false;
    while ( !m_queue.isEmpty() ) reply(m_queue.takeFirst(), m_waitingClients.takeFirst());
}

void Daemon::newConnection()
{
    QTcpSocket *client = m_tcpServer->nextPendingConnection();
    connect(client, &QAbstractSocket::disconnected, client, &QObject::deleteLater);
    connect(client, SIGNAL(readyRead()), this,SLOT(reply()));
}

void Daemon::reply()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    QString request = client->readAll();
    if (m_suspended)
    {
        m_queue << request;
        m_waitingClients << client;
        return;
    }

    reply(request, client);
}

void Daemon::reply(QString request, QTcpSocket *client)
{
    //split args
    QStringList requestArgs = request.split("&");
    log("I've got these args: \n" + requestArgs.join("\n"), DuQFLog::Data);

    //Parse
    QMap<QString, QString> args;
    for(const QString &arg: qAsConst(requestArgs))
    {
        QStringList aList = arg.split("=");
        QString key = aList.at(0);
        if (key == "") continue;
        if (aList.count() > 1)
        {
            args[key] = aList.at(1);
        }
        else
        {
            args[key] = "";
        }
    }

    //Read
    if (args.contains("ping"))
        ping(client);

    else if (args.contains("raise"))
        emit raise();

    else if (args.contains("getCurrentProject"))
        getCurrentProject(client);

    else if (args.contains("setCurrentProject"))
        setCurrentProject(args.value("uuid"), client);

    else if (args.contains("getRamsesFolder"))
        getRamsesFolder(client);

    else if (args.contains("getProjects"))
        getProjects(client);

    else if (args.contains("setData"))
        setData(args.value("uuid"), args.value("data"), client);

    else if (args.contains("getData"))
            getData(args.value("uuid"), client);

    else if (args.contains("uuidFromPath"))
            uuidFromPath(args.value("path"), args.value("type"), client);

    else if (args.contains("getPath"))
                getPath(args.value("uuid"), client);

    else if (args.contains("getObjects"))
                getObjects(args.value("type"), client);

    else if (args.contains("create"))
                create(args.value("uuid"), args.value("data"), args.value("type"), client);

    else
        post(client, QJsonObject(), "", tr("Unknown query: %1").arg(request), false, false);
}

void Daemon::ping(QTcpSocket *client)
{
    log(tr("I'm replying to this request: %1.").arg("ping"), DuQFLog::Debug);

    QJsonObject content;
    content.insert("version", STR_VERSION);
    content.insert("ramses", STR_INTERNALNAME);
    RamUser *user = Ramses::instance()->currentUser();
    if (user)
    {
        content.insert("userUuid", user->uuid());
    }
    else
    {
        content.insert("userUuid", "");
    }
    post(client, content, "ping", "Hi, this is the Ramses Daemon");
}

void Daemon::getCurrentProject(QTcpSocket *client)
{
    log(tr("I'm replying to this request: %1.").arg("getCurrentProject"), DuQFLog::Debug);

    QJsonObject content;
    RamProject *proj = Ramses::instance()->currentProject();
    QString message = "There's no current project.";
    if (proj)
    {
        content.insert("uuid", proj->uuid());
        content.insert("data", proj->dataString());
        message = "\"%1\" is the current project.";
        message = message.arg(proj->name());
    }
    else
    {
        content.insert("uuid", "");
        content.insert("data", "{}");
    }


    post(client, content, "getCurrentProject", message);
}

void Daemon::setCurrentProject(QString uuid, QTcpSocket *client)
{
    log(tr("I'm replying to this request: %1.").arg("setCurrentProject"), DuQFLog::Debug);
    log(tr("This is the uuid: %1").arg(uuid), DuQFLog::Data);

    Ramses::instance()->setCurrentProjectUuid(uuid);

    RamProject *p = Ramses::instance()->currentProject();
    if (p) post(client, QJsonObject(), "setCurrentProject", tr("Current project set to: \"%1\".").arg(p->name()));
    else post(client, QJsonObject(), "setCurrentProject", tr("Project not found, sorry!"), false);
}

void Daemon::create(QString uuid, QString data, QString type, QTcpSocket *client)
{
    log(tr("I'm replying to this request: %1.").arg("create"), DuQFLog::Debug);
    log(tr("This is the uuid: %1").arg(uuid), DuQFLog::Data);
    log(tr("This is the data: %1").arg(data), DuQFLog::Data);
    log(tr("This is the type: %1").arg(type), DuQFLog::Data);

     LocalDataInterface::instance()->createObject(uuid, type, data);

     post(client, QJsonObject(), "create", tr("I've created a new \"%1\".").arg(type));
}

void Daemon::getObjects(QString type, QTcpSocket *client)
{

    log(tr("I'm replying to this request: %1.").arg("getObjects"), DuQFLog::Debug);
    log(tr("This is the type: %1").arg(type), DuQFLog::Data);

    QVector<QStringList> entries = LocalDataInterface::instance()->tableData(type);

    QJsonObject content;

    QJsonArray objects;
    for (int i = 0; i < entries.count(); i++)
    {
        QStringList entry = entries.at(i);
        QJsonObject obj;
        obj.insert("uuid", entry.at(0) );
        QJsonDocument d = QJsonDocument::fromJson( entry.at(1).toUtf8() );
        obj.insert("data", d.object());
        objects.append(obj);
    }
    content.insert("objects", objects);

    post(client, content, "getObjects", tr("I've got the list of \"%1\".").arg(type));
}

void Daemon::getRamsesFolder(QTcpSocket *client)
{
    log(tr("I'm replying to this request: %1.").arg("getRamsesFolder"), DuQFLog::Debug);

    QJsonObject content;
    content.insert("path", Ramses::instance()->path(RamObject::NoFolder, true));
    post(client, content, "getRamsesFolder", tr("I've got the Ramses folder."));
}

void Daemon::getProjects(QTcpSocket *client)
{
    log(tr("I'm replying to this request: %1.").arg("getProjects"), DuQFLog::Debug);

    QJsonObject content;
    QJsonArray projects;
    DBTableModel *ramProjects = Ramses::instance()->projects();
    RamUser *u = Ramses::instance()->currentUser();
    if (u)
        for (int i = 0; i < ramProjects->rowCount(); i++)
        {
            RamProject *p = RamProject::c( ramProjects->get(i) );
            if (p->users()->contains(u->uuid()))
            {
                QJsonObject proj;
                proj.insert("uuid", p->uuid());
                proj.insert("data", p->data());
                projects.append( proj );
            }
        }
    content.insert("projects", projects);
    post(client, content, "getProjects", tr("I've got the project list."));
}

void Daemon::setData(QString uuid, QString data, QTcpSocket *client)
{
    log(tr("I'm replying to this request: %1.").arg("setData"), DuQFLog::Debug);
    log(tr("This is the uuid: %1").arg(uuid), DuQFLog::Data);
    log(tr("This is the data: %1").arg(data), DuQFLog::Data);

    RamAbstractObject::setObjectData(uuid, data);

    post(client, QJsonObject(), "setData", tr("Data updated."));
}

void Daemon::getData(QString uuid, QTcpSocket *client)
{
    log(tr("I'm replying to this request: %1.").arg("getData"), DuQFLog::Debug);
    log(tr("This is the uuid: %1").arg(uuid), DuQFLog::Data);

    QJsonObject data = RamAbstractObject::getObjectData(uuid);

    QJsonObject content;
    content.insert("uuid", uuid);
    content.insert("data", data);

    post(client, content, "setData", tr("I've got some data."));
}

void Daemon::uuidFromPath(QString path, QString objectType, QTcpSocket *client)
{
    log(tr("I'm replying to this request: %1.").arg("uuidFromPath"), DuQFLog::Debug);
    log(tr("This is the path: %1").arg(path), DuQFLog::Data);

    QString uuid = RamAbstractObject::uuidFromPath(path, RamAbstractObject::objectTypeFromName(objectType));

    QJsonObject content;
    content.insert("uuid", uuid);

    if (uuid == "") post(client, content, "uuidFromPath", tr("I can't find the object."), false);
    else post(client, content, "uuidFromPath", tr("I've found an object."));
}

void Daemon::getPath(QString uuid, QTcpSocket *client)
{
    log(tr("I'm replying to this request: %1.").arg("getPath"), DuQFLog::Debug);
    log(tr("This is the uuid: %1").arg(uuid), DuQFLog::Data);

    QString path = RamAbstractObject::getObjectPath(uuid);

    QJsonObject content;
    content.insert("path", path);

    post(client, content, "getPath", tr("I've got the path of the object."));
}

Daemon::Daemon(QObject *parent) : DuQFLoggerObject("Daemon", parent)
{
    m_tcpServer = new QTcpServer(this);

    start();

    connect(m_tcpServer, &QTcpServer::newConnection, this, &Daemon::newConnection);
    connect(qApp, &QApplication::aboutToQuit, this, &Daemon::stop);
}

void Daemon::post(QTcpSocket *client, QJsonObject content, QString query, QString message, bool success, bool accepted)
{
    QJsonObject obj;
    obj.insert("query", query);
    obj.insert("message", message);
    obj.insert("accepted", accepted);
    obj.insert("success", success);
    obj.insert("content", content);
    QJsonDocument json(obj);

    QString jsonReply = json.toJson();
    client->write( jsonReply.toUtf8() );

    log("Posting:\n" + jsonReply, DuQFLog::Data);
}
