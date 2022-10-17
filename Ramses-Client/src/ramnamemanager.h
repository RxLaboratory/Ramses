#ifndef RAMNAMEMANAGER_H
#define RAMNAMEMANAGER_H

#include "duqf-utils/utils.h"

class RamNameManager
{
public:
    RamNameManager();

    /**
     * @brief originalFileName Gets the filname as it was when set with setFileName()
     * @return
     */
    QString originalFileName() const;
    /**
     * @brief fileName Gets the filename constructed with the parameters
     * @return
     */
    QString fileName() const;
    /**
     * @brief setFileName Sets a path or name to init the manager
     * @param name
     * @return
     */
    bool setFileName(QString name);
    bool setFileName(const QFileInfo &name);

    // Get results
    QString project() const;
    QString type() const;
    QString shortName() const;
    QString step() const;
    QString state() const;
    QString resource() const;
    int version() const;
    /**
     * @brief extension The extension without the leading "."
     * @return
     */
    QString extension() const;

    // Setters
    void setProject(const QString &proj);
    void setType(const QString &type);
    void setShortName(const QString &shortName);
    void setStep(const QString &step);
    void setState(const QString &state);
    void setResource(const QString &resource);
    void setVersion(const int &version);
    void setExtension(const QString &extension);

    // Utils - Constants
    static const QString ShotType;
    static const QString ShotTemplateType;
    static const QString AssetType;
    static const QString AssetTemplateType;
    static const QString GeneralType;
    static const QString GeneralTemplateType;
    static const QString MetaDataFileName;

private:
    void init();

    // Regexes
    QRegularExpression m_reName;

    QString m_fileName;

    // Results
    QString m_project;
    QString m_ramType;
    QString m_shortName;
    QString m_step;
    QString m_resource;
    QString m_state;
    int m_version;
    QString m_extension;
};

#endif // RAMNAMEMANAGER_H
