#include "dbinterface.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif
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
    emit message("Connecting...","remote");
    sendRequest(requestString, json);
}

void DBInterface::sendRequest(QString req,QJsonDocument content)
{
    //request
    emit message("Remote request === " + protocol + serverAddress + req,"remote");
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
            emit message("Remote request === Content: [hidden login informations]","remote");
        }
        else
        {
            emit message("Remote request === Content: " + content.toJson(),"remote");
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

        if (repSuccess)
        {
            emit message(repType + "\n" + repMessage + "\nContent:\n" + repAll,"remote");
        }
        else
        {
            emit message("UNSUCCESSFUL:\n" + repType + "\n" + repMessage + "\nContent:\n" + repAll,"warning");
        }

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
    emit message(reason,"critical");
}

void DBInterface::sslError(QNetworkReply *rep, QList<QSslError> errs)
{
    foreach (QSslError err, errs)
    {
        emit message(err.errorString(),"warning");
    }
    emit message("SSL Error. Connection may not be secured.","warning");
}

//USERS
void DBInterface::updateUser(QString uuid, QString userName, QString firstName, QString lastName, QString password)
{
    QString q = "?type=updateUser";
    QJsonObject obj;
    obj.insert("uuid",uuid);
    obj.insert("userName",userName);
    obj.insert("firstName",firstName);
    obj.insert("lastName",lastName);
    obj.insert("password",password);
    QJsonDocument json(obj);

    emit message("Submitting user","remote");
    sendRequest(q,json);
}

//STATUS
void DBInterface::addStatus(QString name,QString shortName,QString color,QString description,QString uuid)
{   
    QString q = "?type=addStatus";
    QJsonObject obj;
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    obj.insert("color",color);
    obj.insert("description",description);
    if (uuid != "") obj.insert("uuid",uuid);
    QJsonDocument json(obj);

    emit message("Submitting status","remote");

    sendRequest(q,json);
}

void DBInterface::getStatuses()
{
    QString q = "?type=getStatuses";
    emit message("Getting statuses","remote");
    sendRequest(q);
}

void DBInterface::updateStatus(QString uuid, QString name, QString shortName, QString color, QString description)
{
    QString q = "?type=updateStatus";
    QJsonObject obj;
    obj.insert("uuid",uuid);
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    obj.insert("color",color);
    obj.insert("description",description);
    QJsonDocument json(obj);

    emit message("Submitting status","remote");
    sendRequest(q,json);
}

void DBInterface::removeStatus(QString uuid)
{
    QString q = "?type=removeStatus";
    QJsonObject obj;
    obj.insert("uuid",uuid);
    QJsonDocument json(obj);

    emit message("Removing status","remote");
    sendRequest(q,json);
}

//STAGES
void DBInterface::addStage(QString name, QString shortName, QString uuid)
{
    QString q = "?type=addStage";
    QJsonObject obj;
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    if (uuid != "") obj.insert("uuid",uuid);
    QJsonDocument json(obj);

    emit message("Submitting stage","remote");
    sendRequest(q,json);
}

void DBInterface::getStages()
{
    QString q = "?type=getStages";
    emit message("Getting stages","remote");
    sendRequest(q);
}

void DBInterface::updateStage(QString uuid, QString name, QString shortName)
{
    QString q = "?type=updateStage";
    QJsonObject obj;
    obj.insert("uuid",uuid);
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    QJsonDocument json(obj);

    emit message("Submitting stage","remote");
    sendRequest(q,json);
}

void DBInterface::removeStage(QString uuid)
{
    QString q = "?type=removeStage";
    QJsonObject obj;
    obj.insert("uuid",uuid);
    QJsonDocument json(obj);

    emit message("Removing stage","remote");
    sendRequest(q,json);
}

//PROJECTS
void DBInterface::addProject(QString name,QString shortName, QString id)
{
    QString q = "?type=addProject";
    QJsonObject obj;
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    if (id != "") obj.insert("uuid",id);
    QJsonDocument json(obj);

    emit message("Submitting project","remote");
    sendRequest(q,json);
}

void DBInterface::getProjects()
{
    QString q = "?type=getProjects";
    emit message("Getting projects","remote");
    sendRequest(q);
}

void DBInterface::updateProject(QString id, QString name, QString shortName)
{
    QString q = "?type=updateProject";
    QJsonObject obj;
    obj.insert("uuid",id);
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    QJsonDocument json(obj);

    emit message("Submitting project","remote");
    sendRequest(q,json);
}

void DBInterface::removeProject(QString id)
{
    QString q = "?type=removeProject";
    QJsonObject obj;
    obj.insert("uuid",id);
    QJsonDocument json(obj);

    emit message("Removing project","remote");
    sendRequest(q,json);
}

