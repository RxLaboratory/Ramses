#include "ramfiletype.h"

#include "filetypeeditwidget.h"

// STATIC //

QFrame *RamFileType::ui_editWidget = nullptr;

QHash<QString, RamFileType*> RamFileType::m_existingObjects = QHash<QString, RamFileType*>();

RamFileType *RamFileType::get(QString uuid)
{
    if (!checkUuid(uuid, FileType)) return nullptr;

    RamFileType *f = m_existingObjects.value(uuid);
    if (f) return f;

    // Finally return a new instance
    return new RamFileType(uuid);
}

RamFileType *RamFileType::c(RamObject *o)
{
    // return qobject_cast<RamFileType*>(o);
    // For performance, reinterpret_cast, but be careful with the object passed!
    return reinterpret_cast<RamFileType*>(o);
}

RamFileType::RamFileType(QString shortName, QString name):
    RamObject(shortName, name, FileType)
{
    construct();
}

RamFileType::RamFileType(QString uuid):
    RamObject(uuid, FileType)
{
    construct();
}

void RamFileType::setExtensions(QString extensions)
{
    QStringList exts = extensions.split(",");
    setExtensions(exts);
}

void RamFileType::setExtensions(QStringList extensions)
{
    QJsonArray arr;
    for(int i = 0; i < extensions.count(); i++)
    {
        QString ext = extensions[i];
        if (ext.startsWith(".")) ext = ext.remove(0, 1);
        ext = ext.trimmed().toLower();
        if (ext != "") arr.append( ext );
    }
    insertData("extensions", arr);
}

QStringList RamFileType::extensions() const
{
    QJsonArray arr = getData("extensions").toArray();
    QStringList exts;
    for (int i = 0; i < arr.count(); i++)
    {
        exts << arr.at(i).toString();
    }
    QString sn = this->shortName();
    if (sn.startsWith(".")) sn = sn.remove(0, 1);
    if (!exts.contains(sn)) exts << sn;
    return exts;
}

bool RamFileType::previewable() const
{
    return getData("previewable").toBool(false);
}

void RamFileType::setPreviewable(bool previewable)
{
    insertData("previewable", previewable);
}

bool RamFileType::check(QString filePath) const
{
    QFileInfo info(filePath);
    if (!info.isFile()) return false;
    QString ext = info.completeSuffix().toLower();

    if (shortName() == ext) return true;

    return extensions().contains(ext);
}

QString RamFileType::details() const
{
    return "Extensions: " + extensions().join(", ");
}

QVariant RamFileType::roleData(int role) const
{
    switch(role)
    {
    case Qt::DisplayRole: return "." + this->shortName() + " | " + this->name();
    }
    return RamAbstractObject::roleData(role);
}

void RamFileType::edit(bool show)
{
    if (!ui_editWidget) ui_editWidget = createEditFrame(new FileTypeEditWidget());

    if (show) showEdit(ui_editWidget);
}

// PRIVATE //

void RamFileType::construct()
{
    m_existingObjects[m_uuid] = this;
    m_icon = ":/icons/file";
    m_editRole = Admin;
}





