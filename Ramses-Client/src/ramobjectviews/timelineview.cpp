#include "timelineview.h"

#include "ramproject.h"
#include "timelinemanager.h"
#include "ramses.h"

TimelineView::TimelineView(QWidget *parent):
    QTableView(parent)
{
    m_delegate = new TimelineDelegate();

    m_objectList = new TimeLineProxy(this);
    this->setModel(m_objectList);

    setupUi();
    connectEvents();
}

void TimelineView::setObjectModel(RamObjectModel *shots)
{
    if (m_objectList->sourceModel() && m_objectList->sourceModel() != RamObjectModel::emptyModel())
        disconnect(m_objectList->sourceModel(), nullptr, this, nullptr);
    if (!shots)
    {
        m_objectList->setSourceModel(RamObjectModel::emptyModel());
    }
    else
    {
        m_objectList->setSourceModel(shots);
        connect(shots, &RamObjectModel::dataChanged, this, &TimelineView::resetZoom);
        connect(shots, &RamObjectModel::layoutChanged, this, &TimelineView::resetZoom);
        connect(shots, &RamObjectModel::rowsMoved, this, &TimelineView::resetZoom);
    }
    resetZoom();
}

void TimelineView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mouseTransformation == Pan)
    {
        QPoint newPos = event->globalPos();
        QPoint _delta = newPos - m_initialDragPos;
        this->horizontalScrollBar()->setValue( this->horizontalScrollBar()->value() - _delta.x() );
        this->verticalScrollBar()->setValue( this->verticalScrollBar()->value() - _delta.y() );
        m_initialDragPos = newPos;
        event->accept();
        return;
    }
    else if (m_mouseTransformation == Zoom)
    {
        const qreal dist = event->globalPos().x() - m_initialDragPos.x();
        zoom(dist);
        m_initialDragPos = event->globalPos();
    }
    QTableView::mouseMoveEvent(event);
}

void TimelineView::mousePressEvent(QMouseEvent *event)
{
    // Middle click for dragging view
    if (event->button() == Qt::MiddleButton)
    {
        if (event->modifiers().testFlag(Qt::ControlModifier))
        {
            m_mouseTransformation = Zoom;
        }
        else
        {
            m_mouseTransformation = Pan;
        }
        m_initialDragPos = event->globalPos();
        event->accept();
        return;
    }
    // Simple click detection
    else if (event->button() == Qt::LeftButton && event->modifiers().testFlag(Qt::NoModifier))
    {
        // Get the index
        m_clicking = this->indexAt( event->localPos().toPoint() );
    }
    QTableView::mousePressEvent(event);
}

void TimelineView::mouseReleaseEvent(QMouseEvent *event)
{
    // Middle click for dragging view
    if (event->button() == Qt::MiddleButton)
    {
        m_mouseTransformation = None;
        event->accept();
        return;
    }
    // Simple click to select
    else if (m_clicking.isValid())
    {
        // Check index
        QModelIndex testIndex = this->indexAt( event->localPos().toPoint() );
        if (testIndex == m_clicking && event->modifiers().testFlag(Qt::NoModifier)) select(m_clicking);
        // Release
        m_clicking = QModelIndex();
    }
    QTableView::mouseReleaseEvent(event);
}

void TimelineView::resizeEvent(QResizeEvent *event)
{
    this->setRowHeight(0, event->size().height());
}

void TimelineView::columnMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    Q_UNUSED(logicalIndex);

    QSignalBlocker b(this->horizontalHeader());

    // Actually move the row in the model
    m_objectList->moveColumn(QModelIndex(), oldVisualIndex, QModelIndex(), newVisualIndex);

    // move back to the (new) logical index
    this->horizontalHeader()->moveSection(newVisualIndex, oldVisualIndex);

    // Reset column widths
    resetZoom();
}

void TimelineView::changeProject(RamProject *project)
{
    if (project) this->setObjectModel(project->shots());
    else this->setObjectModel(nullptr);
}

void TimelineView::select(const QModelIndex &index)
{
    QString shotUuid = index.data(RamObject::UUID).toString();
    if (shotUuid == "") return;
    RamShot *shot = RamShot::get( shotUuid );

    if (!shot) return;
    this->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
    emit objectSelected(shot);
}

void TimelineView::selectShot(RamShot *shot)
{
    select(shot);
}

void TimelineView::select(RamObject *o)
{
    for (int i = 0; i< m_objectList->columnCount(); i++)
    {
        QString shotUuid = m_objectList->data( m_objectList->index(0, i), RamObject::UUID).toString();
        if (shotUuid == "") return;
        RamShot *shot = RamShot::get( shotUuid );
        if (!shot) return;
        if (shot->is(o))
        {
            QModelIndex index = m_objectList->index(0, i);
            this->setCurrentIndex( index );
            this->select( index );
            this->scrollTo(index, QAbstractItemView::PositionAtCenter);
            TimelineManager::instance()->setCurrentShot(shot);
            return;
        }
    }
}

void TimelineView::setZoom(double zoom)
{
    if (zoom <= 0) return;

    double previousZoom = m_zoom;
    m_zoom = zoom;

    // Get scroll to go back to the same value
    double val = this->horizontalScrollBar()->value();

    if (m_zoom == 1)
    {
        this->resizeColumnsToContents();
    }
    else
    {
        for (int i = 0; i< m_objectList->columnCount(); i++)
        {
            QModelIndex index = m_objectList->index(0, i);
            QSize size = m_delegate->sizeHint(QStyleOptionViewItem(), index);
            size = size * m_zoom;
            this->setColumnWidth(i, size.width());
        }
    }

    this->horizontalScrollBar()->setValue(val*m_zoom/previousZoom);
}

void TimelineView::resetZoom()
{
    setZoom(m_zoom);
}

void TimelineView::zoom(double amount)
{
    qreal s = 1.0 + amount / 10.0 * m_zoomSensitivity;

    setZoom( m_zoom*s );

    emit zoomed(m_zoom);
}

double TimelineView::currentZoom() const
{
    return m_zoom;
}

void TimelineView::wheelEvent(QWheelEvent *event)
{
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta();

    if (!numPixels.isNull())
    {
        zoom( numPixels.y() );
    }
    else if (!numDegrees.isNull())
    {
        zoom( double(numDegrees.y())/90.0 );
    }

    event->accept();
}

void TimelineView::setupUi()
{
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setFrameShape(QFrame::NoFrame);
    this->setDragDropMode(QAbstractItemView::NoDragDrop);
    this->setDragEnabled(false);
    this->setDefaultDropAction(Qt::IgnoreAction);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->verticalHeader()->setSectionsMovable(false);
    this->verticalHeader()->hide();
    this->horizontalHeader()->setSectionsMovable(true);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->setShowGrid(false);
    this->setRowHeight( 0, this->size().height() );
    this->setMouseTracking(true);

    QString style = "QTableView { gridline-color: rgba(0,0,0,0); selection-background-color: rgba(0,0,0,0); } ";
    style += "QTableView::item:hover { background-color: none; } ";
    this->setStyleSheet(style);

    this->setItemDelegate( m_delegate );
}

void TimelineView::connectEvents()
{
    // Update list when project changes
    connect(Ramses::instance(), &Ramses::currentProjectChanged, this, &TimelineView::changeProject);
    // Delegate buttons
    connect(m_delegate, SIGNAL(edited(RamObject*)), this, SLOT(select(RamObject*)));
    // Select
    connect(TimelineManager::instance(), &TimelineManager::currentShotChanged, this, &TimelineView::selectShot);
    // SORT
    connect( this->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(columnMoved(int,int,int)));
}
