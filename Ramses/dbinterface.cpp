#include "dbinterface.h"
#include <QtDebug>
#include <QtGlobal>

DBInterface::DBInterface(QObject *parent) : QObject(parent)
{
    //========== LOCAL DATABASE =============

    localDB = QSqlDatabase::addDatabase("QSQLITE","localdata");

    //check if the file already exists, if not, extract it from resources
    QString localDBPath = "";
#ifdef Q_OS_MAC
    localDBPath = QDir::homePath() + "/Ramses/localdata.s3db";
#else
    localDBPath = "localdata.s3db";
#endif

    QFile dbFile(localDBPath);

    if (!dbFile.exists())
    {
        QFile dbResource(":/localdata");
        //on mac, we can not write inside the app, so create folder at home
#ifdef Q_OS_MAC
        QDir home = QDir::home();
        home.mkdir("Ramses");
#endif
        //copy the default file from the resources
        dbResource.copy(localDBPath);
        QFile::setPermissions(localDBPath,QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ReadOther | QFileDevice::WriteOther);
    }

    localDB.setDatabaseName(localDBPath);
    localDB.setHostName("localhost");
    localDB.open();

    //============ REMOTE SERVER =============

    serverAddress = "rainboxprod.coop/ramtest";
    protocol = "https://";
    updateFreq = 1;

    QNetworkCookieJar *cj = new QNetworkCookieJar();
    qnam.setCookieJar(cj);

    connect(&qnam, SIGNAL(finished(QNetworkReply *)), this,SLOT(dataReceived(QNetworkReply *)));
    connect(&qnam, SIGNAL(sslErrors(QNetworkReply *,QList<QSslError>)), this,SLOT(sslError(QNetworkReply *,QList<QSslError>)));

    //TODO Check if server is available and if server version is higher
}

//PARAMETERS
void DBInterface::setServerAddress(QString address)
{
    if (!address.endsWith("/")) address += "/";
    serverAddress = address;
}

void DBInterface::setSsl(bool ssl)
{
    if (ssl) protocol = "https://";
    else protocol = "http://";
}

void DBInterface::setUpdateFreq(int freq)
{
    updateFreq = freq;
}

//CONNECTION
void DBInterface::connection(QString user,QString passHash)
{
    //request url
    QString requestString = "?type=login";

    QJsonObject obj;
    obj.insert("username",user);
    obj.insert("password",passHash);
    QJsonDocument json(obj);

    emit connecting();
    emit message("Connecting...");
    sendRequest(requestString, json);
}

void DBInterface::sendRequest(QString req,QJsonDocument content)
{
    //request
    emit message("Remote request === " + protocol + serverAddress + req);
    QUrl url(protocol + serverAddress + req);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::UserAgentHeader, "Rainbox-Asset-Manager 0.1-Alpha");
    if (content.isEmpty())
    {
        reply = qnam.get(request);
    }
    else
    {
        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
        if (req.indexOf("login") >= 0)
        {
            emit message("Remote request === Content: [hidden login informations]");
        }
        else
        {
            emit message("Remote request === Content: " + content.toJson());
        }
        reply = qnam.post(request,content.toJson());
    }
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this,SLOT(networkError(QNetworkReply::NetworkError)));
}

void DBInterface::dataReceived(QNetworkReply * rep)
{
    if (rep->error() == QNetworkReply::NoError)
    {
        QString repAll = rep->readAll();

        QJsonDocument repDoc = QJsonDocument::fromJson(repAll.toUtf8());
        QJsonObject repObj = repDoc.object();

        if (repObj.isEmpty())
        {
            repObj.insert("message",repAll);
            repObj.insert("accepted",false);
        }

        emit data(repObj);

        QString repType = repObj.value("type").toString();
        QString repMessage = repObj.value("message").toString();
        bool repSuccess = repObj.value("success").toBool();
        QJsonValue content = repObj.value("content");
    }

}

