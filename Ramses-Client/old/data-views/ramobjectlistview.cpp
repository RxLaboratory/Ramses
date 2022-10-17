#include "ramobjectlistview.h"

#include "data-models/ramitemtablelistproxy.h"
#include "timelinemanager.h"

RamObjectListView::RamObjectListView(DisplayMode mode, QWidget *parent):
        QTableView(parent)
{
    m_delegate = new RamObjectDelegateOld();
    m_displayMode = mode;
    setupUi();
    m_objectList = new RamItemFilterModel(this);
    if (mode == List)
    {
        RamItemTableListProxy *tlp = new RamItemTableListProxy(this);
        tlp->setSourceModel(m_objectList);
        this->setModel(tlp);
    }
    else
    {
        this->setModel(m_objectList);
    }
    connectEvents();
}

RamObjectListView::RamObjectListView(RamObjectList *list, DisplayMode mode, QWidget *parent):
    QTableView(parent)
{
    m_delegate = new RamObjectDelegateOld();
    m_displayMode = mode;
    setupUi();
    m_objectList = new RamItemFilterModel(this);
    if (mode == List)
    {
        RamItemTableListProxy *tlp = new RamItemTableListProxy(this);
        tlp->setSourceModel(m_objectList);
        this->setModel(tlp);
    }
    else
    {
        this->setModel(m_objectList);
    }
    this->setList(list);
    connectEvents();
}

RamObjectListView::RamObjectListView(QAbstractItemModel *list, bool editableObjects, RamUser::UserRole editRole, DisplayMode mode, QWidget *parent):
    QTableView(parent)
{
    m_displayMode = mode;
    m_delegate = new RamObjectDelegateOld();
    m_delegate->setEditable(editableObjects);
    m_delegate->setEditRole(editRole);
    setupUi();
    m_objectList = new RamItemFilterModel(this);
    if (mode == List)
    {
        RamItemTableListProxy *tlp = new RamItemTableListProxy(this);
        tlp->setSourceModel(m_objectList);
        this->setModel(tlp);
    }
    else
    {
        this->setModel(m_objectList);
    }
    setList(list);
    connectEvents();
}

void RamObjectListView::setList(QAbstractItemModel *list)
{
    m_objectList->setList(list);

    this->resizeRowsToContents();
    this->resizeColumnsToContents();
}

RamItemFilterModel *RamObjectListView::filteredList()
{
    return m_objectList;
}

void RamObjectListView::setEditableObjects(bool editableObjects, RamUser::UserRole editRole)
{
    m_delegate->setEditable(editableObjects);
    m_delegate->setEditRole(editRole);
}

void RamObjectListView::setSortable(bool sortable)
{
    this->verticalHeader()->setSectionsMovable(sortable);
}

void RamObjectListView::setSortMode(RamObjectList::DataRole mode)
{
    m_objectList->setSortMode(mode);
    m_objectList->sort(0);
}

void RamObjectListView::sort()
{
    m_objectList->sort(0);
}

void RamObjectListView::setTimeTracking(bool trackTime)
{
    m_delegate->setTimeTracking(trackTime);
}

void RamObjectListView::setCompletionRatio(bool showCompletion)
{
    m_delegate->setCompletionRatio(showCompletion);
}

void RamObjectListView::showDetails(bool s)
{
    m_delegate->showDetails(s);
    this->resizeRowsToContents();
}

void RamObjectListView::search(QString s)
{
    // Search
    m_objectList->search(s);
    //this->resizeRowsToContents();
}

void RamObjectListView::mouseMoveEvent(QMouseEvent *event)
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

