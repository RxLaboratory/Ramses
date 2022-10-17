#include "resolutionwidget.h"

ResolutionWidget::ResolutionWidget(QWidget *parent) : QWidget(parent)
{
    setupUi();
    connectEvents();
}

int ResolutionWidget::getWidth() const
{
    return widthBox->value();
}

int ResolutionWidget::getHeight() const
{
    return heightBox->value();
}

void ResolutionWidget::setWidth(int w)
{
    widthBox->setValue(w);
}

void ResolutionWidget::setHeight(int h)
{
    heightBox->setValue(h);
}

void ResolutionWidget::setPreset(int p)
{
    if (p < 1) return;

    QList<QVariant> resolution = presetsBox->itemData(p).toList();

    widthBox->setValue(resolution.at(0).toInt());
    heightBox->setValue(resolution.at(1).toInt());
}

void ResolutionWidget::selectPreset()
{
    int w = widthBox->value();
    int h = heightBox->value();

    presetsBox->setCurrentIndex(0);

    for(int i = 0; i < presetsBox->count(); i++)
    {
        QList<QVariant> resolution = presetsBox->itemData(i).toList();
        if (w == resolution.at(0).toInt() && h == resolution.at(1).toInt())
        {
            presetsBox->setCurrentIndex(i);
            break;
        }
    }

    emit resolutionChanged(w, h);
}

void ResolutionWidget::setupUi()
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(3);

    presetsBox = new QComboBox(this);
    presetsBox->addItem("Custom", QList<QVariant>() << 0 << 0);
    presetsBox->addItem("Square HD ( 1080x1080 1:1 )", QList<QVariant>() << 1080 << 1080);
    presetsBox->addItem("HD ( 1280x720 1.78:1 )", QList<QVariant>() << 1280 << 720);
    presetsBox->addItem("HD ( 1920x1080 1.78:1 )", QList<QVariant>() << 1920 << 1080);
    presetsBox->addItem("2K DCP Flat ( 1998x1080 1.85:1 )", QList<QVariant>() << 1998 << 1080);
    presetsBox->addItem("2K DCP Scope ( 2048x858 2.39:1 )", QList<QVariant>() << 2048 << 858);
    presetsBox->addItem("2K DSM ( 2048x1080 1.9:1 )", QList<QVariant>() << 2048 << 1080);
    presetsBox->addItem("4K UHD ( 3840x2160 1.78:1 )", QList<QVariant>() << 3840 << 2160);
    presetsBox->addItem("4K DCP Flat ( 3996x2160 1.85:1 )", QList<QVariant>() << 3996 << 2160);
    presetsBox->addItem("4K DCP Scope ( 4096x1716 2.39:1 )", QList<QVariant>() << 4096 << 1716);
    presetsBox->addItem("4K DSM ( 4096x2160 1.9:1 )", QList<QVariant>() << 4096 << 2160);
    presetsBox->addItem("8K UHD ( 7680x4320 1.78:1 )", QList<QVariant>() << 7680 << 4320);
    presetsBox->setCurrentIndex(3);
    layout->addWidget(presetsBox);

    widthBox = new AutoSelectSpinBox(this);
    widthBox->setPrefix("Width: ");
    widthBox->setSuffix("px");
    widthBox->setMinimum(4);
    widthBox->setMaximum(10000);
    widthBox->setValue(1920);
    layout->addWidget(widthBox);

    heightBox = new AutoSelectSpinBox(this);
    heightBox->setPrefix("Height: ");
    heightBox->setSuffix("px");
    heightBox->setMinimum(4);
    heightBox->setMaximum(10000);
    heightBox->setValue(1080);
    layout->addWidget(heightBox);

    this->setLayout(layout);
}

void ResolutionWidget::connectEvents()
{
    connect(presetsBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setPreset(int)));
    connect(widthBox, SIGNAL(valueChanged(int)), this, SLOT(selectPreset()));
    connect(heightBox, SIGNAL(valueChanged(int)), this, SLOT(selectPreset()));
}