void DBInterface::networkError(QNetworkReply::NetworkError err)
{
    QString reason;
    if (err == QNetworkReply::ConnectionRefusedError)
    {
        reason = "The server refused the connection,\nplease try again later.";
    }
    else if (err == QNetworkReply::RemoteHostClosedError)
    {
        reason = "Server unavailable,\nplease try again later.";
    }
    else if (err == QNetworkReply::HostNotFoundError)
    {
        reason = "Invalid server,\ncheck the network settings.";
    }
    else if (err == QNetworkReply::TimeoutError)
    {
        reason = "Server unavailable,\nplease try again later.";
    }
    else if (err == QNetworkReply::OperationCanceledError)
    {
        reason = "Operation canceled.";
    }
    else if (err == QNetworkReply::SslHandshakeFailedError)
    {
        reason = "Secure connection (SSL) unavailable.\nCheck the network settings.";
    }
    else if (err == QNetworkReply::TemporaryNetworkFailureError)
    {
        reason = "Temporary network error,\nplease try again later.";
    }
    else if (err == QNetworkReply::NetworkSessionFailedError)
    {
        reason = "No connection found,\nplease check your network connection.";
    }
    else if (err == QNetworkReply::BackgroundRequestNotAllowedError)
    {
        reason = "Background request not allowed,\nplease check your system firewall settings.";
    }
#if QT_VERSION >= 0x050700
    else if (err == QNetworkReply::TooManyRedirectsError)
    {
        reason = "Too many redirections,\nthis server is misconfigured.";
    }
    else if (err == QNetworkReply::InsecureRedirectError)
    {
        reason = "Insecure redirection.";
    }
#endif
    else if (err == QNetworkReply::ProxyConnectionRefusedError)
    {
        reason = "The proxy refused the connection.";
    }
    else if (err == QNetworkReply::ProxyConnectionClosedError)
    {
        reason = "The proxy closed the connection prematurely.";
    }
    else if (err == QNetworkReply::ProxyNotFoundError)
    {
        reason = "The proxy was not found.\nPlease check your proxy settings.";
    }
    else if (err == QNetworkReply::ProxyTimeoutError)
    {
        reason = "The proxy did not respond in time.\nPlease check your proxy settings.";
    }
    else if (err == QNetworkReply::ProxyAuthenticationRequiredError)
    {
        reason = "The proxy needs authentication.";
    }
    else if (err == QNetworkReply::ContentAccessDenied)
    {
        reason = "The server denied the access (401).";
    }
    else if (err == QNetworkReply::ContentOperationNotPermittedError)
    {
        reason = "Operation not permitted.";
    }
    else if (err == QNetworkReply::ContentNotFoundError)
    {
        reason = "Content not found (404).\nCheck network settings.";
    }
    else if (err == QNetworkReply::AuthenticationRequiredError)
    {
        reason = "The server needs authentication.";
    }
    else if (err == QNetworkReply::ContentReSendError)
    {
        reason = "The request failed.\nPlease try again later.";
    }
    else if (err == QNetworkReply::ContentConflictError)
    {
        reason = "Content conflict.";
    }
    else if (err == QNetworkReply::ContentGoneError)
    {
        reason = "The requested resource is no longer available.";
    }
    else if (err == QNetworkReply::InternalServerError)
    {
        reason = "Internal server error.";
    }
    else if (err == QNetworkReply::OperationNotImplementedError)
    {
        reason = "The server cannot reply (operation not implemented).";
    }
    else if (err == QNetworkReply::ServiceUnavailableError)
    {
        reason = "Service unavailable.\nPlease try again later.";
    }
    else if (err == QNetworkReply::UnknownNetworkError)
    {
        reason = "Unknown network error.";
    }
    else if (err == QNetworkReply::UnknownProxyError)
    {
        reason = "Unknown proxy error.";
    }
    else if (err == QNetworkReply::UnknownContentError)
    {
        reason = "Unknown content error.";
    }
    else if (err == QNetworkReply::ProtocolFailure)
    {
        reason = "Protocol failure.";
    }
    else if (err == QNetworkReply::ProtocolFailure)
    {
        reason = "Unknown server error.";
    }
    emit connected(false,reason);
    emit message(reason);
}

void DBInterface::sslError(QNetworkReply *rep, QList<QSslError> errs)
{
    foreach (QSslError err, errs)
    {
        qDebug() << "SSL error:";
        qDebug() << err.errorString();
    }
    emit message("SSL Error. Connection may not be secured.");
}

//STATUS
void DBInterface::addStatus(QString name,QString shortName,QString color,QString description,int id)
{   
    QString q = "?type=addStatus";
    QJsonObject obj;
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    obj.insert("color",color);
    obj.insert("description",description);
    if (id >= 0) obj.insert("id",id);
    QJsonDocument json(obj);

    emit message("Submitting status");

    sendRequest(q,json);
}

void DBInterface::getStatuses()
{
    QString q = "?type=getStatuses";
    emit message("Getting statuses");
    sendRequest(q);
}

void DBInterface::updateStatus(int id, QString name, QString shortName, QString color, QString description)
{
    QString q = "?type=updateStatus";
    QJsonObject obj;
    obj.insert("id",id);
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    obj.insert("color",color);
    obj.insert("description",description);
    QJsonDocument json(obj);

    emit message("Submitting status");
    sendRequest(q,json);
}

