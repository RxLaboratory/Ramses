#include "sequenceeditwidget.h"

#include "ramsequence.h"
#include "ramshot.h"

SequenceEditWidget::SequenceEditWidget(QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();
}

SequenceEditWidget::SequenceEditWidget(RamSequence *sequence, QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();
    setObject(sequence);
}

RamSequence *SequenceEditWidget::sequence() const
{
    return m_sequence;
}

void SequenceEditWidget::reInit(RamObject *o)
{
    m_sequence = qobject_cast<RamSequence*>(o);
    if (m_sequence)
    {
        ui_shotsList->setObjectModel( m_sequence->project()->shots() );
        ui_shotsList->setFilter(m_sequence);
        ui_colorSelector->setColor(m_sequence->color());
    }
    else
    {
        ui_shotsList->setObjectModel(nullptr);
        ui_colorSelector->setColor(QColor(67,67,67));
    }
}

void SequenceEditWidget::setColor(QColor c)
{
    if (!m_sequence) return;
    m_sequence->setColor(c);
}

void SequenceEditWidget::createShot()
{
    if (!m_sequence) return;
    RamShot *shot = new RamShot(
                "NEW",
                "New Shot",
                m_sequence);
    m_sequence->project()->shots()->appendObject(shot->uuid());
    shot->edit();
}

void SequenceEditWidget::setupUi()
{
    QLabel *colorLabel = new QLabel("Color", this);
    ui_mainFormLayout->addWidget(colorLabel, 3, 0);

    ui_colorSelector = new DuQFColorSelector(this);
    ui_mainFormLayout->addWidget(ui_colorSelector, 3, 1);

    ui_shotsList = new ObjectListWidget(true, RamUser::ProjectAdmin, this);
    ui_shotsList->setEditMode(ObjectListWidget::UnassignObjects);
    ui_shotsList->setTitle("Shots");
    ui_mainLayout->addWidget(ui_shotsList);
}

void SequenceEditWidget::connectEvents()
{
    connect(ui_shotsList, SIGNAL(add()), this, SLOT(createShot()));
    connect(ui_colorSelector, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
}

