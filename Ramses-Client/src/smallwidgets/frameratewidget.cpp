#include "frameratewidget.h"

FramerateWidget::FramerateWidget(QWidget *parent) : QWidget(parent)
{
    setupUi();
    connectEvents();
}

double FramerateWidget::framerate() const
{
    return framerateBox->value();
}

void FramerateWidget::setFramerate(double fr)
{
    framerateBox->setValue(fr);
}

void FramerateWidget::setPreset(int p)
{
    if (p < 1) return;

    framerateBox->setValue( presetsBox->itemData(p).toDouble());
}

void FramerateWidget::selectPreset(double fr)
{
    presetsBox->setCurrentIndex(0);
    for (int i =0; i < presetsBox->count(); i++)
    {
        if (presetsBox->itemData(i).toDouble() == fr)
        {
            presetsBox->setCurrentIndex(i);
            break;
        }
    }

    emit framerateChanged(fr);
}

void FramerateWidget::setupUi()
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout *l = new QVBoxLayout();
    l->setContentsMargins(0,0,0,0);
    l->setSpacing(3);

    presetsBox = new QComboBox(this);
    presetsBox->addItem("Custom", 0.0);
    presetsBox->addItem("8 fps", 8.0);
    presetsBox->addItem("12 fps", 12.0);
    presetsBox->addItem("15 fps", 15.0);
    presetsBox->addItem("23.976 fps", 23.976);
    presetsBox->addItem("24 fps", 24.0);
    presetsBox->addItem("25 fps", 25.0);
    presetsBox->addItem("29.97 fps", 29.97);
    presetsBox->addItem("30 fps", 30.0);
    presetsBox->addItem("50 fps", 50.0);
    presetsBox->addItem("59.94 fps", 59.94);
    presetsBox->addItem("60 fps", 60.0);
    presetsBox->addItem("120 fps", 120.0);
    presetsBox->setCurrentIndex(5);
    l->addWidget(presetsBox);

    framerateBox = new AutoSelectDoubleSpinBox(this);
    framerateBox->setMinimum(0.0);
    framerateBox->setMaximum(1000.0);
    framerateBox->setDecimals(3);
    framerateBox->setValue(24.0);
    l->addWidget(framerateBox);

    this->setLayout(l);
}

void FramerateWidget::connectEvents()
{
    connect(presetsBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setPreset(int)));
    connect(framerateBox, SIGNAL(valueChanged(double)), this, SLOT(selectPreset(double)));
}
