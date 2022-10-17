#ifndef RAMSEQUENCE_H
#define RAMSEQUENCE_H

#include "ramobject.h"

#include "ramobjectsortfilterproxymodel.h"

#include "ramproject.h"

class RamSequence : public RamObject
{
    Q_OBJECT
public:

    // STATIC METHODS //

    static RamSequence *get(QString uuid);
    static RamSequence *c(RamObject *o);

    // METHODS //

    RamSequence(QString shortName, QString name, RamProject *project);

    int shotCount() const;
    double duration() const;

    RamProject *project() const;

    virtual QString details() const override;

    virtual QVariant roleData(int role) const override;

public slots:
    virtual void edit(bool show = true) override;

protected:
    static QHash<QString, RamSequence*> m_existingObjects;
    RamSequence(QString uuid);
    virtual QString folderPath() const override { return ""; };

    static QFrame *ui_editWidget;

private:
    void construct();
    void setProject(RamProject *project);

    RamObjectSortFilterProxyModel *m_shots;
};

#endif // RAMSEQUENCE_H
