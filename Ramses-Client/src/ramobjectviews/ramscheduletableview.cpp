#include "ramscheduletableview.h"

RamScheduleTableView::RamScheduleTableView(QWidget *parent):
    QTableView(parent)
{
    setupUi();
    connectEvents();
}

void RamScheduleTableView::showDetails(bool s)
{
    m_delegate->showDetails(s);
    this->resizeRowsToContents();
    this->resizeColumnsToContents();
}

void RamScheduleTableView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging)
    {
        QPoint newPos = event->globalPos();
        QPoint _delta = newPos - m_initialDragPos;
        this->horizontalScrollBar()->setValue( this->horizontalScrollBar()->value() - _delta.x() );
        this->verticalScrollBar()->setValue( this->verticalScrollBar()->value() - _delta.y() );
        m_initialDragPos = newPos;
        event->accept();
        return;
    }
    QTableView::mouseMoveEvent(event);
}

void RamScheduleTableView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        m_initialDragPos = event->globalPos();
        m_dragging = true;
        event->accept();
        return;
    }
    QTableView::mousePressEvent(event);
}

void RamScheduleTableView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        m_dragging = false;
        event->accept();
        return;
    }
    QTableView::mouseReleaseEvent(event);
}

void RamScheduleTableView::setupUi()
{
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setFrameShape(QFrame::NoFrame);
    this->setDragDropMode(QAbstractItemView::NoDragDrop);
    this->setDragEnabled(false);
    this->setDefaultDropAction(Qt::IgnoreAction);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->verticalHeader()->setSectionsMovable(false);
    // For some reason, the vertical header width is too low
    this->verticalHeader()->setMinimumWidth(150);
    this->horizontalHeader()->setSectionsMovable(false);
    this->setShowGrid(false);
    //this->setMouseTracking(true);

    this->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

    QString style = "QTableView { padding-top: 3px; padding-bottom: 3px; gridline-color: rgba(0,0,0,0); selection-background-color: rgba(0,0,0,0); } ";
    style += "QTableView::item:hover { background-color: none; } ";
    this->setStyleSheet(style);

    m_delegate = new RamScheduleDelegate(this);
    this->setItemDelegate(m_delegate);
}

void RamScheduleTableView::connectEvents()
{

}
