#include "ramworkingfolder.h"

#include "ramobject.h"
#include "ramnamemanager.h"

RamWorkingFolder::RamWorkingFolder(QString path)
{
    m_path = path;
    if (!m_path.endsWith("/") || !m_path.endsWith("\\")) m_path += "/";
}

const QString &RamWorkingFolder::path() const
{
    return m_path;
}

bool RamWorkingFolder::isPublished(int version, QString resource) const
{
    // List content
    QDir dir( publishPath() );
    QStringList subdirs = dir.entryList( QDir::Dirs | QDir::NoDotAndDotDot );
    for (int i = 0; i < subdirs.count(); i++)
    {
        QString dirname = subdirs.at(i);
        QStringList splitDirname = dirname.split("_");
        QString res = "";
        int v  = 0;
        if (splitDirname.count() == 3)
        {
            res = splitDirname.at(0);
            v = splitDirname.at(1).toInt();
        }
        else
        {
            v = splitDirname.at(0).toInt();
        }

        if (resource == res)
        {
            // Found the version
            if (version > 0 && version == v) return true;
            // Don't check the version
            if (version == 0) return true;
        }
    }
    return false;
}

QStringList RamWorkingFolder::files(QString subPath) const
{
    return listFiles(m_path + subPath);
}

QFileInfoList RamWorkingFolder::fileInfos(QString subPath) const
{
    return listFileInfos(m_path + subPath);
}

QStringList RamWorkingFolder::folders(QString subPath) const
{
    return listFolders(m_path + subPath);
}

QFileInfoList RamWorkingFolder::folderInfos(QString subPath) const
{
    return listFolderInfos(m_path + subPath);
}

QStringList RamWorkingFolder::folderNames(QString subPath) const
{
    return listFolderNames(m_path + subPath);
}

QString RamWorkingFolder::defaultWorkingFile() const
{
    QStringList files = workingFiles();
    RamNameManager nm;
    foreach( QString file, files)
    {
        nm.setFileName(file);
        if (nm.resource() == "") return file;
    }
    return "";
}

QStringList RamWorkingFolder::workingFiles() const
{
    return files();
}

QFileInfoList RamWorkingFolder::workingFileInfos() const
{
    return fileInfos();
}

QStringList RamWorkingFolder::publishedFiles(QString resource, int version) const
{
    // List content
    QString path = publishPath();
    QDir dir( path );
    QStringList subdirs = dir.entryList( QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase );
    QStringList publishedFiles;
    for (int i = subdirs.count() - 1; i >= 0; i--)
    {
        QString dirname = subdirs.at(i);
        QStringList splitDirname = dirname.split("_");
        int v = 0;
        QString res = "";
        if (splitDirname.count() == 3)
        {
            res = splitDirname.at(0);
            v = splitDirname.at(1).toInt();
        }
        else
        {
            v = splitDirname.at(0).toInt();
        }
        if (res == resource)
        {
            // Check version
            if (version == 0 || version == v)
            {
                // list files
                publishedFiles = listFiles( path + dirname );
                if (publishedFiles.count() > 0) return publishedFiles;
            }
        }
    }
    return publishedFiles;
}

QStringList RamWorkingFolder::publishedVersionFolderNames() const
{
    // Sort before returning
    QStringList folders = folderNames( publishPath() );
    std::sort(folders.begin(), folders.end(), versionFolderSorter);
    return folders;
}

QFileInfoList RamWorkingFolder::publishedVersionFolderInfos() const
{
    // Sort before returning
    QFileInfoList folders = listFolderInfos( publishPath() );
    std::sort(folders.begin(), folders.end(), versionFolderInfoSorter);
    return folders;
}

QStringList RamWorkingFolder::versionFiles() const
{
    return listFiles(versionsPath());
}

QStringList RamWorkingFolder::versionFiles(QString resource) const
{
    // Get current information
    RamNameManager nm;
    nm.setFileName(m_path);

    // look for files with the same resource
    QStringList files;
    RamNameManager filenm;
    foreach(QString file, versionFiles())
    {
        if (nm.setFileName(file))
        {
            if (filenm.project().toLower() != nm.project().toLower()) continue;
            if (filenm.step().toLower() != nm.step().toLower()) continue;
            if (filenm.shortName().toLower() != nm.shortName().toLower()) continue;
            if (filenm.resource() == resource) files << file;
        }
    }
    return files;
}

QFileInfoList RamWorkingFolder::versionFileInfos() const
{
    return listFileInfos(versionsPath());
}

QFileInfoList RamWorkingFolder::versionFileInfos(QString resource) const
{
    // Get current information
    RamNameManager nm;
    nm.setFileName(m_path);

    // look for files with the same resource
    QFileInfoList files;
    RamNameManager filenm;
    foreach(QFileInfo file, versionFileInfos())
    {
        if (filenm.setFileName(file))
        {
            if (filenm.project().toLower() != nm.project().toLower()) continue;
            if (filenm.step().toLower() != nm.step().toLower()) continue;
            if (filenm.shortName().toLower() != nm.shortName().toLower()) continue;
            if (filenm.resource() == resource) files << file;
        }
    }
    return files;
}

