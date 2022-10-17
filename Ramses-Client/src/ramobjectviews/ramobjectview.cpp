#include "ramobjectview.h"

#include "ramobjectsortfilterproxymodel.h"
#include "timelinemanager.h"

RamObjectView::RamObjectView(DisplayMode mode, QWidget *parent):
        QTableView(parent)
{
    m_displayMode = mode;
    setupUi();
    connectEvents();
}

void RamObjectView::setObjectModel(RamObjectModel *list)
{
    m_objectModel->setSourceModel(list);
    this->setModel(m_objectModel);
    //this->resizeRowsToContents();
    //this->resizeColumnsToContents();
}

void RamObjectView::setEditableObjects(bool editableObjects, RamUser::UserRole editRole)
{
    m_delegate->setEditable(editableObjects);
    m_delegate->setEditRole(editRole);
}

void RamObjectView::setSortable(bool sortable)
{
    this->verticalHeader()->setSectionsMovable(sortable);
}

void RamObjectView::setSortRole(RamObject::DataRole role)
{
    m_objectModel->setSortRole(role);
    m_objectModel->sort(0);
}

void RamObjectView::sort()
{
    m_objectModel->sort(0);
}

void RamObjectView::setCompletionRatio(bool showCompletion)
{
    m_delegate->setCompletionRatio(showCompletion);
}

void RamObjectView::showDetails(bool s)
{
    m_delegate->showDetails(s);
    this->resizeRowsToContents();
}

void RamObjectView::search(QString s)
{
    // Search
    m_objectModel->search(s);
    this->resizeRowsToContents();
}

void RamObjectView::mouseMoveEvent(QMouseEvent *event)
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

void RamObjectView::mousePressEvent(QMouseEvent *event)
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

void RamObjectView::mouseReleaseEvent(QMouseEvent *event)
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

void RamObjectView::resizeEvent(QResizeEvent *event)
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

void RamObjectView::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    if (m_layout) return;
    //this->resizeRowsToContents();
    //this->resizeColumnsToContents();
    m_layout = true;

    QTableView::showEvent(event);
}

void RamObjectView::select(const QModelIndex &index)
{
    RamObject *obj = m_objectModel->get(index);
    if (!obj) return;
    this->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
    emit objectSelected(obj);
}

void RamObjectView::rowMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    Q_UNUSED(logicalIndex);

    QSignalBlocker b(this->verticalHeader());

    //m_objectList->moveRow(QModelIndex(), oldVisualIndex, QModelIndex(), newVisualIndex);

    // Get the source model to move the row
    QAbstractItemModel *model = m_objectModel->sourceModel();
    // Convert the filtered index to the model index
    QModelIndex oldIndex = m_objectModel->index(oldVisualIndex, 0);
    QModelIndex newIndex = m_objectModel->index(newVisualIndex, 0);
    oldIndex = m_objectModel->mapToSource(oldIndex);
    newIndex = m_objectModel->mapToSource(newIndex);
    model->moveRow(QModelIndex(), oldIndex.row(), QModelIndex(), newIndex.row());//*/

    // move back to the (new) logical index
    this->verticalHeader()->moveSection(newVisualIndex, oldVisualIndex);
}

void RamObjectView::select(RamObject *o)
{
    if (!m_objectModel) return;
    for (int i = 0; i< m_objectModel->rowCount(); i++)
    {
        RamObject *obj = m_objectModel->get(i);
        if (obj->is(o))
        {
            QModelIndex index = m_objectModel->index(i, 0);
            this->setCurrentIndex( index );
            this->select( index );
            this->scrollTo(index, QAbstractItemView::PositionAtTop);
            RamShot *shot = RamShot::c(o);
            if (shot) TimelineManager::instance()->setCurrentShot(shot);
            return;
        }
    }
}

void RamObjectView::filter(RamObject *o)
{
    if (!o) m_objectModel->setFilterUuid("");
    else m_objectModel->setFilterUuid(o->uuid());
    this->resizeRowsToContents();
}

void RamObjectView::setupUi()
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
    this->setColumnWidth( 0, this->size().width() );
    this->setMouseTracking(true);

    QString style = "QTableView { padding-top: 3px; padding-bottom: 3px; gridline-color: rgba(0,0,0,0); selection-background-color: rgba(0,0,0,0); } ";
    style += "QTableView::item:hover { background-color: none; } ";
    this->setStyleSheet(style);

    m_delegate = new RamObjectDelegate();
    this->setItemDelegate( m_delegate );

    // Set model
    m_objectModel = new RamObjectSortFilterProxyModel(this);
    this->setModel(m_objectModel);

    if (m_displayMode == List)
    {
        this->horizontalHeader()->hide();
        this->horizontalHeader()->setStretchLastSection(true);
        m_objectModel->setSingleColumn(true);
    }
}

void RamObjectView::connectEvents()
{
    connect(m_delegate, SIGNAL(edited(RamObject*)), this, SLOT(select(RamObject*)));
    // SORT
    connect( this->verticalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(rowMoved(int,int,int)));
    // SELECT
    // Unselect before filtering
    connect(m_objectModel, SIGNAL(aboutToFilter()), this->selectionModel(), SLOT(clear()));
}