void DBInterface::removeStatus(int id)
{
    QString q = "?type=removeStatus";
    QJsonObject obj;
    obj.insert("id",id);
    QJsonDocument json(obj);

    emit message("Removing status");
    sendRequest(q,json);
}

//STAGES
void DBInterface::addStage(QString name, QString shortName, int id)
{
    QString q = "?type=addStage";
    QJsonObject obj;
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    if (id >= 0) obj.insert("id",id);
    QJsonDocument json(obj);

    emit message("Submitting stage");
    sendRequest(q,json);
}

void DBInterface::getStages()
{
    QString q = "?type=getStages";
    emit message("Getting stages");
    sendRequest(q);
}

void DBInterface::updateStage(int id, QString name, QString shortName)
{
    QString q = "?type=updateStage";
    QJsonObject obj;
    obj.insert("id",id);
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    QJsonDocument json(obj);

    emit message("Submitting stage");
    sendRequest(q,json);
}

void DBInterface::removeStage(int id)
{
    QString q = "?type=removeStage";
    QJsonObject obj;
    obj.insert("id",id);
    QJsonDocument json(obj);

    emit message("Removing stage");
    sendRequest(q,json);
}

//PROJECTS
void DBInterface::addProject(QString name,QString shortName, int id)
{
    QString q = "?type=addProject";
    QJsonObject obj;
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    if (id >= 0) obj.insert("id",id);
    QJsonDocument json(obj);

    emit message("Submitting project");
    sendRequest(q,json);
}

void DBInterface::getProjects()
{
    QString q = "?type=getProjects";
    emit message("Getting projects");
    sendRequest(q);
}

void DBInterface::updateProject(int id, QString name, QString shortName)
{
    QString q = "?type=updateProject";
    QJsonObject obj;
    obj.insert("id",id);
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    QJsonDocument json(obj);

    emit message("Submitting project");
    sendRequest(q,json);
}

void DBInterface::removeProject(int id)
{
    QString q = "?type=removeProject";
    QJsonObject obj;
    obj.insert("id",id);
    QJsonDocument json(obj);

    emit message("Removing project");
    sendRequest(q,json);
}

void DBInterface::addProjectStage(int projectId, int stageId)
{
    QString q = "?type=addProjectStage";
    QJsonObject obj;
    obj.insert("projectId",projectId);
    obj.insert("stageId",stageId);
    QJsonDocument json(obj);

    emit message("Associating stage with project");
    sendRequest(q,json);
}

void DBInterface::removeProjectStage(int projectId, int stageId)
{
    QString q = "?type=removeProjectStage";
    QJsonObject obj;
    obj.insert("projectId",projectId);
    obj.insert("stageId",stageId);
    QJsonDocument json(obj);

    emit message("Removing stage from project");
    sendRequest(q,json);
}

//SHOTS
void DBInterface::addShot(int projectId, int id, QString name, double duration, int order)
{
    QString q = "?type=addShots";
    QJsonObject obj;
    QJsonArray jsonShots;
    QJsonObject jsonShot;
    jsonShot.insert("name",name);
    jsonShot.insert("duration",duration);
    jsonShot.insert("id",id);
    jsonShots.insert(0,jsonShot);
    obj.insert("shots",jsonShots);
    obj.insert("projectId",projectId);
    obj.insert("shotOrder",order);
    QJsonDocument json(obj);

    emit message("Submitting shots");
    sendRequest(q,json);
}

void DBInterface::addShots(int projectId, QList<QStringList> shots, int order)
{
    QString q = "?type=addShots";
    QJsonObject obj;
    QJsonArray jsonShots;
    foreach(QStringList shot,shots)
    {
        QJsonObject jsonShot;
        jsonShot.insert("name",shot[0]);
        jsonShot.insert("duration",shot[1]);
        jsonShot.insert("id",shot[2]);
        jsonShots.insert(jsonShots.count(),jsonShot);
    }
    obj.insert("shots",jsonShots);
    obj.insert("projectId",projectId);
    obj.insert("shotOrder",order);
    QJsonDocument json(obj);

    emit message("Submitting shots");
    sendRequest(q,json);
}

void DBInterface::getShots(int projectId)
{
    QString q = "?type=getShots";
    QJsonObject obj;
    obj.insert("projectId",projectId);
    QJsonDocument json(obj);

    emit message("Getting shots");
    sendRequest(q, json);
}

void DBInterface::updateShot(int id, QString name, double duration)
{
    QString q = "?type=updateShot";
    QJsonObject obj;
    obj.insert("id",id);
    obj.insert("name",name);
    obj.insert("duration",duration);
    QJsonDocument json(obj);

    emit message("Submitting shot");
    sendRequest(q,json);
}