void DBInterface::addProjectStage(QString projectId, QString stageId)
{
    QString q = "?type=addProjectStage";
    QJsonObject obj;
    obj.insert("projectId",projectId);
    obj.insert("stageId",stageId);
    QJsonDocument json(obj);

    emit message("Associating stage with project","remote");
    sendRequest(q,json);
}

void DBInterface::removeProjectStage(QString projectId, QString stageId)
{
    QString q = "?type=removeProjectStage";
    QJsonObject obj;
    obj.insert("projectId",projectId);
    obj.insert("stageId",stageId);
    QJsonDocument json(obj);

    emit message("Removing stage from project","remote");
    sendRequest(q,json);
}

//SHOTS
void DBInterface::addShot(QString id, QString name, double duration)
{
    QString q = "?type=addShots";
    QJsonObject obj;
    QJsonArray jsonShots;
    QJsonObject jsonShot;
    jsonShot.insert("name",name);
    jsonShot.insert("duration",duration);
    jsonShot.insert("uuid",id);
    jsonShots.insert(0,jsonShot);
    obj.insert("shots",jsonShots);
    QJsonDocument json(obj);

    emit message("Submitting shot","remote");
    sendRequest(q,json);
}

void DBInterface::addShots(QList<QStringList> shots)
{
    QString q = "?type=addShots";
    QJsonObject obj;
    QJsonArray jsonShots;
    foreach(QStringList shot,shots)
    {
        QJsonObject jsonShot;
        jsonShot.insert("name",shot[0]);
        jsonShot.insert("duration",shot[1]);
        jsonShot.insert("uuid",shot[2]);
        jsonShots.insert(jsonShots.count(),jsonShot);
    }
    obj.insert("shots",jsonShots);
    QJsonDocument json(obj);

    emit message("Submitting shots","remote");
    sendRequest(q,json);
}

void DBInterface::updateShots(QList<QStringList> shots)
{
    QString q = "?type=updateShots";
    QJsonObject obj;
    QJsonArray jsonShots;
    foreach(QStringList shot,shots)
    {
        QJsonObject jsonShot;
        jsonShot.insert("name",shot[0]);
        jsonShot.insert("duration",shot[1]);
        jsonShot.insert("uuid",shot[2]);
        jsonShots.insert(jsonShots.count(),jsonShot);
    }
    obj.insert("shots",jsonShots);
    QJsonDocument json(obj);

    emit message("Submitting shots","remote");
    sendRequest(q,json);
}

void DBInterface::insertShots(QStringList shots,QString projectId, int order)
{
    QString q = "?type=insertShots";
    QJsonObject obj;
    QJsonArray jsonShots;
    foreach(QString shot,shots)
    {
        QJsonObject jsonShot;
        jsonShot.insert("shotId",shot);
        jsonShots.insert(jsonShots.count(),jsonShot);
    }
    obj.insert("shots",jsonShots);
    obj.insert("projectId",projectId);
    obj.insert("shotOrder",order);
    QJsonDocument json(obj);

    emit message("Inserting shots","remote");
    sendRequest(q,json);
}

void DBInterface::insertShot(QString id,QString projectId, int order)
{
    QString q = "?type=insertShots";
    QJsonObject obj;
    QJsonArray jsonShots;
    QJsonObject jsonShot;
    jsonShot.insert("shotId",id);
    jsonShots.insert(0,jsonShot);
    obj.insert("shots",jsonShots);
    obj.insert("projectId",projectId);
    obj.insert("shotOrder",order);
    QJsonDocument json(obj);

    emit message("Inserting shot","remote");
    sendRequest(q,json);
}

void DBInterface::addInsertShots(QList<QStringList> shots,QString projectId, int order)
{
    QString q = "?type=addInsertShots";
    QJsonObject obj;
    QJsonArray jsonShots;
    foreach(QStringList shot,shots)
    {
        QJsonObject jsonShot;
        jsonShot.insert("name",shot[0]);
        jsonShot.insert("duration",shot[1]);
        jsonShot.insert("uuid",shot[2]);
        jsonShots.insert(jsonShots.count(),jsonShot);
    }
    obj.insert("shots",jsonShots);
    obj.insert("projectId",projectId);
    obj.insert("shotOrder",order);
    QJsonDocument json(obj);

    emit message("Inserting shots","remote");
    sendRequest(q,json);
}

void DBInterface::getShots(QString projectId)
{
    QString q = "?type=getShots";
    QJsonObject obj;
    obj.insert("projectId",projectId);
    QJsonDocument json(obj);

    emit message("Getting shots","remote");
    sendRequest(q, json);
}

