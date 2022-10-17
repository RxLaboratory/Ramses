#include "ramtemplateassetgroup.h"

#include "templateassetgroupeditwidget.h"

// STATIC //

QFrame *RamTemplateAssetGroup::ui_editWidget = nullptr;

QHash<QString, RamTemplateAssetGroup*> RamTemplateAssetGroup::m_existingObjects = QHash<QString, RamTemplateAssetGroup*>();

RamTemplateAssetGroup *RamTemplateAssetGroup::get(QString uuid)
{
    if (!checkUuid(uuid, TemplateAssetGroup)) return nullptr;

    RamTemplateAssetGroup *t = m_existingObjects.value(uuid);
    if (t) return t;

    // Finally return a new instance
    return new RamTemplateAssetGroup(uuid);
}

RamTemplateAssetGroup *RamTemplateAssetGroup::c(RamObject *o)
{
    //return qobject_cast<RamTemplateAssetGroup*>(o);
    // For performance, reinterpret_cast, but be careful with the object passed!
    return reinterpret_cast<RamTemplateAssetGroup*>(o);
}

// PUBLIC //

RamTemplateAssetGroup::RamTemplateAssetGroup(QString shortName, QString name, ObjectType type) :
    RamObject(shortName, name, type)
{
    construct();
}

RamTemplateAssetGroup::RamTemplateAssetGroup(QString uuid, ObjectType type):
    RamObject(uuid, type)
{
    construct();
}

// PUBLIC SLOTS //

void RamTemplateAssetGroup::edit(bool show)
{
    if (!ui_editWidget) ui_editWidget = createEditFrame(new TemplateAssetGroupEditWidget());

    if (show) showEdit( ui_editWidget );
}

// PRIVATE //

void RamTemplateAssetGroup::construct()
{
    m_existingObjects[m_uuid] = this;
    m_icon = ":/icons/asset-group";
    m_editRole = Admin;
}
