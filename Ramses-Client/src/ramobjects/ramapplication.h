#ifndef RAMAPPLICATION_H
#define RAMAPPLICATION_H

#include "ramobject.h"

#include "ramobjectmodel.h"
#include "ramfiletype.h"

class RamApplication : public RamObject
{
    Q_OBJECT
public:

    // STATIC //

    static RamApplication *get(QString uuid);
    static RamApplication *c(RamObject *o);

    // OTHER //

    /**
     * @brief RamApplication creates a new Application in the database
     * @param shortName
     * @param name
     */
    RamApplication(QString shortName, QString name);

    QString executableFilePath() const;
    void setExecutableFilePath(const QString &executableFilePath);

    RamObjectModel *nativeFileTypes() const;
    RamObjectModel *importFileTypes() const;
    RamObjectModel *exportFileTypes() const;

    bool canExportFileType(RamFileType *ft) const;
    bool canExportFileType(QString extension) const;
    bool canImportFileType(RamFileType *ft) const;
    bool canImportFileType(QString extension) const;

    bool canOpen(QString filePath) const;
    bool open(QString filePath) const;

public slots:
    void unassignFileType(RamFileType *ft);
    virtual void edit(bool show = true) override;

protected:
    static QHash<QString, RamApplication*> m_existingObjects;
    /**
     * @brief RamApplication constructs a RamApplication from the database
     * @param uuid
     */
    RamApplication(QString uuid);
    virtual QString folderPath() const override;

    static QFrame *ui_applicationWidget;

private:
    void construct();

    RamObjectModel *m_nativeFileTypes;
    RamObjectModel *m_importFileTypes;
    RamObjectModel *m_exportFileTypes;
};

#endif // RAMAPPLICATION_H
