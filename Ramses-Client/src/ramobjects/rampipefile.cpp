#include "rampipefile.h"

#include "pipefileeditwidget.h"

QFrame *RamPipeFile::ui_editWidget = nullptr;

QHash<QString, RamPipeFile*> RamPipeFile::m_existingObjects = QHash<QString, RamPipeFile*>();

RamPipeFile *RamPipeFile::get(QString uuid)
{
    if (!checkUuid(uuid, PipeFile)) return nullptr;

    RamPipeFile *p = m_existingObjects.value(uuid);
    if (p) return p;

    // Finally return a new instance
    return new RamPipeFile(uuid);
}

RamPipeFile *RamPipeFile::c(RamObject *o)
{
    // return qobject_cast<RamPipeFile*>(o);
    // For performance, reinterpret_cast, but be careful with the object passed!
    return reinterpret_cast<RamPipeFile*>(o);
}

// PUBLIC //

RamPipeFile::RamPipeFile(QString shortName, RamProject *project) :
    RamObject(shortName, shortName, PipeFile, project)
{
    construct();

    insertData("project", project->uuid());
}

RamPipeFile::RamPipeFile(QString uuid):
    RamObject(uuid, PipeFile)
{
    construct();

    QJsonObject d = data();
}

QString RamPipeFile::name() const
{
    QString name = shortName();
    RamFileType *ft = fileType();
    if (!ft) return name;
    return name + "." + ft->shortName();
}

RamFileType *RamPipeFile::fileType() const
{
    return RamFileType::get( getData("fileType").toString("none") );
}

void RamPipeFile::setFileType(RamFileType *newFileType)
{
    QJsonObject d = data();

    // set file type
    d.insert("fileType", newFileType->uuid());

    // Update the custom settings
    QString format;
    QStringList extensions = newFileType->extensions();
    if (extensions.count() > 0)
        format = extensions.first();
    else
        format = "*";

    QStringList settings = d.value("customSettings").toString().split("\n");
    bool found = false;

    for(int i = 0; i < settings.count(); i++)
    {
        QString s = settings[i].trimmed();
        if (s.startsWith("format:"))
        {
            found = true;
            QStringList setting = settings[i].split(":");
            if (setting.count() > 1) setting[1] = format;
            else setting << format;
            settings[i] = setting.join(": ");
            break;
        }
    }

    if (!found)
    {
        settings << "format: " + format;
    }

    d.insert("customSettings", settings.join("\n"));

    setData(d);
}

const RamProject *RamPipeFile::project() const
{
    return RamProject::get( getData("project").toString() );
}

QString RamPipeFile::customSettings() const
{
    return getData("customSettings").toString();
}

void RamPipeFile::setCustomSettings(const QString &newCustomSettings)
{
    insertData("customSettings", newCustomSettings);
}

QString RamPipeFile::details() const
{
    RamFileType *ft = fileType();
    if (ft) return "File type: " + ft->name();
    return RamObject::details();
}

void RamPipeFile::edit(bool show)
{
    if (!ui_editWidget) ui_editWidget = createEditFrame(new PipeFileEditWidget());

    if (show) showEdit(ui_editWidget);
}

// PRIVATE //

void RamPipeFile::construct()
{
    m_existingObjects[m_uuid] = this;
    m_icon = ":/icons/connection";
    m_editRole = ProjectAdmin;
}
