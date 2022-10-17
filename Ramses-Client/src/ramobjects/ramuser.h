#ifndef RAMUSER_H
#define RAMUSER_H

#include <QStringBuilder>

#include "ramobject.h"
#include "ramobjectmodel.h"

class RamStep;
class RamScheduleEntry;

class RamUser : public RamObject
{
    Q_OBJECT
public:

    // STATIC METHODS //

    static RamUser *get(QString uuid);
    static RamUser *c(RamObject *o);

    // METHODS //

    RamUser(QString shortName, QString name);

    virtual void setShortName(const QString &shortName) override;
    virtual bool validateShortName(const QString &shortName) override;

    UserRole role() const;
    void setRole(const UserRole &role);
    void setRole(const QString role);

    RamObjectModel *schedule() const;
    bool isStepAssigned(RamStep *step) const;

    virtual QString iconName() const override;
    virtual QString details() const override;

public slots:
    virtual void edit(bool show = true) override;

protected:
    static QHash<QString, RamUser*> m_existingObjects;
    RamUser(QString uuid);
    virtual QString folderPath() const override;

    static QFrame *ui_editWidget;

private:
    void construct();

    RamObjectModel *m_schedule;
};

#endif // RAMUSER_H

