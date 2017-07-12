#include "ramuser.h"

RAMUser::RAMUser(DBInterface *db,QString user,bool updateDb,QString password, QString id, QString first, QString last, QObject *parent) : QObject(parent)
{
    dbi = db;
    username = user;
    uuid = id;
    firstname = first;
    lastname = last;

    passHash = generatePassHash(password);

    if (uuid == "") uuid = RAMUuid::generateUuidString(username);

    //TODO update db

    connect(dbi,SIGNAL(data(QJsonObject)),this,SLOT(gotData(QJsonObject)));
}

QString RAMUser::getUsername()
{
    return username;
}

QString RAMUser::getFirstname()
{
    return firstname;
}

QString RAMUser::getLastname()
{
    return lastname;
}

QString RAMUser::getPassHash()
{
    return passHash;
}

QString RAMUser::getId()
{
    return uuid;
}

void RAMUser::setUsername(QString user)
{
    username = user;
}

void RAMUser::setFirstname(QString first)
{
    firstname = first;
}

void RAMUser::setLastname(QString last)
{
    lastname = last;
}

void RAMUser::setPassword(QString pass)
{
    passHash = generatePassHash(pass);
}

void RAMUser::login()
{
    dbi->connection(username,passHash);
}

void RAMUser::update()
{
    dbi->updateUser(uuid,username,firstname,lastname,passHash);
}

void RAMUser::gotData(QJsonObject data)
{
    //extract data
    QString type = data.value("type").toString();
    bool accepted = data.value("accepted").toBool();
    bool success = data.value("success").toBool();
    QJsonValue content = data.value("content");

    //if not accepted or unsuccessful, return
    if (!accepted || !success)
    {
        return;
    }

    // LOGIN
    if (type == "login" && content.toObject().count() > 0)
    {
        connected(content);
        return;
    }
}

void RAMUser::connected(QJsonValue mess)
{
    QJsonObject jsonUser = mess.toObject();
    firstname = jsonUser.value("firstName").toString();
    lastname = jsonUser.value("lastName").toString();
    uuid = jsonUser.value("uuid").toString();
}

QString RAMUser::generatePassHash(QString pass)
{
    //hash password //TODO Use UUID of User as salt
    QString salt = "salt";
    QString passToHash = salt + pass;
    return QCryptographicHash::hash(passToHash.toUtf8(), QCryptographicHash::Sha3_512).toHex();
}

