#include "ramfilemetadatamanager.h"

RamFileMetaDataManager::RamFileMetaDataManager(QString folderPath, QObject *parent): QObject(parent)
{
    m_folder = QDir(folderPath);
    if (!m_folder.exists()) m_valid = false;

    m_metaDataFile = new QFile( m_folder.filePath(RamNameManager::MetaDataFileName) );
    if (m_metaDataFile->exists() && m_metaDataFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString content = m_metaDataFile->readAll();
        m_metaData = QJsonDocument::fromJson(content.toUtf8());
        m_metaDataFile->close();
    }
    else
    {
        m_metaData.setObject(QJsonObject());
    }
}

QVariant RamFileMetaDataManager::getValue(QString fileName, RamFileMetaDataManager::MetaDataKey key) const
{
    switch(key)
    {
    case Comment: return getValue(fileName, "comment");
    case Version: return getValue(fileName, "version");
    case VersionFile: return getValue(fileName, "versionFilePath");
    case PipeType: return getValue(fileName, "pipeFileShortName");
    case ModificationHistory: return getValue(fileName, "history");
    }
    return QVariant();
}

QVariant RamFileMetaDataManager::getValue(QString fileName, QString key) const
{
    if (!m_valid) return QVariant();
    QJsonObject fileObj = m_metaData.object().value(fileName).toObject();
    return fileObj.value(key).toVariant();
}

void RamFileMetaDataManager::setValue(QString fileName, RamFileMetaDataManager::MetaDataKey key, QVariant value)
{
    switch(key)
    {
    case Comment: return setValue(fileName, "comment", value);
    case Version: return setValue(fileName, "version", value);
    case VersionFile: return setValue(fileName, "versionFilePath", value);
    case PipeType: return setValue(fileName, "pipeFileShortName", value);
    case ModificationHistory: return setValue(fileName, "history", value);
    }
}

void RamFileMetaDataManager::setValue(QString fileName, QString key, QVariant value)
{
    QJsonObject fileObj = m_metaData.object().value(fileName).toObject();
    fileObj.insert(key, QJsonValue::fromVariant(value));
    m_metaData.object().insert(fileName, fileObj);
}

QString RamFileMetaDataManager::getComment(QString fileName) const
{
    return getValue(fileName, Comment).toString();
}

void RamFileMetaDataManager::setComment(QString fileName, QString comment)
{
    setValue(fileName, Comment, comment);
}

int RamFileMetaDataManager::getVersion(QString fileName) const
{
    return getValue(fileName, Version).toInt();
}

void RamFileMetaDataManager::setVersion(QString fileName, int version)
{
    setValue(fileName, Version, version);
}

QString RamFileMetaDataManager::getVersionFilePath(QString fileName) const
{
    return getValue(fileName, VersionFile).toString();
}

void RamFileMetaDataManager::setVersionFilePath(QString fileName, QString versionFilePath)
{
    setValue(fileName, VersionFile, versionFilePath);
}

QString RamFileMetaDataManager::getPipeType(QString fileName) const
{
    return getValue(fileName, PipeType).toString();
}

void RamFileMetaDataManager::setPipeType(QString fileName, QString pipeType)
{
    setValue(fileName, PipeType, pipeType);
}

QVector<qint64> RamFileMetaDataManager::getModificationHistory(QString fileName) const
{
    QVariantList history = getValue(fileName, ModificationHistory).toList();

    QVector<qint64> h;
    for (int i =0; i < history.count(); i++)
    {
        h << history.at(i).toULongLong();
    }
    std::sort(h.begin(), h.end());
    return h;
}

qint64 RamFileMetaDataManager::getTimeRange(QString fileName, QDateTime after) const
{
    QVector<qint64> fileHistory = getModificationHistory(fileName);
    if (fileHistory.count() == 0)
    {
        // if the file exists, consider there's been half an hour
        if (QFileInfo::exists(m_folder.filePath(fileName)))
            return 1800;
        return 0;
    }

    // Pick the creation date as starting point
    QDateTime current;
    QFileInfo fileInfo( m_folder.filePath(fileName) );
    if (fileInfo.exists())
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
        current = fileInfo.created();
#else
        current = fileInfo.birthTime();
#endif
    // If not found, pick the first history item
    if (!current.isValid())
        current = QDateTime::fromSecsSinceEpoch( fileHistory.at(0) );

    // an extra half hour no matter what (time before first save)
    qint64 range = 1800;

    // get the range, IF there's less than 2 hours between each entry.
    for(int i = 0; i < fileHistory.count(); i++)
    {
        QDateTime newDate = QDateTime::fromSecsSinceEpoch( fileHistory.at(i) );
        if (newDate < after)
        {
            current = newDate;
            continue;
        }

        qint64 r = current.secsTo( newDate );

        // if it's less than 2 hours, consider we've been working on it full time, add the range
        if (r < 7200) range += r;
        // otherwise, it's a first save, consider it's happening half an hour after opening the file
        else range += 1800;
    }
    return range;
}

int RamFileMetaDataManager::getTimeRange(QDateTime after) const
{
    // Get the time range for all files in the folder
    QStringList files = m_folder.entryList(QDir::Files);
    qint64 range = 0;
    foreach( QString file, files)
    {
        if (file == RamNameManager::MetaDataFileName) continue;
        range += getTimeRange( file, after);
    }
    return range;
}

void RamFileMetaDataManager::writeMetaData()
{
    if (!m_valid) return;
    if (m_metaDataFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        m_metaDataFile->write( m_metaData.toJson(QJsonDocument::Indented));
        m_metaDataFile->close();
    }
}

bool RamFileMetaDataManager::isValid() const
{
    return m_valid;
}
