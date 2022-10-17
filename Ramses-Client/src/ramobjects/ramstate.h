#ifndef RAMSTATE_H
#define RAMSTATE_H

#include "ramobject.h"

class RamState : public RamObject
{
    Q_OBJECT
public:

    // STATIC METHODS //

    static RamState *get(QString uuid);
    static RamState *c(RamObject *o);

    // METHODS //

    RamState(QString shortName, QString name);

    int completionRatio() const;
    void setCompletionRatio(int completionRatio);

    virtual QString details() const override;
    virtual QVariant roleData(int role) const override;

public slots:
    virtual void edit(bool show = true) override;

protected:
    static QHash<QString, RamState*> m_existingObjects;
    RamState(QString uuid);
    virtual QString folderPath() const override { return ""; };

    static QFrame *ui_editWidget;

private:
    void construct();

};

#endif // RAMSTATE_H
