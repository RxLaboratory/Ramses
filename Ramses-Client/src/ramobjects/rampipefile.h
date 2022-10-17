#ifndef RAMPIPEFILE_H
#define RAMPIPEFILE_H

#include <QObject>

#include "ramobject.h"
#include "ramproject.h"
#include "ramfiletype.h"

class RamPipeFile : public RamObject
{
    Q_OBJECT
public:

    // STATIC //

    static RamPipeFile *get(QString uuid);
    static RamPipeFile *c(RamObject *o);

    // OTHER //

    RamPipeFile(QString shortName, RamProject *project);

    QString name() const override;

    RamFileType *fileType() const;
    void setFileType(RamFileType *newFileType);

    const RamProject *project() const;

    QString customSettings() const;
    void setCustomSettings(const QString &newCustomSettings);

    virtual QString details() const override;

public slots:
    virtual void edit(bool show = true) override;

protected:
    static QHash<QString, RamPipeFile*> m_existingObjects;
    RamPipeFile(QString uuid);
    virtual QString folderPath() const override { return ""; };

    static QFrame *ui_editWidget;

private:
    void construct();
};

#endif // RAMPIPEFILE_H
