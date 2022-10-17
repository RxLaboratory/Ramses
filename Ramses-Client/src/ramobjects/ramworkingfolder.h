#ifndef RAMWORKINGFOLDER_H
#define RAMWORKINGFOLDER_H

/**
 * @brief The RamFolder class represents a working folder in the Ramses tree,
 * with its versions, published and preview folder
 */
class RamWorkingFolder
{
public:
    /**
     * @brief RamFolder Constructor
     * @param path The path to the folder
     */
    explicit RamWorkingFolder(QString path);
    const QString &path() const;

    /**
     * @brief isPublished checks if the file has been published
     * @param version is the specific version which has to be published. If 0, will look for *any* version.
     * @param resource checks a specific resource
     * @return true if the file is published
     */
    bool isPublished(int version = 0, QString resource = "") const;

    // <=== General Files and Folders ===>

    /**
     * @brief files Gets the file paths in the subfolder
     * @param subPath
     * @return
     */
    QStringList files(QString subPath = "") const;
    /**
     * @brief files Gets the files in the subfolder
     * @param subPath
     * @return
     */
    QFileInfoList fileInfos(QString subPath = "") const;
    /**
     * @brief folders Gets the folder paths in the subfolder
     * @param subPath
     * @return
     */
    QStringList folders(QString subPath = "") const;
    /**
     * @brief folderInfos Gets the folders in the subfolder
     * @param subPath
     * @return
     */
    QFileInfoList folderInfos(QString subPath = "") const;
    /**
     * @brief folderInfos Gets the folder names in the subfolder
     * @param subPath
     * @return
     */
    QStringList folderNames(QString subPath = "") const;

    // <=== Working Files ===>

    /**
     * @brief defaultWorkingFile The default file (the one without resource string)
     * @return The path to the file, an empty string if not found
     */
    QString defaultWorkingFile() const;
    /**
     * @brief workingFiles Gets the available working files
     * @return The list of paths to the files
     */
    QStringList workingFiles() const;
    /**
     * @brief workingFileInfos Gets the available working files
     * @return The list of files
     */
    QFileInfoList workingFileInfos() const;

    // <=== Published Files ===>

    /**
     * @brief publishedFiles Gets the latest published files
     * @param resource The resource to get
     * @param version A specific version, or 0 for the latest one
     * @return The file paths
     */
    QStringList publishedFiles(QString resource = "", int version = 0) const;
    /**
     * @brief publishedVersionFolders Gets the version subfolders of the published folder
     * @return
     */
    QStringList publishedVersionFolderNames() const;
    /**
     * @brief publishedVersionFolderInfos Gets the version subfolders of the published folder
     * @return
     */
    QFileInfoList publishedVersionFolderInfos() const;

    // <=== Version Files ===>

    /**
     * @brief versionFiles Gets the versions of all resources
     * @param resource
     * @return
     */
    QStringList versionFiles() const;
    /**
     * @brief versionFiles Gets the versions of the given resource
     * @param resource
     * @return
     */
    QStringList versionFiles(QString resource) const;
    /**
     * @brief versionFileInfos Gets the versions of all resources
     * @param resource
     * @return
     */
    QFileInfoList versionFileInfos() const;
    /**
     * @brief versionFileInfos Gets the versions of the given resource
     * @param resource
     * @return
     */
    QFileInfoList versionFileInfos(QString resource) const;
    /**
     * @brief latestVersion Checks the highets available version for all resources
     * @return
     */
    int latestVersion() const;
    /**
     * @brief latestVersion Checks the highets available version for the given resource
     * @return
     */
    int latestVersion(QString resource) const;

    // <=== Preview Files ===>

    /**
     * @brief previewFiles Gets the files used to previewing this work
     * @return
     */
    QStringList previewFiles() const;

    /**
     * @brief previewFileInfos Gets the files used to previewing this work
     * @return
     */
    QFileInfoList previewFileInfos() const;

    // <=== Path Getters ===>

    /**
     * @brief publishPath gets the paths for published resources
     * @return
     */
    QString publishPath() const;
    /**
     * @brief publishFolderInfo gets the paths for published resources
     * @return
     */
    QFileInfo publishFolderInfo() const;
    /**
     * @brief previewPath gets the path for preview files
     * @return
     */
    QString previewPath() const;
    /**
     * @brief previewFolderInfo gets the path for preview files
     * @return
     */
    QFileInfo previewFolderInfo() const;
    /**
     * @brief versionsPath gets the path for version history
     * @return
     */
    QString versionsPath() const;
    /**
     * @brief versionsFolderInfo gets the path for version history
     * @return
     */
    QFileInfo versionsFolderInfo() const;

public:
    static QStringList listFiles(QString absolutePath);
    static QFileInfoList listFileInfos(QString absolutePath);
    static QStringList listFolders(QString absolutePath);
    static QStringList listFolderNames(QString absolutePath);
    static QFileInfoList listFolderInfos(QString absolutePath);


private:
    QString m_path;

};

bool versionFolderSorter(QString a, QString b);
bool versionFolderInfoSorter(QFileInfo a, QFileInfo b);
int getVersionFolderRank(QString folder);

inline bool operator==(const RamWorkingFolder &wf1, const RamWorkingFolder &wf2)
{
    return wf1.path() == wf2.path();
}

inline uint qHash(const RamWorkingFolder &wf, uint seed)
{
    return qHash(wf.path(), seed);
}


#endif // RAMWORKINGFOLDER_H
