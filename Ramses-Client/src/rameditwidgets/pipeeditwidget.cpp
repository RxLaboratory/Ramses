#include "pipeeditwidget.h"

#include "rampipefile.h"
#include "ramstep.h"

PipeEditWidget::PipeEditWidget(QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();
}

PipeEditWidget::PipeEditWidget(RamPipe *pipe, QWidget *parent) : ObjectEditWidget( parent)
{
    setupUi();
    connectEvents();
    setObject(pipe);
}

RamPipe *PipeEditWidget::pipe() const
{
    return m_pipe;
}

void PipeEditWidget::reInit(RamObject *o)
{
    m_pipe = qobject_cast<RamPipe*>(o);
    if (m_pipe)
    {
        // Load steps
        RamProject *project = m_pipe->project();
        if (!project) return;

        // find output and input steps
        QSignalBlocker b1(ui_toBox);
        QSignalBlocker b2(ui_fromBox);
        ui_fromBox->setObjectModel( project->steps() );
        ui_toBox->setObjectModel( project->steps() );

        ui_fromBox->setObject( m_pipe->outputStep() );
        ui_toBox->setObject( m_pipe->inputStep() );

        // Load file types
        ui_pipeFileList->setAssignList( project->pipeFiles() );
        ui_pipeFileList->setObjectModel( m_pipe->pipeFiles() );
    }
    else
    {
        // Clear boxes
        ui_fromBox->setObjectModel(nullptr);
        ui_toBox->setObjectModel(nullptr);
        ui_pipeFileList->setObjectModel(nullptr);
    }
}

void PipeEditWidget::appChanged()
{
    // Load file types
}

void PipeEditWidget::createPipeFile()
{
    if(!m_pipe) return;
    RamProject *project = m_pipe->project();
    if (!project) return;
    RamPipeFile *pipeFile = new RamPipeFile(
                "NEW",
                project);
    project->pipeFiles()->appendObject(pipeFile->uuid());
    m_pipe->pipeFiles()->appendObject(pipeFile->uuid());
    pipeFile->edit();
}

void PipeEditWidget::setInputStep(RamObject *step)
{
    if (!m_pipe) return;
    if (!step) return;
    m_pipe->setInputStep( step );
}

void PipeEditWidget::setOutputStep(RamObject *step)
{
    if (!m_pipe) return;
    if (!step) return;
    m_pipe->setOutputStep( step );
}

void PipeEditWidget::setupUi()
{
    this->hideName();

    ui_commentEdit->hide();
    ui_commentLabel->hide();

    QLabel *fromLabel = new QLabel("From", this);
    ui_mainFormLayout->addWidget(fromLabel, 3, 0);

    ui_fromBox = new RamObjectComboBox(this);
    ui_mainFormLayout->addWidget(ui_fromBox, 3, 1);

    QLabel *toLabel = new QLabel("To", this);
    ui_mainFormLayout->addWidget(toLabel, 4, 0);

    ui_toBox = new RamObjectComboBox(this);
    ui_mainFormLayout->addWidget(ui_toBox, 4, 1);

    ui_pipeFileList = new ObjectListWidget(true, RamUser::ProjectAdmin, this);
    ui_pipeFileList->setEditMode(ObjectListWidget::UnassignObjects);
    ui_pipeFileList->setTitle("Files");
    ui_mainLayout->addWidget(ui_pipeFileList);
}

void PipeEditWidget::connectEvents()
{
    connect(ui_fromBox, &RamObjectComboBox::currentObjectChanged, this, &PipeEditWidget::setOutputStep);
    connect(ui_toBox,  &RamObjectComboBox::currentObjectChanged, this, &PipeEditWidget::setInputStep);
    connect(ui_pipeFileList, &ObjectListWidget::add, this, &PipeEditWidget::createPipeFile);
}

