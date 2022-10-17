#include "ramassetgroup.h"

#include "assetgroupeditwidget.h"

// STATIC //

QFrame *RamAssetGroup::ui_assetGroupWidget = nullptr;

QHash<QString, RamAssetGroup*> RamAssetGroup::m_existingObjects = QHash<QString, RamAssetGroup*>();

RamAssetGroup *RamAssetGroup::get(QString uuid)
{
    if (!checkUuid(uuid, AssetGroup)) return nullptr;

    RamAssetGroup *a = m_existingObjects.value(uuid);
    if (a) return a;

    // Finally return a new instance
    return new RamAssetGroup(uuid);
}

RamAssetGroup *RamAssetGroup::c(RamObject *o)
{
    //return qobject_cast<RamAssetGroup*>(o);
    // For performance, reinterpret_cast, but be careful with the object passed!
    return reinterpret_cast<RamAssetGroup*>(o);
}

RamAssetGroup *RamAssetGroup::createFromTemplate(RamTemplateAssetGroup *tempAG, RamProject *project)
{
    // Create
    RamAssetGroup *assetGroup = new RamAssetGroup(tempAG->shortName(), tempAG->name(), project);
    assetGroup->setColor(tempAG->color());
    project->assetGroups()->appendObject(assetGroup->uuid());
    return assetGroup;
}

// PUBLIC //

RamAssetGroup::RamAssetGroup(QString shortName, QString name, RamProject *project):
    RamTemplateAssetGroup(shortName, name, AssetGroup)
{
    construct();
    setProject(project);
    insertData("project", project->uuid());
}

RamAssetGroup::RamAssetGroup(QString uuid):
    RamTemplateAssetGroup(uuid, AssetGroup)
{
    construct();

    QJsonObject d = data();

    QString projUuid = d.value("project").toString();
    setProject( RamProject::get(projUuid) );
}

int RamAssetGroup::assetCount() const
{
    return m_assets->rowCount();
}

RamProject *RamAssetGroup::project() const
{
    QString projUuid = getData("project").toString();
    return RamProject::get(projUuid);
}

QString RamAssetGroup::details() const
{
    return "Contains " + QString::number(assetCount()) + " assets";
}

void RamAssetGroup::edit(bool show)
{
    if (!ui_assetGroupWidget) ui_assetGroupWidget = createEditFrame(new AssetGroupEditWidget());

    if (show) showEdit(ui_assetGroupWidget);
}

// PROTECTED //

QString RamAssetGroup::folderPath() const
{
    RamProject *proj = project();
    if (!proj) return "";
    return proj->path(RamObject::AssetsFolder) + "/" + name();
}

// PRIVATE //

void RamAssetGroup::construct()
{
    m_existingObjects[m_uuid] = this;
    m_objectType = AssetGroup;
    m_assets = new RamObjectSortFilterProxyModel(this);
    m_assets->setSingleColumn(true);
    m_icon = ":/icons/asset-group";
    m_editRole = ProjectAdmin;
}

void RamAssetGroup::setProject(RamProject *project)
{
    m_assets->setSourceModel( project->assets() );
    m_assets->setFilterUuid( m_uuid );
    this->setParent( project );
}

