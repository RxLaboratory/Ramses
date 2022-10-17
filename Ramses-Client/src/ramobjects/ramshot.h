#ifndef RAMSHOT_H
#define RAMSHOT_H

#include "ramabstractitem.h"

class RamSequence;
class RamAsset;
class RamObjectList;

class RamShot : public RamAbstractItem
{
    Q_OBJECT
public:

    // STATIC METHODS //

    static RamShot *get(QString uuid);
    // Short for qobject_cast<RamShot*>
    static RamShot *c(RamObject *obj);

    // METHODS //

    RamShot(QString shortName, QString name, RamSequence *sequence);

    QColor color() const override;

    RamSequence *sequence() const;
    void setSequence(RamObject *sequence);

    qreal duration() const;
    void setDuration(const qreal &duration);

    RamObjectModel *assets() const;
    RamAsset *assetAt(int row) const;

    virtual QString filterUuid() const override;

    virtual QString details() const override;

    virtual QVariant roleData(int role) const override;

public slots:
    virtual void edit(bool show = true) override;

protected:
    static QHash<QString, RamShot*> m_existingObjects;
    virtual QString folderPath() const override;
    RamShot(QString uuid);

    static QFrame *ui_editWidget;

private:
    void construct();

    RamObjectModel *m_assets;
};

#endif // RAMSHOT_H
