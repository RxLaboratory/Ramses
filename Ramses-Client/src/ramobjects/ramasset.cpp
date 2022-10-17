#include "ramasset.h"

#include "asseteditwidget.h"
#include "ramproject.h"

// STATIC //

QFrame *RamAsset::ui_assetWidget = nullptr;

QHash<QString, RamAsset*> RamAsset::m_existingObjects = QHash<QString, RamAsset*>();

RamAsset *RamAsset::get(QString uuid)
{
    if (!checkUuid(uuid, Asset)) return nullptr;

    RamAsset *a = m_existingObjects.value(uuid);
    if (a) return a;

    // Finally return a new instance
    return new RamAsset(uuid);
}

RamAsset *RamAsset::c(RamObject *o)
{
    //return qobject_cast<RamAsset*>(o);
    // For performance, reinterpret_cast, but be careful with the object passed!
    return reinterpret_cast<RamAsset*>(o);
}

// PUBLIC //

RamAsset::RamAsset(QString shortName, QString name, RamAssetGroup *ag) :
    RamAbstractItem(shortName, name, Asset, ag->project())
{
    Q_ASSERT_X(ag, "RamAsset(shortname, name, assetgroup)", "AssetGroup can't be null!");
    construct();
    setAssetGroup(ag);
}

QColor RamAsset::color() const
{
    RamAssetGroup *ag = assetGroup();
    if (ag) return ag->color();
    return QColor(150,150,150);
}

RamAsset::RamAsset(QString uuid):
    RamAbstractItem(uuid, Asset)
{
    construct();
}

RamAssetGroup *RamAsset::assetGroup() const
{
    return RamAssetGroup::get( getData("assetGroup").toString() );
}

void RamAsset::setAssetGroup(RamAssetGroup *ag)
{
    if(!ag) return;
    insertData("assetGroup", ag->uuid());
}

QStringList RamAsset::tags() const
{
    QJsonArray arr = getData("tags").toArray();
    QStringList ts;
    for (int i = 0; i < arr.count(); i++)
    {
        ts << arr.at(i).toString();
    }
    return ts;
}

void RamAsset::setTags(QString tags)
{
    QStringList ts = tags.toLower().split(",");
    QJsonArray arr;
    for(int i = 0; i < ts.count(); i++)
    {
        arr.append( ts[i].trimmed().toLower() );
    }
    insertData("tags", arr);
}

void RamAsset::addTag(QString tag)
{
    QJsonArray arr = getData("tags").toArray();
    arr.append(tag.trimmed().toLower());
    insertData("tags", arr);
}

void RamAsset::removeTag(QString tag)
{
    QJsonArray arr = getData("tags").toArray();
    if (!arr.contains(tag)) return;
    for (int i = 0; i < arr.count(); i++)
    {
        if (arr.at(i).toString() == tag)
        {
            arr.removeAt(i);
            break;
        }
    }
    insertData("tags", arr);
}

bool RamAsset::hasTag(QString tag)
{
    QJsonArray arr = getData("tags").toArray();
    return arr.contains(tag);
}

QString RamAsset::filterUuid() const
{
    return getData("assetGroup").toString();
}

QString RamAsset::details() const
{
    RamAssetGroup *ag = assetGroup();
    if (ag)
        return assetGroup()->name() +
            "\n" +
            tags().join(", ");
    return tags().join(", ");
}

void RamAsset::edit(bool show)
{
    if (!ui_assetWidget) ui_assetWidget = createEditFrame(new AssetEditWidget());

    if (show) showEdit(ui_assetWidget);
}

// PROTECTED //

QString RamAsset::folderPath() const
{
    RamProject *proj = project();
    if (!proj) return "";
    RamAssetGroup *ag = assetGroup();
    if (ag) return  ag->path() + "/" + proj->shortName() + "_A_" + shortName();
    return proj->shortName() + "_A_" + shortName();
}

// PRIVATE //

void RamAsset::construct()
{
    m_existingObjects[m_uuid] = this;
    m_icon = ":/icons/asset";
    m_editRole = ProjectAdmin;
}

