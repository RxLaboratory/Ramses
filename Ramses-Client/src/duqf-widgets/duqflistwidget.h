#ifndef DUQFLISTWIDGET_H
#define DUQFLISTWIDGET_H

#include <QListWidget>
#include <QDropEvent>

class DuQFListWidget : public QListWidget
{
    Q_OBJECT
public:
    DuQFListWidget(QWidget *parent = nullptr);
    void setDragable(bool dragable=true);
signals:
    void itemDropped();
protected:
    void dropEvent(QDropEvent *event);

};

#endif // DUQFLISTWIDGET_H
