#ifndef RAMTEMPLATEASSETGROUP_H
#define RAMTEMPLATEASSETGROUP_H

#include "ramobject.h"

class RamTemplateAssetGroup : public RamObject
{
    Q_OBJECT
public:

    //STATIC //

    static RamTemplateAssetGroup *get(QString uuid);
    static RamTemplateAssetGroup *c(RamObject *o);

    // OTHER //

    explicit RamTemplateAssetGroup(QString shortName, QString name, ObjectType type = TemplateAssetGroup);

public slots:
    virtual void edit(bool show = true) override;

protected:
    static QHash<QString, RamTemplateAssetGroup*> m_existingObjects;
    RamTemplateAssetGroup(QString uuid, ObjectType type = TemplateAssetGroup);
    virtual QString folderPath() const override { return ""; };

    static QFrame *ui_editWidget;

private:
    void construct();
};

#endif // RAMTEMPLATEASSETGROUP_H
