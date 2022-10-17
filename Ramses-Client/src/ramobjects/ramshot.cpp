#include "ramshot.h"

#include "ramasset.h"
#include "ramassetgroup.h"
#include "ramproject.h"
#include "ramsequence.h"
#include "shoteditwidget.h"

// STATIC //

QFrame *RamShot::ui_editWidget = nullptr;

QHash<QString, RamShot*> RamShot::m_existingObjects = QHash<QString, RamShot*>();

RamShot *RamShot::get(QString uuid)
{
    if (!checkUuid(uuid, Shot)) return nullptr;

    RamShot *s = m_existingObjects.value(uuid);
    if (s) return s;

    // Finally return a new instance
    return new RamShot(uuid);
}

RamShot *RamShot::c(RamObject *obj)
{
    //return qobject_cast<RamShot*>(obj);
    // For performance, reinterpret_cast, but be careful with the object passed!
    return reinterpret_cast<RamShot*>(obj);
}

// PUBLIC //

RamShot::RamShot(QString shortName, QString name, RamSequence *sequence):
    RamAbstractItem(shortName, name, Shot, sequence->project())
{
    Q_ASSERT_X(sequence, "RamAsset(shortname, name, assetgroup)", "Sequence can't be null!");
    construct();
    insertData("sequence", sequence->uuid() );
}

QColor RamShot::color() const
{
    RamSequence *s = sequence();
    if (s) return s->color();
    return QColor(150,150,150);
}

RamShot::RamShot(QString uuid):
    RamAbstractItem(uuid, Shot)
{
    construct();
    loadModel(m_assets, "assets");
}

RamSequence *RamShot::sequence() const
{
    return RamSequence::get( getData("sequence").toString() );
}

void RamShot::setSequence(RamObject *sequence)
{
    insertData("sequence", sequence->uuid() );
}

qreal RamShot::duration() const
{
    return getData("duration").toDouble(5);
}

void RamShot::setDuration(const qreal &duration)
{
    insertData("duration", duration);
}

RamObjectModel *RamShot::assets() const
{
    return m_assets;
}

RamAsset *RamShot::assetAt(int row) const
{
    return RamAsset::c( m_assets->get(row) );
}

QString RamShot::filterUuid() const
{
    return getData("sequence").toString();
}

QString RamShot::details() const
{
    RamProject *proj = project();
    if (!proj) return tr("Invalid Shot.\n\nMaybe the database needs to be repaired.");

    QString details = tr("Duration:") + " " +
                    QString::number(duration(), 'f', 2) +
                    " s | " +
                    QString::number(duration() * proj->framerate(), 'f', 2) +
                    " f";

    // List assigned assets
    QHash<QString,QStringList> assts;
    for (int i = 0; i < assets()->rowCount(); i++)
    {
        RamAsset *asset = RamAsset::c(assets()->get(i));
        QString agName = asset->assetGroup()->name();
        QStringList ag = assts.value( agName );
        ag << asset->shortName();
        assts[ agName ] = ag;
    }
    QHashIterator<QString,QStringList> i(assts);
    while(i.hasNext())
    {
        i.next();
        details = details + "\n" % i.key() + " ► " + i.value().join(", ");
    }

    return details;
}

QVariant RamShot::roleData(int role) const
{
    switch(role) {
    case Duration: return this->duration();
    }
    return RamObject::roleData(role);
}

// PUBLIC SLOTS //

void RamShot::edit(bool show)
{
    if (!ui_editWidget) ui_editWidget = createEditFrame(new ShotEditWidget());

    if (show) showEdit( ui_editWidget );
}

// PROTECTED //

QString RamShot::folderPath() const
{
    RamProject *proj = project();
    if (!proj) return "";
    return proj->path(RamObject::ShotsFolder) + "/" + proj->shortName() + "_S_" + shortName();
}

// PRIVATE //

void RamShot::construct()
{
    m_existingObjects[m_uuid] = this;
    m_icon = ":/icons/shot";
    m_editRole = ProjectAdmin;
    m_assets = createModel(RamObject::Asset, "assets");
}


























