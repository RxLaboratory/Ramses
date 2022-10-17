#ifndef RAMABSTRACTOBJECT_H
#define RAMABSTRACTOBJECT_H

#include <QSettings>

/**
 * @brief The RamAbstractObject class is the base class for RamObject and RamObjectList
 */
class RamAbstractObject
{
public:

    // ENUMS //

    /**
     * @brief The DataRole enum lists all available data got from the RamObjectModel
     */
    enum DataRole {
        UUID = Qt::UserRole,
        ShortName = Qt::UserRole+1,
        Name = Qt::UserRole+2,
        Completion = Qt::UserRole+3,
        Type = Qt::UserRole+4,
        Details = Qt::UserRole+5,
        Disabled = Qt::UserRole+6,
        Path = Qt::UserRole+7,
        Comment = Qt::UserRole+8,
        Lateness = Qt::UserRole+9,
        Estimation = Qt::UserRole+10,
        Goal = Qt::UserRole+11,
        TimeSpent = Qt::UserRole+12,
        ProgressColor = Qt::UserRole+13,
        LabelColor = Qt::UserRole+14,
        PreviewImagePath = Qt::UserRole+15,
        SubDetails = Qt::UserRole+16,
        SizeHint = Qt::UserRole+17,
        DetailedSizeHint = Qt::UserRole+18,
        IsPM = Qt::UserRole+19,
        Date = Qt::UserRole+20,
        IsComment = Qt::UserRole+21,
        Difficulty = Qt::UserRole+23,
        Duration = Qt::UserRole+24,
        Pointer = Qt::UserRole+25,
        Order = Qt::UserRole+26
    };
    /**
     * @brief The ObjectType enum lists all types of RamObjects
     */
    enum ObjectType {
                    Application,
                    Asset,
                    AssetGroup,
                    FileType,
                    Object,
                    Item,
                    Pipe,
                    PipeFile,
                    Project,
                    Sequence,
                    Shot,
                    State,
                    Status,
                    Step,
                    User,
                    ScheduleEntry,
                    ScheduleComment,
                    TemplateStep,
                    TemplateAssetGroup,
                    Ramses
                    };
    /**
     * @brief The SubFolder enum lists all predefined subfolders
     */
    enum SubFolder { NoFolder,
                   ConfigFolder,
                   AdminFolder,
                   PreProdFolder,
                   ProdFolder,
                   PostProdFolder,
                   AssetsFolder,
                   ShotsFolder,
                   ExportFolder,
                   TemplatesFolder,
                   PublishFolder,
                   VersionsFolder,
                   PreviewFolder,
                   UsersFolder,
                   ProjectsFolder,
                   TrashFolder,
                   DataFolder };
    /**
     * @brief The UserRole enum lists the available roles for users, and the level of their rights
     */
    enum UserRole { Admin = 3,
                    ProjectAdmin = 2,
                    Lead = 1,
                    Standard = 0 };

    // STATIC METHODS //

    /**
     * @brief objectTypeName gets the name of a type, as used in the Database and API classes
     * @param type
     * @return
     */
    static const QString objectTypeName(ObjectType type);
    static ObjectType objectTypeFromName(QString name);

    /**
     * @brief subFolderName gets the actual name of a subfolder
     * @param folder
     * @return
     */
    static const QString subFolderName(SubFolder folder);

    static void setObjectData(QString uuid, QString dataStr);
    static void setObjectData(QString uuid, QJsonObject data);
    static QJsonObject getObjectData(QString uuid);
    static QString getObjectDataString(QString uuid);
    static QString getObjectPath(QString uuid);

    static const QString uuidFromPath(QString path, ObjectType type);

    // Checks if this is a valid uuid,
    // and if it exists in the DB
    static bool checkUuid(QString uuid, ObjectType type, bool mayBeVirtual = false);

    static QSet<RamAbstractObject*> invalidObjects();
    static void removeInvalidObjects();

    // METHODS //

    RamAbstractObject(QString shortName, QString name, ObjectType type, bool isVirtual = false, bool encryptData = false);
    ~RamAbstractObject();

    bool is(RamAbstractObject *other) const;

    /**
     * @brief uuid is this object's uuid
     * @return
     */
    QString uuid() const;

