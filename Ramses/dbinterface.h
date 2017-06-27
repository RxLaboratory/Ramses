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

class DBInterface : public QObject
{
    Q_OBJECT
public:
    explicit DBInterface(QObject *parent = 0);
    //parameters
    void setServerAddress(QString address);
    void setSsl(bool ssl);
    void setUpdateFreq(int freq);
    //connection
    void connection(QString user,QString passHash);
    void sendRequest(QString request, QJsonDocument content = QJsonDocument());
    //Status
    void addStatus(QString name = "New status",QString shortName = "New",QString color = "6d6d6d",QString description = "");
    void getStatuses();
    void updateStatus(int id, QString name, QString shortName, QString color, QString description);
    void removeStatus(int id);
    //Stage
    void addStage(QString name = "New stage",QString shortName = "New",QString type = "s");
    void getStages();
    void updateStage(int id, QString name, QString shortName);
    void removeStage(int id);
    //Project
    void addProject(QString name = "New project",QString shortName = "New");
    void getProjects();
    void updateProject(int id, QString name, QString shortName);
    void removeProject(int id);
    void addProjectStage(int projectId, int stageId);
    void removeProjectStage(int projectId, int stageId);
    //Shot
    void addShots(int projectId, int statusId, QStringList shots, int order);
    void addShots(int projectId, int statusId, QList<QStringList> shots, int order);
    void getShots(int projectId);
    void updateShot(int id, QString name, double duration);
    void setStageStatus(int statusId,int stageId,int shotId);
    void setStageComment(QString comment,int stageId,int shotId);
    void removeShots(QList<int> ids);
    void moveShotsUp(QList<int> ids);
    void moveShotsDown(QList<int> ids);
    //Asset
    void addAsset(QString name, QString shortName, int stageId, int statusId, int shotId = -1);
    void setAssetStatus(int statusId, int assetId);
    void assignAsset(int assetId,int shotId);
signals:
    void connecting();
    void connected(bool,QString);
    void message(QString,int i = 0);
    //Status
    void statusAdded(bool,QString);
    void gotStatuses(bool,QString,QJsonValue);
    void statusUpdated(bool,QString);
    void statusRemoved(bool,QString);
    //Stage
    void stageAdded(bool,QString);
    void gotStages(bool,QString,QJsonValue);
    void stageUpdated(bool,QString);
    void stageRemoved(bool,QString);
    //Project
    void projectAdded(bool,QString);
    void gotProjects(bool,QString,QJsonValue);
    void projectUpdated(bool,QString);
    void projectRemoved(bool,QString);
    void projectStageAdded(bool,QString);
    void projectStageRemoved(bool,QString);
    //Shot
    void shotAdded(bool,QString);
    void gotShots(bool,QString,QJsonValue);
    void shotUpdated(bool,QString);
    void stageStatusUpdated(bool,QString);
    void stageCommentUpdated(bool,QString);
    void shotRemoved(bool,QString);
    void shotsMovedUp(bool,QString);
    void shotsMovedDown(bool,QString);
    //Asset
    void assetAdded(bool,QString);
    void assetStatusUpdated(bool,QString);
    void assetAssigned(bool,QString);

public slots:
    void dataReceived(QNetworkReply *rep);
    void networkError(QNetworkReply::NetworkError err);
    void sslError(QNetworkReply *rep, QList<QSslError> errs);
private:
    //PHP API
    QString serverAddress; //server
    QString protocol; //use SSL
    int updateFreq; //update freq in minutes
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    //QSqlDatabase localDB; //SQLite used for offline mode and sync
};

#endif // BDDINTERFACE_H
