#include "stateeditwidget.h"

StateEditWidget::StateEditWidget(RamState *state, QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();

    setObject(state);
}

StateEditWidget::StateEditWidget(QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();
}

RamState *StateEditWidget::state() const
{
    return m_state;
}

void StateEditWidget::reInit(RamObject *o)
{
    m_state = qobject_cast<RamState*>(o);
    if (m_state)
    {
        ui_colorSelector->setColor(m_state->color());
        ui_completionSpinBox->setValue(m_state->completionRatio());
    }
    else
    {
        ui_colorSelector->setColor(QColor(25,25,25));
        ui_completionSpinBox->setValue(50);
    }
}

void StateEditWidget::setCompletion(int c)
{
    if(!m_state) return;
    m_state->setCompletionRatio(c);
}

void StateEditWidget::setColor(QColor c)
{
    if(!m_state) return;
    m_state->setColor(c);
}

void StateEditWidget::setupUi()
{
    QLabel *colorLabel = new QLabel("Color", this);
    ui_mainFormLayout->addWidget(colorLabel, 3, 0);

    ui_colorSelector = new DuQFColorSelector(this);

    ui_mainFormLayout->addWidget(ui_colorSelector, 3, 1);

    QLabel *completionLabel = new QLabel("Completion ratio", this);
    ui_mainFormLayout->addWidget(completionLabel, 4, 0);

    ui_completionSpinBox = new DuQFSpinBox(this);
    ui_completionSpinBox->setSuffix("%");
    ui_completionSpinBox->setMaximumHeight(completionLabel->height());
    ui_mainFormLayout->addWidget(ui_completionSpinBox, 4, 1);

    ui_mainLayout->addStretch();

    ui_mainLayout->setStretch(0,0);
    ui_mainLayout->setStretch(1,0);
    ui_mainLayout->setStretch(2,100);

    m_dontRename << "TODO" << "STB" << "NO" << "WIP" << "OK";

    ui_mainLayout->addStretch();
}

void StateEditWidget::connectEvents()
{
    connect(ui_completionSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setCompletion(int)));
    connect(ui_colorSelector, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
}