void DBInterface::updateShot(QString id, QString name, double duration)
{
    QString q = "?type=updateShot";
    QJsonObject obj;
    obj.insert("uuid",id);
    obj.insert("name",name);
    obj.insert("duration",duration);
    QJsonDocument json(obj);

    emit message("Submitting shot","remote");
    sendRequest(q,json);
}

void DBInterface::removeShot(QString id)
{
    QString q = "?type=removeShots";
    QJsonObject obj;
    QJsonArray jsonIds;
    jsonIds.insert(0,id);
    obj.insert("ids",jsonIds);
    QJsonDocument json(obj);

    emit message("Removing shots","remote");
    sendRequest(q,json);
}

void DBInterface::removeShots(QStringList ids, QString projectId)
{
    QString q = "?type=removeShots";
    QJsonObject obj;
    QJsonArray jsonIds;
    foreach(QString id, ids)
    {
        jsonIds.insert(jsonIds.count(),id);
    }
    obj.insert("ids",jsonIds);
    obj.insert("projectId",projectId);
    QJsonDocument json(obj);

    emit message("Removing shots","remote");
    sendRequest(q,json);
}

void DBInterface::resetShotsOrder(QStringList ids)
{
    QString q = "?type=resetShotsOrder";
    QJsonObject obj;
    QJsonArray jsonIds;
    foreach(QString id, ids)
    {
        jsonIds.insert(jsonIds.count(),id);
    }
    obj.insert("ids",jsonIds);
    QJsonDocument json(obj);

    emit message("Reseting shots order.","remote");
    sendRequest(q,json);
}

//ASSETS
void DBInterface::addAsset(QString name, QString shortName, QString statusId,QString stageId,QString comment,QString uuid)
{
    // REMOTE

    QString remote = "?type=addAsset";
    QJsonObject obj;
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    obj.insert("statusId",statusId);
    obj.insert("stageId",stageId);
    obj.insert("uuid",uuid);
    obj.insert("comment",comment);
    QJsonDocument json(obj);

    emit message("Submitting asset","remote");
    sendRequest(remote,json);
}

void DBInterface::addAssets(QList<QStringList> assets, QString stageId, QString projectId)
{
    if (assets.count() == 0) return ;

/*    // LOCAL
    emit message("Saving assets","local");
    QString local = "INSERT OR IGNORE INTO assets (name,shortName,statusId,comment,stageId,projectId) VALUES %1 ;";
    QString result = "SELECT DISTINCT id FROM assets WHERE %1 ORDER BY id;";

    //the values for insert
    QStringList assetsValues;
    //the values for select id
    QStringList assetsResults;

    QString stageIdString = QString::number(stageId);
    QString projectIdString = QString::number(projectId);

    //build values
    foreach(QStringList asset,assets)
    {
        QString assetValue = "('" + asset[0] + "','" + asset[1] + "'," + asset[2] + ",'" + asset[3] + "'," + stageIdString + "," + projectIdString + ")";
        assetsValues << assetValue;

        QString assetResult = "( name = '" + asset[0] + "' AND shortName = '" + asset[1] + "' AND projectId = " + projectIdString + " )";
        assetsResults << assetResult;
    }

    //build queries
    local = local.arg(assetsValues.join(","));
    result = result.arg(assetsResults.join(" OR "));

    //insert
    QSqlQuery(local,localDB);

    //get ids
    QSqlQuery qResult(result,localDB);
    QList<int> ids;
    while(qResult.next())
    {
        int id = qResult.value(0).toInt();
        ids << id;
    }

*/



    // REMOTE

    QString remote = "?type=addAssets";
    QJsonObject obj;
    QJsonArray jsonAssets;
    for(int i = 0 ; i < assets.count() ; i++)
    {
        QStringList asset = assets[i];

        QJsonObject jsonAsset;
        jsonAsset.insert("name",asset[0]);
        jsonAsset.insert("shortName",asset[1]);
        jsonAsset.insert("statusId",asset[2]);
        jsonAsset.insert("comment",asset[3]);
        jsonAsset.insert("uuid",asset[4]);
        jsonAssets.insert(jsonAssets.count(),jsonAsset);
    }
    obj.insert("assets",jsonAssets);
    obj.insert("projectId",projectId);
    obj.insert("stageId",stageId);
    QJsonDocument json(obj);

    emit message("Submitting assets","remote");
    sendRequest(remote,json);
}

void DBInterface::updateAsset(QString id, QString name, QString shortName, QString comment)
{
    QString remote = "?type=updateAsset";
    QJsonObject obj;
    obj.insert("assetId",id);
    obj.insert("name",name);
    obj.insert("shortName",shortName);
    obj.insert("comment",comment);

    QJsonDocument json(obj);

    emit message("Updating asset","remote");
    sendRequest(remote,json);
}