int RamWorkingFolder::latestVersion() const
{
    // Get current information
    RamNameManager nm;
    nm.setFileName(m_path);

    // look for files with the same resource
    QFileInfoList files;
    RamNameManager filenm;
    int v = 0;
    foreach(QFileInfo file, versionFileInfos())
    {
        if (nm.setFileName(file))
        {
            if (filenm.project().toLower() != nm.project().toLower()) continue;
            if (filenm.step().toLower() != nm.step().toLower()) continue;
            if (filenm.shortName().toLower() != nm.shortName().toLower()) continue;
            if (filenm.version() > v) v = filenm.version();
        }
    }
    return v;
}

int RamWorkingFolder::latestVersion(QString resource) const
{
    // Get current information
    RamNameManager nm;
    nm.setFileName(m_path);

    // look for files with the same resource
    QFileInfoList files;
    RamNameManager filenm;
    int v = 0;
    foreach(QFileInfo file, versionFileInfos())
    {
        if (nm.setFileName(file))
        {
            if (filenm.project().toLower() != nm.project().toLower()) continue;
            if (filenm.step().toLower() != nm.step().toLower()) continue;
            if (filenm.shortName().toLower() != nm.shortName().toLower()) continue;
            if (filenm.resource() == resource && filenm.version() > v) v = filenm.version();
        }
    }
    return v;
}

QStringList RamWorkingFolder::previewFiles() const
{
    return listFiles(previewPath());
}

QFileInfoList RamWorkingFolder::previewFileInfos() const
{
    return listFileInfos(previewPath());
}

QString RamWorkingFolder::publishPath() const
{
    QString p = m_path + RamObject::subFolderName(RamObject::PublishFolder) + "/";
    return p;
}

QFileInfo RamWorkingFolder::publishFolderInfo() const
{
    return QFileInfo(publishPath());
}

QString RamWorkingFolder::previewPath() const
{
    QString p = m_path + RamObject::subFolderName(RamObject::PreviewFolder) + "/";
    return p;
}

QFileInfo RamWorkingFolder::previewFolderInfo() const
{
    return QFileInfo(previewPath());
}

QString RamWorkingFolder::versionsPath() const
{
    QString p = m_path + RamObject::subFolderName(RamObject::VersionsFolder) + "/";
    return p;
}

QFileInfo RamWorkingFolder::versionsFolderInfo() const
{
    return QFileInfo(versionsPath());
}

QStringList RamWorkingFolder::listFiles(QString absolutePath)
{
    QDir dir(absolutePath);
    QStringList files = dir.entryList( QDir::Files );

    QStringList result;
    for (int i = 0; i < files.count(); i++)
    {
        QString file = files.at(i);
        if (file == RamNameManager::MetaDataFileName) continue;

        result.append(absolutePath + "/" + file);
    }
    return result;
}

QFileInfoList RamWorkingFolder::listFileInfos(QString absolutePath)
{
    QDir dir(absolutePath);
    QFileInfoList files = dir.entryInfoList( QDir::Files );
    // Remove the ramses data file
    for (int i = files.length() - 1; i >= 0; i--)
    {
        if (files.at(i).fileName() == RamNameManager::MetaDataFileName ) files.removeAt(i);
    }
    return files;
}

QStringList RamWorkingFolder::listFolders(QString absolutePath)
{
    QStringList folders = listFolderNames(absolutePath);
    QStringList result;
    for (int i = 0; i < folders.count(); i++)
    {
        QString folder = folders.at(i);

        result.append(absolutePath + "/" + folder);
    }
    return folders;
}

QStringList RamWorkingFolder::listFolderNames(QString absolutePath)
{
    QDir dir(absolutePath);
    return dir.entryList( QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name );
}

QFileInfoList RamWorkingFolder::listFolderInfos(QString absolutePath)
{
    QDir dir(absolutePath);
    QFileInfoList folderInfos = dir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name );
    return folderInfos;
}

bool versionFolderSorter(QString a, QString b)
{
    int ai = getVersionFolderRank(a);
    int bi = getVersionFolderRank(b);
    return ai < bi;
}

bool versionFolderInfoSorter(QFileInfo a, QFileInfo b)
{
    int ai = getVersionFolderRank(a.fileName());
    int bi = getVersionFolderRank(b.fileName());
    return ai < bi;
}

int getVersionFolderRank(QString folder)
{
    QFileInfo folderInfo(folder);
    QString name = folderInfo.baseName();
    QStringList nameBlocks = name.split("_");
    int num = nameBlocks.length();
    // Invalid, return lowest value possible
    if (num == 0 || num  > 3) return INT32_MIN;
    // Single or Dual Block, first should be a number
    if (num == 1 || num == 2)
    {
        bool ok;
        int i = nameBlocks[0].toInt(&ok);
        if (ok) return i;
        return INT32_MIN;
    }
    // We've got a resource
    bool ok;
    int i = nameBlocks[1].toInt(&ok);
    if (!ok) return INT32_MIN;
    // Hash the resource, add the version
    std::hash<std::string> hasher;
    int hashed = hasher(nameBlocks[0].toStdString());
    return hashed + i;
}

