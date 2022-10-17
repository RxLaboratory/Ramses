#include "ramapplication.h"

#include "applicationeditwidget.h"

// STATIC //

QFrame *RamApplication::ui_applicationWidget = nullptr;

QHash<QString, RamApplication*> RamApplication::m_existingObjects = QHash<QString, RamApplication*>();

RamApplication *RamApplication::get(QString uuid)
{
    if (!checkUuid(uuid, Application)) return nullptr;

    RamApplication *a = m_existingObjects.value(uuid);
    if (a) return a;

    // Finally return a new instance
    return new RamApplication(uuid);
}

RamApplication *RamApplication::c(RamObject *o)
{
    //return qobject_cast<RamApplication*>(o);
    // For performance, reinterpret_cast, but be careful with the object passed!
    return reinterpret_cast<RamApplication*>(o);
}

// PUBLIC //

RamApplication::RamApplication(QString shortName, QString name):
    RamObject(shortName, name, ObjectType::Application)
{
    construct();
}

QString RamApplication::executableFilePath() const
{
    return getData("executableFilePath").toString("");
}

void RamApplication::setExecutableFilePath(const QString &executableFilePath)
{
    insertData("executableFilePath", executableFilePath);
}

RamObjectModel *RamApplication::nativeFileTypes() const
{
    return m_nativeFileTypes;
}

RamObjectModel *RamApplication::importFileTypes() const
{
    return m_importFileTypes;
}

RamObjectModel *RamApplication::exportFileTypes() const
{
    return m_exportFileTypes;
}

bool RamApplication::canExportFileType(RamFileType *ft) const
{
    if (m_nativeFileTypes->contains(ft->uuid())) return true;
    if (m_exportFileTypes->contains(ft->uuid())) return true;
    return false;
}

bool RamApplication::canExportFileType(QString extension) const
{
    for (int i = 0; i < m_nativeFileTypes->rowCount(); i++)
    {
        RamFileType *ft = RamFileType::c( m_nativeFileTypes->get(i) );
        if (ft->extensions().contains(extension,Qt::CaseInsensitive)) return true;
    }
    for (int i = 0; i < m_exportFileTypes->rowCount(); i++)
    {
        RamFileType *ft = RamFileType::c( m_exportFileTypes->get(i) );
        if (ft->extensions().contains(extension, Qt::CaseInsensitive)) return true;
    }
    return false;
}

bool RamApplication::canImportFileType(RamFileType *ft) const
{
    if (m_nativeFileTypes->contains(ft->uuid())) return true;
    if (m_importFileTypes->contains(ft->uuid())) return true;
    return false;
}

bool RamApplication::canImportFileType(QString extension) const
{
    for (int i = 0; i < m_nativeFileTypes->rowCount(); i++)
    {
        RamFileType *ft = RamFileType::c( m_nativeFileTypes->get(i) );
        if (ft->extensions().contains(extension,Qt::CaseInsensitive)) return true;
    }
    for (int i = 0; i < m_importFileTypes->rowCount(); i++)
    {
        RamFileType *ft = RamFileType::c( m_importFileTypes->get(i) );
        if (ft->extensions().contains(extension, Qt::CaseInsensitive)) return true;
    }
    return false;
}

bool RamApplication::canOpen(QString filePath) const
{
    for(int i =0; i < m_nativeFileTypes->rowCount(); i++)
    {
        RamFileType *ft = RamFileType::c( m_nativeFileTypes->get(i) );
        if (ft->check( filePath )) return true;
    }
    return false;
}

bool RamApplication::open(QString filePath) const
{
    if(!canOpen(filePath)) return false;

    QString execFilePath = executableFilePath();

    if (execFilePath == "") return false;

    return QProcess::startDetached(execFilePath, QStringList(filePath));
}

// PUBLIC SLOTS //

void RamApplication::unassignFileType(RamFileType *ft)
{
    if (!ft) return;
    m_nativeFileTypes->removeObjects(QStringList(ft->uuid()));
    m_importFileTypes->removeObjects(QStringList(ft->uuid()));
    m_exportFileTypes->removeObjects(QStringList(ft->uuid()));
}

void RamApplication::edit(bool show)
{
    if (!ui_applicationWidget) ui_applicationWidget = createEditFrame(new ApplicationEditWidget());
    if (show) showEdit(ui_applicationWidget);
}

// PROTECTED //

RamApplication::RamApplication(QString uuid):
    RamObject(uuid, ObjectType::Application)
{
    construct();

    // Load lists
    QJsonObject d = data();
    loadModel(m_nativeFileTypes, "nativeFileTypes", d);
    loadModel(m_importFileTypes, "importFileTypes", d);
    loadModel(m_exportFileTypes, "exportFileTypes", d);
}

QString RamApplication::folderPath() const
{
    QFileInfo fpath( executableFilePath() );
    if (!fpath.exists()) return "";
    return fpath.absolutePath();
}

// PRIVATE //

void RamApplication::construct()
{
    m_existingObjects[m_uuid] = this;
    m_icon = ":/icons/application";
    m_editRole = Admin;

    m_nativeFileTypes = createModel(RamObject::FileType, "nativeFileTypes");
    m_importFileTypes = createModel(RamObject::FileType, "importFileTypes");
    m_exportFileTypes = createModel(RamObject::FileType, "exportFileTypes");
}
