#ifndef RAMFILEMETADATAMANAGER_H
#define RAMFILEMETADATAMANAGER_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QFileInfo>
#include <QVariant>
#include <QtGlobal>

#include "ramnamemanager.h"

class RamFileMetaDataManager: QObject
{
    Q_OBJECT
public:
    enum MetaDataKey { Comment,
                     Version,
                     VersionFile,
                     PipeType,
                     ModificationHistory};
    Q_ENUM( MetaDataKey )

    RamFileMetaDataManager(QString folderPath, QObject *parent = nullptr);
    void writeMetaData();
    bool isValid() const;

    QVariant getValue(QString fileName, MetaDataKey key) const;
    QVariant getValue(QString fileName, QString key) const;
    void setValue(QString fileName, MetaDataKey key, QVariant value);
    void setValue(QString fileName, QString key, QVariant value);

    QString getComment(QString fileName) const;
    void setComment(QString fileName, QString comment);

    int getVersion(QString fileName) const;
    void setVersion(QString fileName, int version);

    QString getVersionFilePath(QString fileName) const;
    void setVersionFilePath(QString fileName, QString versionFilePath);

    QString getPipeType(QString fileName) const;
    void setPipeType(QString fileName, QString pipeType);


    QVector<qint64> getModificationHistory(QString fileName) const;
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    // Note: QDateTime(QDate()) is deprecated on Qt 5.15 but Linux version still uses 5.12
    qint64 getTimeRange(QString fileName, QDateTime after = QDateTime(QDate(1970,1,1))) const;
    int getTimeRange( QDateTime after = QDateTime(QDate(1970,1,1)) ) const;
#else
    qint64 getTimeRange(QString fileName, QDateTime after = QDate(1970,1,1).startOfDay() ) const;
    int getTimeRange( QDateTime after = QDate(1970,1,1).startOfDay() ) const;
#endif

private:
    QDir m_folder;
    QFile *m_metaDataFile;
    QJsonDocument m_metaData;
    bool m_valid = true;

};

#endif // RAMFILEMETADATAMANAGER_H