    /**
     * @brief isValid checks if the object has valid data. If not, it means the data in the database is corrupted,
     * And this object should be ignored or deactivated.
     * @return
     */
    bool isValid() const;
    void invalidate();

    /**
     * @brief objectType the type of ramobject
     * @return
     */
    ObjectType objectType() const;
    QString objectTypeName() const;

    /**
     * @brief data gets the data from the database
     * @return
     */
    QJsonObject data() const;
    /**
     * @brief getData returns the data for a specific key
     * @return
     */
    QJsonValue getData(QString key) const;
    /**
     * @brief setData sets an entirely new data
     * @param data
     */
    void setData(QJsonObject data);
    /**
     * @brief insertData inserts or updates a value in the data
     * @param key
     * @param value
     */
    void insertData(QString key, QJsonValue value);

    /**
     * @brief shortName the identifier of the object
     * @return
     */
    virtual QString shortName() const;
    virtual void setShortName(const QString &shortName);
    virtual bool validateShortName(const QString &shortName);

    /**
     * @brief name the user-friendly name of the object
     * @return
     */
    virtual QString name() const;
    void setName(const QString &name);

    /**
     * @brief comment is a comment associated to this object. Plain text or markdown
     * @return
     */
    QString comment() const;
    void setComment(const QString comment);

    virtual QColor color() const;
    void setColor(QColor color);

    int order() const;
    void setOrder(int o);

    QString customSettings() const;
    void setCustomSettings(const QString &newCustomSettings);

    QIcon icon() const;
    virtual QString iconName() const { return m_icon; };
    QPixmap iconPixmap() const;

    virtual QString details() const { return QString(); };
    virtual QString subDetails() const { return QString(); };

    virtual bool isDisabled() const { return false; }

    virtual QVariant roleData(int role) const;

    /**
     * @brief remove marks the object as removed in the database
     */
    virtual void remove();
    /**
     * @brief restore markes the object as available in the database
     */
    void restore();
    bool isRemoved();

    /**
     * @brief settings are the settings corresponding to this object
     * @return
     */
    QSettings *settings();
    void reInitSettingsFile();

    QString path(SubFolder subFolder = NoFolder, bool create = false) const;
    QString path(SubFolder subFolder, QString subPath, bool create = false) const;
    QStringList listFiles(SubFolder subFolder = NoFolder, QString subPath = "") const;
    QFileInfoList listFileInfos(SubFolder subFolder = NoFolder, QString subPath = "") const;
    QStringList listFolders(SubFolder subFolder = NoFolder) const;
    QStringList listFolders(SubFolder subFolder, QString subPath) const;
    void deleteFile(QString fileName, SubFolder folder=NoFolder) const;
    void revealFolder(SubFolder subFolder = NoFolder);
    virtual QString previewImagePath() const;

    // Low level data handling.
    QString dataString() const;
    void setDataString(QString data);

    virtual void emitDataChanged() {};

    void suspendSave(bool suspend);
    bool isSaveSuspended() const;

protected:

    // METHODS //

    /**
     * @brief RamAbstractObject constructs an existing object from an existing uuid. This method is private, use obj(QString) to get an object from the uuid
     * @param uuid
     */
    RamAbstractObject(QString uuid, ObjectType type, bool encryptData = false);

    virtual QJsonObject reloadData() = 0;

    // SIGNALS in QObject instances
    virtual void emitRemoved() = 0;
    virtual void emitRestored() = 0;

    // UTILS
    static QHash<QString, RamAbstractObject*> m_allObjects;
    static QSet<RamAbstractObject*> m_invalidObjects;
    /**
     * @brief folderPath the folder of this object
     * @return
     */
    virtual QString folderPath() const = 0;

    // ATTRIBUTES //

    QString m_uuid;
    ObjectType m_objectType;
    bool m_virtual = false;
    bool m_saveSuspended = false;
    bool m_dataEncrypted = false;
    QString m_cachedData = "";
    bool m_savingData = false;
    QString m_icon = ":/icons/asset";

    static QHash<QString, QPixmap> m_iconPixmaps;
    static QPixmap iconPixmap(QString iconName);
    static QRegularExpression m_rxsn;
    static QRegularExpression shortNameRegularExpression();

private:
    void construct();
    void createData(QString data);

    QSettings *m_settings = nullptr;
    bool m_valid = true;
};

#endif // RAMABSTRACTOBJECT_H
