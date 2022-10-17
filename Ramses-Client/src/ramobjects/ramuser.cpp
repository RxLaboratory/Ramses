#include "ramuser.h"

#include "ramabstractitem.h"
#include "ramses.h"
#include "ramscheduleentry.h"
#include "usereditwidget.h"
#include "ramdatainterface/dbinterface.h"
#include "ramasset.h"
#include "ramshot.h"
#include "duqf-app/app-config.h"

// STATIC //

QFrame *RamUser::ui_editWidget = nullptr;

QHash<QString, RamUser*> RamUser::m_existingObjects = QHash<QString, RamUser*>();

RamUser *RamUser::get(QString uuid )
{
    if (!checkUuid(uuid, User)) return nullptr;

    RamUser *u = m_existingObjects.value(uuid);
    if (u) return u;

    // Finally return a new instance
    return new RamUser(uuid);
}

RamUser *RamUser::c(RamObject *o)
{
    //return qobject_cast<RamUser*>(o);
    // For performance, reinterpret_cast, but be careful with the object passed!
    return reinterpret_cast<RamUser*>(o);
}

// PUBLIC //

RamUser::RamUser(QString shortName, QString name) :
    RamObject(shortName, name, User, nullptr, shortName == "Ramses", ENCRYPT_USER_DATA)
{
    construct();
    if (shortName.toLower() != "new" && shortName != "Ramses")
        DBInterface::instance()->setUsername(m_uuid, shortName);
    if (shortName == "Ramses") m_uuid = "none";
}

RamUser::RamUser(QString uuid):
    RamObject(uuid, User, nullptr, ENCRYPT_USER_DATA)
{
    construct();

    loadModel( m_schedule, "schedule" );
}

void RamUser::setShortName(const QString &shortName)
{
    RamAbstractObject::setShortName(shortName);
    // Set username
    DBInterface::instance()->setUsername(m_uuid, shortName);
}

bool RamUser::validateShortName(const QString &shortName)
{
    if (!RamAbstractObject::validateShortName(shortName)) return false;
    return DBInterface::instance()->isUserNameAavailable(shortName);
}

RamUser::UserRole RamUser::role() const
{
    QString roleStr = getData("role").toString("standard");
    if (roleStr == "admin") return Admin;
    if (roleStr == "project") return ProjectAdmin;
    if (roleStr == "lead") return Lead;
    return Standard;
}

void RamUser::setRole(const UserRole &role)
{
    switch(role)
    {
    case Admin:
        insertData("role", "admin");
        m_icon = ":/icons/admin";
        break;
    case ProjectAdmin:
        insertData("role", "project");
        m_icon = ":/icons/project-admin";
        break;
    case Lead:
        insertData("role", "lead");
        m_icon = ":/icons/lead";
        break;
    case Standard:
        insertData("role", "standard");
        m_icon = ":/icons/user";
        break;
    }
}

void RamUser::setRole(const QString role)
{
    insertData("role", role);
}

RamObjectModel *RamUser::schedule() const
{
    return m_schedule;
}

bool RamUser::isStepAssigned(RamStep *step) const
{
    // Check in schedule
    for(int i = 0; i < m_schedule->rowCount(); i++)
    {
        RamScheduleEntry *entry = RamScheduleEntry::c( m_schedule->get(i) );
        if (!entry) continue;

        if (step->is(entry->step())) return true;
    }

    if (step->type() != RamStep::ShotProduction && step->type() != RamStep::AssetProduction) return false;

    // Check in status
    RamObjectModel *items;
    RamStep::Type type = step->type();
    if (type == RamStep::ShotProduction) items = step->project()->shots();
    else items = step->project()->assets();

    for (int i =0; i < items->rowCount(); i++)
    {
        RamAbstractItem *item;
        if (type == RamStep::ShotProduction) item = RamShot::c( items->get(i) );
        else item = RamAsset::c( items->get(i) );
        RamStatus *status = item->status(step);
        if (!status) continue;
        if (this->is(status->assignedUser())) return true;
    }

    return false;
}

QString RamUser::iconName() const
{
    switch(role())
    {
    case Admin:
        return ":/icons/admin";
    case ProjectAdmin:
        return ":/icons/project-admin";
    case Lead:
        return ":/icons/lead";
    case Standard:
        return ":/icons/user";
    }
    return ":/icons/user";
}

QString RamUser::details() const
{
    switch(role())
    {
    case Admin:
        return "Administrator";
    case ProjectAdmin:
        return "Project Admin";
    case Lead:
        return "Lead";
    case Standard:
        return "Standard User";
    }
    return "User";
}

// PUBLIC SLOTS //

void RamUser::edit(bool show)
{
    if (!ui_editWidget) ui_editWidget = createEditFrame(new UserEditWidget(this));

    if (show) showEdit( ui_editWidget  );
}

// PROTECTED //

QString RamUser::folderPath() const
{
    return Ramses::instance()->path(RamObject::UsersFolder) + "/" + shortName();
}

// PRIVATE //

void RamUser::construct()
{
    m_existingObjects[m_uuid] = this;
    m_icon = ":/icons/user";
    m_editRole = Admin;
    m_schedule = createModel(RamObject::ScheduleEntry, "schedule");
}