void DBInterface::removeShot(int id)
{
    QString q = "?type=removeShots";
    QJsonObject obj;
    QJsonArray jsonIds;
    jsonIds.insert(0,id);
    obj.insert("ids",jsonIds);
    QJsonDocument json(obj);

    emit message("Removing shots");
    sendRequest(q,json);
}

void DBInterface::removeShots(QList<int> ids)
{
    QString q = "?type=removeShots";
    QJsonObject obj;
    QJsonArray jsonIds;
    foreach(int id, ids)
    {
        jsonIds.insert(jsonIds.count(),id);
    }
    obj.insert("ids",jsonIds);
    QJsonDocument json(obj);

    emit message("Removing shots");
    sendRequest(q,json);
}

void DBInterface::resetShotsOrder(QList<int> ids)
{
    QString q = "?type=resetShotsOrder";
    QJsonObject obj;
    QJsonArray jsonIds;
    foreach(int id, ids)
    {
        jsonIds.insert(jsonIds.count(),id);
    }
    obj.insert("ids",jsonIds);
    QJsonDocument json(obj);

    emit message("Reseting shots order.");
    sendRequest(q,json);
}

void DBInterface::moveShotsUp(QList<int> ids)
{
    QString q = "?type=moveShotsUp";
    QJsonObject obj;
    QJsonArray jsonIds;
    foreach(int id, ids)
    {
        jsonIds.insert(jsonIds.count(),id);
    }
    obj.insert("ids",jsonIds);
    QJsonDocument json(obj);

    emit message("Moving shots up.");
    sendRequest(q,json);
}

void DBInterface::moveShotsDown(QList<int> ids)
{
    QString q = "?type=moveShotsDown";
    QJsonObject obj;
    QJsonArray jsonIds;
    foreach(int id, ids)
    {
        jsonIds.insert(jsonIds.count(),id);
    }
    obj.insert("ids",jsonIds);
    QJsonDocument json(obj);

    emit message("Moving shots down.");
    sendRequest(q,json);
}

//ASSETS
int DBInterface::addAsset(QString name, QString shortName, int statusId,int stageId,QString comment)
{
    // LOCAL

    emit message("Saving asset");
    QString local = "INSERT INTO assets (name,shortName,statusId,stageId,comment) VALUES ('%1','%2',%3,%4,'%5');";
    local = local.arg(name,shortName,QString::number(statusId),QString::number(stageId),comment);
    QSqlQuery qInsert(local,localDB);
    QString result = "SELECT last_insert_rowid();";
    QSqlQuery qResult(result,localDB);
    qResult.next();
    int id = qResult.value(0).toInt();

    // REMOTE

    QString remote = "?type=addAsset";
    QJsonObject obj;
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    obj.insert("statusId",statusId);
    obj.insert("stageId",stageId);
    obj.insert("id",id);
    obj.insert("comment",comment);
    QJsonDocument json(obj);

    emit message("Submitting asset");
    sendRequest(remote,json);

    return id;
}

void DBInterface::updateAsset(int id, QString name, QString shortName, QString comment)
{
    QString remote = "?type=updateAsset";
    QJsonObject obj;
    obj.insert("assetId",id);
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    obj.insert("comment",comment);

    QJsonDocument json(obj);

    emit message("Updating asset");
    sendRequest(remote,json);
}

void DBInterface::assignAsset(int assetId, int shotId)
{
    QString q = "?type=assignAsset";
    QJsonObject obj;
    obj.insert("assetId",assetId);
    obj.insert("shotId",shotId);
    QJsonDocument json(obj);

    emit message("Assigning asset");
    sendRequest(q,json);
}

void DBInterface::unAssignAsset(int assetId, int shotId)
{
    QString q = "?type=unAssignAsset";
    QJsonObject obj;
    obj.insert("assetId",assetId);
    obj.insert("shotId",shotId);
    QJsonDocument json(obj);

    emit message("un-assigning asset");
    sendRequest(q,json);
}

void DBInterface::getAssets(int projectId)
{
    QString q = "?type=getAssets";
    QJsonObject obj;
    obj.insert("projectId",projectId);
    QJsonDocument json(obj);

    emit message("Requesting assets");
    sendRequest(q,json);
}

void DBInterface::setAssetStatus(int statusId, int assetId)
{
    QString q = "?type=setAssetStatus";
    QJsonObject obj;
    obj.insert("statusId",statusId);
    obj.insert("assetId",assetId);
    QJsonDocument json(obj);

    emit message("Updating asset status");
    sendRequest(q,json);
}


