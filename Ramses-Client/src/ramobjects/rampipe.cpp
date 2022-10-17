#include "rampipe.h"

#include "pipeeditwidget.h"
#include "ramstep.h"

QFrame *RamPipe::ui_editWidget = nullptr;

QHash<QString, RamPipe*> RamPipe::m_existingObjects = QHash<QString, RamPipe*>();

RamPipe *RamPipe::get(QString uuid)
{
    if (!checkUuid(uuid, Pipe)) return nullptr;

    RamPipe *p = m_existingObjects.value(uuid);
    if (p) return p;

    // Finally return a new instance
    return new RamPipe(uuid);
}

RamPipe *RamPipe::c(RamObject *o)
{
    //return qobject_cast<RamPipe*>(o);
    // For performance, reinterpret_cast, but be careful with the object passed!
    return reinterpret_cast<RamPipe*>(o);
}

// PUBLIC //

RamPipe::RamPipe(RamStep *output, RamStep *input):
    RamObject("PIPE","Pipe", Pipe)
{
    construct();

    QJsonObject d = data();

    d.insert("outputStep", output->uuid());
    d.insert("inputStep", input->uuid());
    d.insert("shortName", output->shortName() + "-" + input->shortName());

    setData(d);

    this->setParent(this->project());
    connectEvents();
}

RamPipe::RamPipe(QString uuid):
    RamObject(uuid, Pipe)
{
    construct();
    loadModel(m_pipeFiles, "pipeFiles");
    connectEvents();
}

void RamPipe::pipeFilesDataChanged()
{
    emit dataChanged(this);
}

RamStep *RamPipe::outputStep() const
{
    return RamStep::get( getData("outputStep").toString("none") );
}

void RamPipe::setOutputStep(RamObject *outputStep)
{
    QJsonObject d = data();
    d.insert("outputStep", outputStep->uuid());
    d.insert("shortName", outputStep->shortName() + "-" + inputStep()->shortName());
    setData(d);
}

RamStep *RamPipe::inputStep() const
{
    return RamStep::get( getData("inputStep").toString("none") );
}

void RamPipe::setInputStep(RamObject *inputStep)
{
    QJsonObject d = data();
    d.insert("inputStep", inputStep->uuid());
    d.insert("shortName", outputStep()->shortName() + "-" + inputStep->shortName());
    setData(d);
}

QString RamPipe::name() const
{
    if (!m_pipeFiles) return "";
    int numFiles = m_pipeFiles->rowCount();
    if (numFiles == 0) return "";

    QStringList nameList;
    for (int i = 0; i < m_pipeFiles->rowCount(); i++)
    {
        nameList << m_pipeFiles->get(i)->name();
    }
    return nameList.join("\n");
}

RamProject *RamPipe::project() const
{
    return outputStep()->project();
}

RamObjectModel *RamPipe::pipeFiles() const
{
    return m_pipeFiles;
}

// PUBLIC SLOTS //

void RamPipe::edit(bool show)
{
    if (!ui_editWidget) ui_editWidget = createEditFrame(new PipeEditWidget());

    if (show) showEdit(ui_editWidget);
}

// PRIVATE //

void RamPipe::construct()
{
    m_existingObjects[m_uuid] = this;
    m_icon = ":/icons/connection";
    m_editRole = ProjectAdmin;

    m_pipeFiles = createModel(RamObject::PipeFile, "pipeFiles");

    // When data has changed, we change the name/shortname
    connect(m_pipeFiles, &RamObjectModel::dataChanged, this, &RamPipe::pipeFilesDataChanged);
}

void RamPipe::connectEvents()
{
    connect( inputStep(), &RamStep::removed, this, &RamObject::remove);
    connect( outputStep(), &RamStep::removed, this, &RamObject::remove);
}
