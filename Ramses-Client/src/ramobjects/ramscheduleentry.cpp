#include "ramscheduleentry.h"

// STATIC //

QHash<QString, RamScheduleEntry*> RamScheduleEntry::m_existingObjects = QHash<QString, RamScheduleEntry*>();

RamScheduleEntry *RamScheduleEntry::get(QString uuid)
{
    if (!checkUuid(uuid, ScheduleEntry)) return nullptr;

    RamScheduleEntry *s = m_existingObjects.value(uuid);
    if (s) return s;

    // Finally return a new instance
    return new RamScheduleEntry(uuid);
}

RamScheduleEntry *RamScheduleEntry::c(RamObject *o)
{
    //return qobject_cast<RamScheduleEntry*>(o);
    // For performance, reinterpret_cast, but be careful with the object passed!
    return reinterpret_cast<RamScheduleEntry*>(o);
}

// PUBLIC //

RamScheduleEntry::RamScheduleEntry(RamUser *user, QDateTime date):
       RamObject(user->shortName(), user->name() + " | " + date.toString("yyyy-MM-dd ap"), ObjectType::ScheduleEntry, user)
{
    construct();

    m_user = user;
    m_date = date;

    QJsonObject d = data();
    d.insert("user", user->uuid());
    d.insert("step", "none");
    d.insert("date", date.toString("yyyy-MM-dd hh:mm:ss"));
    setData(d);

    connectEvents();
}

RamScheduleEntry::RamScheduleEntry(QString uuid):
    RamObject(uuid, ObjectType::ScheduleEntry)
{
    construct();

    QJsonObject d = data();
    m_user = RamUser::get( d.value("user").toString() );
    m_date = QDateTime::fromString( d.value("date").toString(), "yyyy-MM-dd hh:mm:ss");

    this->setParent(m_user);

    connectEvents();
}

RamUser *RamScheduleEntry::user() const
{
    return m_user;
}

RamStep *RamScheduleEntry::step() const
{
    return RamStep::get( getData("step").toString() );
}

const QDateTime &RamScheduleEntry::date() const
{
    return m_date;
}

void RamScheduleEntry::setStep(RamStep *newStep)
{
    RamStep *currentStep = step();
    if (currentStep)
    {
        disconnect(currentStep, nullptr, this, nullptr);
        disconnect(this, nullptr, currentStep, nullptr);
        currentStep->countAssignedDays();
    }

    if (newStep)
    {
        insertData("step", newStep->uuid());
        connect(this, SIGNAL(removed(RamObject*)), newStep, SLOT(countAssignedDays()));
        connect(newStep, SIGNAL(removed(RamObject*)), this, SLOT(stepRemoved()));
        newStep->countAssignedDays();
    }
    else insertData("step", "none");
}

QString RamScheduleEntry::iconName() const
{
    RamStep *s = this->step();
    if (!s) return "";
    switch( s->type() )
    {
    case RamStep::PreProduction: return ":/icons/project";
    case RamStep::ShotProduction: return ":/icons/shot";
    case RamStep::AssetProduction: return ":/icons/asset";
    case RamStep::PostProduction: return ":/icons/film";
    case RamStep::All: return "";
    }
    return "";
}

QVariant RamScheduleEntry::roleData(int role) const
{
    switch(role)
    {
    case Qt::DisplayRole: {
        RamStep *s = this->step();
        if (s) return this->step()->shortName();
        else return "";
    }
    case Qt::ToolTipRole: {

        RamStep *s = this->step();
        QString stepName = "";
        if (s) stepName = s->name();

        RamUser *u = this->user();
        QString userName = "";
        if (u) userName = u->name();

        QSettings settings;
        QString dateFormat = settings.value("appearance/dateFormat", "yyyy-MM-dd").toString();
        return this->date().toString(dateFormat) +
                "\n" + stepName +
                "\n" + userName;
    }
    case Qt::StatusTipRole: {

        RamStep *s = this->step();
        QString stepName = "";
        if (s) stepName = s->shortName();

        RamUser *u = this->user();
        QString userName = "";
        if (u) userName = u->shortName();

        QSettings settings;
        QString dateFormat = settings.value("appearance/dateFormat", "yyyy-MM-dd").toString();
        return this->date().toString(dateFormat) +
                " | " + stepName +
                " | " + userName;
    }
    case Qt::BackgroundRole: {
        RamStep *s = this->step();
        if (s) return QBrush(s->color());
        else return QBrush();
    }
    case Qt::EditRole: {
        RamStep *s = this->step();
        if (s) return s->uuid();
        else return "";
    }
    case SizeHint: return QSize(75,10);
    }
    return RamObject::roleData(role);
}

void RamScheduleEntry::stepRemoved()
{
    setStep(nullptr);
}

void RamScheduleEntry::construct()
{
    m_existingObjects[m_uuid] = this;
    m_icon = ":/icons/calendar";
    m_editRole = Lead;
}

void RamScheduleEntry::connectEvents()
{
    connect(m_user, &RamUser::removed, this, &RamScheduleEntry::remove);
}