void RamObjectListView::mousePressEvent(QMouseEvent *event)
{
    // Middle click for dragging view
    if (event->button() == Qt::MiddleButton)
    {
        m_initialDragPos = event->globalPos();
        m_dragging = true;
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

void RamObjectListView::mouseReleaseEvent(QMouseEvent *event)
{
    // Middle click for dragging view
    if (event->button() == Qt::MiddleButton)
    {
        m_dragging = false;
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

void RamObjectListView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    if (m_displayMode == List) {
        this->horizontalHeader()->setStretchLastSection(false);
        this->horizontalHeader()->setStretchLastSection(true);
    }
    else {
        // Incorrect draw after resize, fixed by resizing the first row...
        int rowHeight = this->rowHeight(0);
        this->setRowHeight(0,5);
        this->setRowHeight(0,rowHeight);
    }

    QTableView::resizeEvent(event);
}

void RamObjectListView::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    if (m_layout) return;
    this->resizeRowsToContents();
    this->resizeColumnsToContents();
    m_layout = true;

    QTableView::showEvent(event);
}

void RamObjectListView::select(const QModelIndex &index)
{
    RamObject *obj = RamObjectList::at(index);
    if (!obj) return;
    this->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
    emit objectSelected(obj);
}

void RamObjectListView::rowMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    Q_UNUSED(logicalIndex);

    QSignalBlocker b(this->verticalHeader());

    //m_objectList->moveRow(QModelIndex(), oldVisualIndex, QModelIndex(), newVisualIndex);

    // Get the source model to move the row
    QAbstractItemModel *model = m_objectList->sourceModel();
    // Convert the filtered index to the model index
    QModelIndex oldIndex = m_objectList->index(oldVisualIndex, 0);
    QModelIndex newIndex = m_objectList->index(newVisualIndex, 0);
    oldIndex = m_objectList->mapToSource(oldIndex);
    newIndex = m_objectList->mapToSource(newIndex);
    model->moveRow(QModelIndex(), oldIndex.row(), QModelIndex(), newIndex.row());//*/

    // move back to the (new) logical index
    this->verticalHeader()->moveSection(newVisualIndex, oldVisualIndex);
}

void RamObjectListView::select(RamObject *o)
{
    if (!m_objectList) return;
    for (int i = 0; i< m_objectList->rowCount(); i++)
    {
        RamObject *obj = m_objectList->at(i);
        if (obj->is(o))
        {
            QModelIndex index = m_objectList->index(i, 0);
            this->setCurrentIndex( index );
            this->select( index );
            this->scrollTo(index, QAbstractItemView::PositionAtTop);
            RamShot *shot = RamShot::c(o);
            if (shot) TimelineManager::instance()->setCurrentShot(shot);
            return;
        }
    }
}

void RamObjectListView::filter(RamObject *o)
{
    if (!o) m_objectList->setFilterUuid("");
    else m_objectList->setFilterUuid(o->uuid());
    this->resizeRowsToContents();
}

void RamObjectListView::setupUi()
{
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setFrameShape(QFrame::NoFrame);
    this->setDragDropMode(QAbstractItemView::NoDragDrop);
    this->setDragEnabled(false);
    this->setDefaultDropAction(Qt::IgnoreAction);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->verticalHeader()->setSectionsMovable(false);
    this->setShowGrid(false);
    if (m_displayMode == List) this->horizontalHeader()->hide();
    this->setColumnWidth( 0, this->size().width() );
    this->setMouseTracking(true);

    QString style = "QTableView { padding-top: 3px; padding-bottom: 3px; gridline-color: rgba(0,0,0,0); selection-background-color: rgba(0,0,0,0); } ";
    style += "QTableView::item:hover { background-color: none; } ";
    this->setStyleSheet(style);

    this->setItemDelegate( m_delegate );

    if (m_displayMode == List) this->horizontalHeader()->setStretchLastSection(true);
}

void RamObjectListView::connectEvents()
{
    connect(m_delegate, SIGNAL(edited(RamObject*)), this, SLOT(select(RamObject*)));
    // SORT
    connect( this->verticalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(rowMoved(int,int,int)));
    // SELECT
    // Unselect before filtering
    connect(m_objectList, SIGNAL(aboutToFilter()), this->selectionModel(), SLOT(clear()));
}
