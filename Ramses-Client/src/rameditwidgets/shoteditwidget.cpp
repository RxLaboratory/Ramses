#include "shoteditwidget.h"
#include "ramproject.h"
#include "ramsequence.h"

ShotEditWidget::ShotEditWidget(QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();
}

ShotEditWidget::ShotEditWidget(RamShot *shot, QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    setObject(shot);
    connectEvents();
}

RamShot *ShotEditWidget::shot()
{
    return m_shot;
}

void ShotEditWidget::reInit(RamObject *o)
{
    m_shot = qobject_cast<RamShot*>(o);
    if (m_shot)
    {
        ui_secondsBox->setValue(m_shot->duration());
        secondsChanged();
        ui_folderWidget->setPath( m_shot->path() );

        // Set sequence
        RamProject *project = m_shot->project();
        QSignalBlocker b(ui_sequencesBox);
        ui_sequencesBox->setObjectModel(project->sequences());
        ui_sequencesBox->setObject( m_shot->sequence() );

        // Set assets
        ui_assetList->setObjectModel( m_shot->assets() );
        ui_assetList->setFilterList( project->assetGroups() );
        ui_assetList->setAssignList( project->assets() );
    }
    else
    {
        //Reset values
        ui_framesBox->setValue(0);
        ui_secondsBox->setValue(0);
        ui_folderWidget->setPath("");
        ui_sequencesBox->setCurrentIndex(-1);
    }
}

void ShotEditWidget::setDuration()
{
    if(!m_shot) return;
    m_shot->setDuration(ui_secondsBox->value());
}

void ShotEditWidget::setSequence(RamObject *seq)
{
    if (!m_shot) return;
    m_shot->setSequence(seq);
}

void ShotEditWidget::framesChanged()
{
    RamProject *proj = m_shot->project();
    if (!proj) return;

    ui_secondsBox->setValue( ui_framesBox->value() / proj->framerate() );
}

void ShotEditWidget::secondsChanged()
{
    RamProject *proj = m_shot->project();
    if (!proj) return;

    ui_framesBox->setValue( ui_secondsBox->value() * proj->framerate() );
}

void ShotEditWidget::setupUi()
{
    //Duration
    QLabel *durationLabel = new QLabel("Duration", this);
    ui_mainFormLayout->addWidget(durationLabel, 3, 0);

    ui_secondsBox = new AutoSelectDoubleSpinBox(this);
    ui_secondsBox->setMinimum(0.0);
    ui_secondsBox->setMaximum(14400.0);
    ui_secondsBox->setSingleStep(0.1);
    ui_secondsBox->setSuffix(" seconds");
    ui_mainFormLayout->addWidget(ui_secondsBox, 3, 1);

    ui_framesBox = new AutoSelectSpinBox(this);
    ui_framesBox->setMinimum(0);
    ui_framesBox->setMaximum(1728000);
    ui_framesBox->setSingleStep(1);
    ui_framesBox->setSuffix(" frames");
    ui_mainFormLayout->addWidget(ui_framesBox, 4, 1);

    QLabel *seqLabel = new QLabel("Sequence", this);
    ui_mainFormLayout->addWidget(seqLabel, 5,0);

    ui_sequencesBox = new RamObjectComboBox(this);
    ui_mainFormLayout->addWidget(ui_sequencesBox, 5, 1);

    ui_folderWidget = new DuQFFolderDisplayWidget(this);
    ui_mainLayout->addWidget( ui_folderWidget);

    ui_assetList = new ObjectListWidget(true, RamUser::Lead, this);
    ui_assetList->setEditMode(ObjectListWidget::UnassignObjects);
    ui_assetList->setEditable(true);
    ui_assetList->setSearchable(true);
    ui_assetList->setTitle("Assets");

    ui_mainLayout->addWidget(ui_assetList);
}

void ShotEditWidget::connectEvents()
{
    connect(ui_secondsBox, SIGNAL(editingFinished()), this, SLOT(secondsChanged()));
    connect(ui_secondsBox, SIGNAL(editingFinished()), this, SLOT(setDuration()));
    connect(ui_framesBox, SIGNAL(editingFinished()), this, SLOT(framesChanged()));
    connect(ui_framesBox, SIGNAL(editingFinished()), this, SLOT(setDuration()));
    connect(ui_sequencesBox, &RamObjectComboBox::currentObjectChanged, this, &ShotEditWidget::setSequence);
}
