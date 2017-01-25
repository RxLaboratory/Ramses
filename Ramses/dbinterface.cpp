#include "dbinterface.h"
#include <QtDebug>
#include <QtGlobal>

DBInterface::DBInterface(QObject *parent) : QObject(parent)
{
    serverAddress = "rainboxprod.coop/ramtest";
    protocol = "https://";
    updateFreq = 1;

    QNetworkCookieJar *cj = new QNetworkCookieJar();
    qnam.setCookieJar(cj);

    connect(&qnam, SIGNAL(finished(QNetworkReply *)), this,SLOT(dataReceived(QNetworkReply *)));
    connect(&qnam, SIGNAL(sslErrors(QNetworkReply *,QList<QSslError>)), this,SLOT(sslError(QNetworkReply *,QList<QSslError>)));

    //TODO Check if server is available and if server version is higherr
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
    message("Remote request === " + protocol + serverAddress + req);
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
            message("Remote request === Content: [hidden login informations]");
        }
        else
        {
            message("Remote request === Content: " + content.toJson());
        }
        reply = qnam.post(request,content.toJson());
    }
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this,SLOT(networkError(QNetworkReply::NetworkError)));
    message("Remote request === Sent!");
}

void DBInterface::dataReceived(QNetworkReply * rep)
{
    if (rep->error() == QNetworkReply::NoError)
    {
        QString repAll = rep->readAll();
        QJsonDocument repDoc = QJsonDocument::fromJson(repAll.toUtf8());
        QJsonObject repObj = repDoc.object();
        QString repType = repObj.value("type").toString();
        bool repAccepted = repObj.value("accepted").toBool();
        QString repMessage = repObj.value("message").toString();
        bool repSuccess = repObj.value("success").toBool();
        QJsonValue content = repObj.value("content");

        if (repMessage == "") repMessage = repAll;
        emit message(repMessage);

        if (!repAccepted)
        {
            if (repMessage == "") repMessage = "The server could not compute the request.";
            emit connected(false,repMessage);
            return;
        }

        //LOGIN
        if (repType == "login") { emit connected(repSuccess,repMessage); return; }

        //STATUS
        else if (repType == "addStatus") {  emit statusAdded(repSuccess,repMessage); return; }
        else if (repType == "getStatuses") { emit gotStatuses(repSuccess,repMessage,content); return; }
        else if (repType == "updateStatus") { emit statusUpdated(repSuccess,repMessage); return; }
        else if (repType == "removeStatus") { emit statusRemoved(repSuccess,repMessage); return; }

        //STAGE
        else if (repType == "addStage") {  emit stageAdded(repSuccess,repMessage); return; }
        else if (repType == "getStages") { emit gotStages(repSuccess,repMessage,content); return; }
        else if (repType == "updateStage") { emit stageUpdated(repSuccess,repMessage); return; }
        else if (repType == "removeStage") { emit stageRemoved(repSuccess,repMessage); return; }

        //PROJECT
        else if (repType == "addProject") {  emit projectAdded(repSuccess,repMessage); return; }
        else if (repType == "getProjects") { emit gotProjects(repSuccess,repMessage,content); return; }
        else if (repType == "updateProject") { emit projectUpdated(repSuccess,repMessage); return; }
        else if (repType == "removeProject") { emit projectRemoved(repSuccess,repMessage); return; }
        else if (repType == "addProjectStage") { emit projectStageAdded(repSuccess,repMessage); return; }
        else if (repType == "removeProjectStage") { emit projectStageRemoved(repSuccess,repMessage); return; }

        //SHOT
        else if (repType == "addShots") {  emit shotAdded(repSuccess,repMessage); return; }
        else if (repType == "getShots") { emit gotShots(repSuccess,repMessage,content); return; }
        else if (repType == "updateShot") { emit shotUpdated(repSuccess,repMessage); return; }
        else if (repType == "setShotStatus") { emit shotStatusUpdated(repSuccess,repMessage); return; }
        else if (repType == "removeShots") { emit shotRemoved(repSuccess,repMessage); return; }
        else if (repType == "moveShotsUp") { emit shotsMovedUp(repSuccess,repMessage); return; }
        else if (repType == "moveShotsDown") { emit shotsMovedDown(repSuccess,repMessage); return; }

        //ASSET
        else if (repType == "addAsset") {  emit assetAdded(repSuccess,repMessage); return; }
        else if (repType == "setAssetStatus") { emit assetStatusUpdated(repSuccess,repMessage); return; }
        else if (repType == "assignAsset") { emit assetAssigned(repSuccess,repMessage); return; }

        if (repMessage != "") emit connected(false,repMessage);
        emit connected(false,"Unknown error from server.");
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
void DBInterface::addStatus(QString name,QString shortName,QString color,QString description)
{
    QString q = "?type=addStatus";
    QJsonObject obj;
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    obj.insert("color",color);
    obj.insert("description",description);
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
void DBInterface::addStage(QString name,QString shortName,QString type)
{
    QString q = "?type=addStage";
    QJsonObject obj;
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    obj.insert("type",type);
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

void DBInterface::updateStage(int id, QString name, QString shortName, QString type)
{
    QString q = "?type=updateStage";
    QJsonObject obj;
    obj.insert("id",id);
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    obj.insert("type",type);
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
void DBInterface::addProject(QString name,QString shortName)
{
    QString q = "?type=addProject";
    QJsonObject obj;
    obj.insert("name",name);
    obj.insert("shortName",shortName);
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
void DBInterface::addShots(int projectId, int statusId, QStringList shots, int order)
{
    QString q = "?type=addShots";
    QJsonObject obj;
    QJsonArray jsonNames;
    foreach(QString name,shots)
    {
        jsonNames.insert(jsonNames.count(),name);
    }
    obj.insert("names",jsonNames);
    obj.insert("projectId",projectId);
    obj.insert("statusId",statusId);
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

void DBInterface::setShotStatus(int statusId,int stageId,int shotId)
{
    QString q = "?type=setShotStatus";
    QJsonObject obj;
    obj.insert("statusId",statusId);
    obj.insert("stageId",stageId);
    obj.insert("shotId",shotId);
    QJsonDocument json(obj);

    emit message("Updating shot status");
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
void DBInterface::addAsset(QString name, QString shortName, int stageId, int statusId, int shotId)
{
    QString q = "?type=addAsset";
    QJsonObject obj;
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    obj.insert("stageId",stageId);
    obj.insert("statusId",statusId);
    obj.insert("shotId",shotId);
    QJsonDocument json(obj);

    emit message("Submitting asset");
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

void DBInterface::assignAsset(int assetId,int shotId)
{
    QString q = "?type=assignAsset";
    QJsonObject obj;
    obj.insert("assetId",assetId);
    obj.insert("shotId",shotId);
    QJsonDocument json(obj);

    emit message("Assigning asset");
    sendRequest(q,json);
}
