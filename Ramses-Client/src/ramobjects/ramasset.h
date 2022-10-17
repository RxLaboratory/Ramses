#ifndef RAMASSET_H
#define RAMASSET_H

#include "ramabstractitem.h"
#include "ramassetgroup.h"

class RamAsset : public RamAbstractItem
{
    Q_OBJECT
public:

    // STATIC //

    static RamAsset *get(QString uuid);
    static RamAsset *c(RamObject *o);

    // OTHER //

    RamAsset(QString shortName, QString name, RamAssetGroup *ag);

    QColor color() const override;

    RamAssetGroup *assetGroup() const;
    void setAssetGroup(RamAssetGroup *ag);

    QStringList tags() const;
    void setTags(QString tags);
    void addTag(QString tag);
    void removeTag(QString tag);
    bool hasTag(QString tag);

    QString filterUuid() const override;

    virtual QString details() const override;

public slots:
    virtual void edit(bool show = true) override;

protected:
    static QHash<QString, RamAsset*> m_existingObjects;
    RamAsset(QString uuid);
    virtual QString folderPath() const override;

    static QFrame *ui_assetWidget;

private:
    void construct();
};

#endif // RAMASSET_H
