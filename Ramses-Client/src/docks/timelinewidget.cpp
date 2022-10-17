#include "timelinewidget.h"

TimelineWidget::TimelineWidget(QWidget *parent)
    : QWidget{parent}
{
    setupUi();
    connectEvents();
}

void TimelineWidget::setupUi()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(3);

    QVBoxLayout *buttonsLayout = new QVBoxLayout();
    buttonsLayout->setContentsMargins(0,0,0,0);
    buttonsLayout->setSpacing(3);

    mainLayout->addLayout(buttonsLayout);

    buttonsLayout->addStretch();

    ui_zoomSlider = new DuQFDoubleSlider(this);
    ui_zoomSlider->setMinimum(0.1);
    ui_zoomSlider->setMaximum(4.0);
    ui_zoomSlider->showValue(false);
    ui_zoomSlider->setPrefix("Zoom");
    ui_zoomSlider->setMaximumWidth(100);
    ui_zoomSlider->setMaximumHeight(20);
    ui_zoomSlider->setValue(1.0);
    buttonsLayout->addWidget(ui_zoomSlider);

    buttonsLayout->addStretch();

    ui_timeline = new TimelineView(this);
    mainLayout->addWidget(ui_timeline);
}

void TimelineWidget::connectEvents()
{
    connect(ui_zoomSlider, SIGNAL(valueChanging(double)), ui_timeline, SLOT(setZoom(double)));
    connect(ui_timeline, SIGNAL(zoomed(double)), ui_zoomSlider, SLOT(setValue(double)));
}
