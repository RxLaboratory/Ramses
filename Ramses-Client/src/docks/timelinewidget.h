#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>

#include "duqf-widgets/duqfdoubleslider.h"
#include "timelineview.h"

class TimelineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimelineWidget(QWidget *parent = nullptr);

private:
    void setupUi();
    void connectEvents();

    TimelineView *ui_timeline;
    DuQFDoubleSlider *ui_zoomSlider;
};

#endif // TIMELINEWIDGET_H
