#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include <QObject>
#include <QSqlDatabase>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkCookieJar>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>

class DBInterface : public QObject
{
    Q_OBJECT
public:
    explicit DBInterface(QObject *parent = 0);
    //parameters
    void setServerAddress(QString address);
    void setSsl(bool ssl);
    //connection
    void connection(QString user,QString passHash);
    void sendRequest(QString request, QJsonDocument content = QJsonDocument());
    //Status
    void addStatus(QString name = "New status", QString shortName = "New", QString color = "6d6d6d", QString description = "", QString uuid = "");
    void getStatuses();
    void updateStatus(QString uuid, QString name, QString shortName, QString color, QString description);
    void removeStatus(QString uuid);
    //Stage
    void addStage(QString name = "New stage", QString shortName = "New", QString uuid = "");
    void getStages();
    void updateStage(QString uuid, QString name, QString shortName);
    void removeStage(QString uuid);
    //Project
    void addProject(QString name = "New project",QString shortName = "New", QString id = "");
    void getProjects();
    void updateProject(QString id, QString name, QString shortName);
    void removeProject(QString id);
    void addProjectStage(QString projectId, QString stageId);
    void removeProjectStage(QString projectId, QString stageId);
    //Shot
    void addShot(QString id, QString name, double duration);
    void addShots(QList<QStringList> shots);
    void updateShots(QList<QStringList> shots);
    void insertShots(QStringList shots, QString projectId, int order);
    void insertShot(QString id, QString projectId, int order);
    void addInsertShots(QList<QStringList> shots, QString projectId, int order);
    void getShots(QString projectId);
    void updateShot(QString id, QString name, double duration);
    void removeShot(QString id);
    void removeShots(QStringList ids, QString projectId);
    void resetShotsOrder(QStringList ids);
    //Asset
    void addAsset(QString name, QString shortName, QString statusId, QString stageId, QString comment, QString uuid);
    void addAssets(QList<QStringList> assets, QString stageId, QString projectId);
    void updateAsset(QString id, QString name, QString shortName, QString comment);
    void assignAsset(QString assetId, QString shotId);
    void assignAssets(QList<QStringList> assignments);
    void addAssignAssets(QList<QStringList> assets, QString stageId, QString projectId);
    void unAssignAsset(QString assetId, QString shotId);
    void getAssets(QString projectId);
    void setAssetStatus(QString statusId, QString assetId);
    void removeAsset(QString id);
signals:
    void connecting();
    void connected(bool,QString);
    void message(QString,QString type);
    void data(QJsonObject d);

public slots:
    void dataReceived(QNetworkReply *rep);
    void networkError(QNetworkReply::NetworkError err);
    void sslError(QNetworkReply *rep, QList<QSslError> errs);
private:
    //LOCAL DB
    QSqlDatabase localDB;
    //PHP API
    QString serverAddress; //server
    QString protocol; //use SSL
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    //QSqlDatabase localDB; //SQLite used for offline mode and sync
};

#endif // BDDINTERFACE_H