void DBInterface::assignAsset(QString assetId, QString shotId)
{
    QString q = "?type=assignAsset";
    QJsonObject obj;
    obj.insert("assetId",assetId);
    obj.insert("shotId",shotId);
    QJsonDocument json(obj);

    emit message("Assigning asset","remote");
    sendRequest(q,json);
}

void DBInterface::assignAssets(QList<QStringList> assignments)
{
    if (assignments.count() == 0) return;

    QString q = "?type=assignAssets";
    QJsonObject obj;
    QJsonArray jsonAssignments;
    foreach(QStringList assignment,assignments)
    {
        QJsonObject jsonAssignment;
        jsonAssignment.insert("assetId",assignment[0]);
        jsonAssignment.insert("shotId",assignment[1]);

        jsonAssignments.insert(jsonAssignments.count(),jsonAssignment);
    }

    obj.insert("assignments",jsonAssignments);
    QJsonDocument json(obj);

    emit message("Assigning assets","remote");
    sendRequest(q,json);
}

void DBInterface::addAssignAssets(QList<QStringList> assets, QString stageId, QString projectId)
{
    if (assets.count() == 0) return ;

    // LOCAL ADD
 /*   emit message("Saving assets","local");
    QString local = "INSERT OR IGNORE INTO assets (name,shortName,statusId,comment,stageId,projectId) VALUES %1 ;";
    QString result = "SELECT DISTINCT id FROM assets WHERE %1 ORDER BY id;";

    //the values for insert
    QStringList assetsValues;
    //the values for select id
    QStringList assetsResults;

    QString projectIdString = QString::number(projectId);

    //build values
    foreach(QStringList asset,assets)
    {
        QString assetValue = "('" + asset[0] + "','" + asset[1] + "'," + asset[2] + ",'" + asset[3] + "'," + stageId + "," + projectIdString + ")";
        assetsValues << assetValue;

        QString assetResult = "( name = '" + asset[0] + "' AND shortName = '" + asset[1] + "' AND projectId = " + projectIdString + " )";
        assetsResults << assetResult;
    }

    //build queries
    local = local.arg(assetsValues.join(","));
    result = result.arg(assetsResults.join(" OR "));

    //insert
    QSqlQuery(local,localDB);

    //get ids
    QSqlQuery qResult(result,localDB);
    QList<int> ids;
    while(qResult.next())
    {
        int id = qResult.value(0).toInt();
        ids << id;
    }

*/


    // REMOTE ADD AND ASSIGN

    QString remote = "?type=addAssignAssets";
    QJsonObject obj;
    QJsonArray jsonAssets;
    for(int i = 0 ; i < assets.count() ; i++)
    {
        QStringList asset = assets[i];

        QJsonObject jsonAsset;
        jsonAsset.insert("name",asset[0]);
        jsonAsset.insert("shortName",asset[1]);
        jsonAsset.insert("statusId",asset[2]);
        jsonAsset.insert("comment",asset[3]);
        jsonAsset.insert("uuid",asset[5]);
        jsonAsset.insert("shotId",asset[4]);
        jsonAssets.insert(jsonAssets.count(),jsonAsset);
    }
    obj.insert("assets",jsonAssets);
    obj.insert("projectId",projectId);
    obj.insert("stageId",stageId);
    QJsonDocument json(obj);

    emit message("Submitting assets","remote");
    sendRequest(remote,json);
}

void DBInterface::unAssignAsset(QString assetId, QString shotId)
{
    QString q = "?type=unAssignAsset";
    QJsonObject obj;
    obj.insert("assetId",assetId);
    obj.insert("shotId",shotId);
    QJsonDocument json(obj);

    emit message("un-assigning asset","remote");
    sendRequest(q,json);
}

void DBInterface::getAssets(QString projectId)
{
    QString q = "?type=getAssets";
    QJsonObject obj;
    obj.insert("projectId",projectId);
    QJsonDocument json(obj);

    emit message("Requesting assets","remote");
    sendRequest(q,json);
}

void DBInterface::setAssetStatus(QString statusId, QString assetId)
{
    QString q = "?type=setAssetStatus";
    QJsonObject obj;
    obj.insert("statusId",statusId);
    obj.insert("assetId",assetId);
    QJsonDocument json(obj);

    emit message("Updating asset status","remote");
    sendRequest(q,json);
}

void DBInterface::removeAsset(QString id)
{
    QString q = "?type=removeAsset";
    QJsonObject obj;
    obj.insert("assetId",id);
    QJsonDocument json(obj);

    emit message("Removing asset","remote");
    sendRequest(q,json);
}


