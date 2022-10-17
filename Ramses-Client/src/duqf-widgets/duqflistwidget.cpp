#include "duqflistwidget.h"

DuQFListWidget::DuQFListWidget(QWidget *parent): QListWidget(parent)
{
    setDragable(true);
    this->setSelectionMode(ExtendedSelection);
    this->setFrameShape(QFrame::NoFrame);
}

void DuQFListWidget::setDragable(bool dragable)
{
    if (dragable)
    {
        this->setDragDropMode(InternalMove);
        this->setDragEnabled(true);
        this->setDefaultDropAction(Qt::MoveAction);
    }
    else
    {
        this->setDragDropMode(NoDragDrop);
        this->setDragEnabled(false);
        this->setDefaultDropAction(Qt::IgnoreAction);
    }
}

void DuQFListWidget::dropEvent(QDropEvent *event)
{
    QListWidget::dropEvent(event);
    emit itemDropped();
}
