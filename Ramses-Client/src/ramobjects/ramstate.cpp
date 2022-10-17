#include "ramstate.h"

#include "stateeditwidget.h"

// STATIC //

QFrame *RamState::ui_editWidget = nullptr;

QHash<QString, RamState*> RamState::m_existingObjects = QHash<QString, RamState*>();

RamState *RamState::get(QString uuid )
{
    if (!checkUuid(uuid, State)) return nullptr;

    RamState *s = m_existingObjects.value(uuid);
    if (s) return s;

    // Finally return a new instance
    return new RamState(uuid);
}

RamState *RamState::c(RamObject *o)
{
    //return qobject_cast<RamState*>(o);
    // For performance, reinterpret_cast, but be careful with the object passed!
    return reinterpret_cast<RamState*>(o);
}

// PUBLIC //

RamState::RamState(QString shortName, QString name) :
    RamObject(shortName, name, State)
{
    construct();
}

RamState::RamState(QString uuid):
    RamObject(uuid, State)
{
    construct();
}

int RamState::completionRatio() const
{
    return getData("completionRatio").toInt(50);
}

void RamState::setCompletionRatio(int completionRatio)
{
    insertData("completionRatio", completionRatio);
}

QString RamState::details() const
{
    return "Completion ratio: " + QString::number(completionRatio()) + "%";
}

QVariant RamState::roleData(int role) const
{
    switch(role)
    {
    case RamAbstractObject::Completion: return this->completionRatio();
    }
    return RamObject::roleData(role);
}

// PUBLIC SLOTS //

void RamState::edit(bool show)
{
    if (!ui_editWidget) ui_editWidget = createEditFrame(new StateEditWidget());

    if (show) showEdit( ui_editWidget );
}

// PRIVATE //

void RamState::construct()
{
    m_existingObjects[m_uuid] = this;
    m_icon = ":/icons/state-l";
    m_editRole = Admin;
}
