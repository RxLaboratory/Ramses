#include "itemtablewidget.h"

ItemTableWidget::ItemTableWidget(QWidget *parent):
    QTableWidget(parent)
{
    setupUi();
    //connectEvents();
    //m_ready = true;
}

void ItemTableWidget::setStepVisible(QString stepUuid, bool visible)
{
    int col = stepColumn(stepUuid);
    if (col < 0) return;
    this->setColumnHidden(col, !visible);
}

void ItemTableWidget::search(QString s)
{
    QHeaderView *header = this->verticalHeader();
    for( int row = 0; row < this->rowCount(); row++)
    {
        if (s == "")
        {
            header->setSectionHidden(row, false);
            continue;
        }

        // short name
        if (objShortName(row).contains(s, Qt::CaseInsensitive))
        {
            header->setSectionHidden(row, false);
            continue;
        }

        if (objName(row).contains(s, Qt::CaseInsensitive))
        {
            header->setSectionHidden(row, false);
            continue;
        }

        header->setSectionHidden(row, true);
    }
}

void ItemTableWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (_dragging)
    {
        QPoint newPos = event->globalPos();
        QPoint _delta = newPos - _initialDragPos;
        this->horizontalScrollBar()->setValue( this->horizontalScrollBar()->value() - _delta.x() );
        this->verticalScrollBar()->setValue( this->verticalScrollBar()->value() - _delta.y() );
        _initialDragPos = newPos;
        event->accept();
        return;
    }
    QTableWidget::mouseMoveEvent(event);
}

void ItemTableWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        _initialDragPos = event->globalPos();
        _dragging = true;
        event->accept();
        return;
    }
    QTableWidget::mousePressEvent(event);
}

void ItemTableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        _dragging = false;
        event->accept();
        return;
    }
    QTableWidget::mouseReleaseEvent(event);
}

void ItemTableWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}

int ItemTableWidget::addStep(RamStep *step)
{
    if (!step) return -1;

    // Make sure it doesn't already exists
    for (int col = 1; col < this->columnCount(); col++)
    {
        if (stepUuid(col) == step->uuid())
            return col;
    }

    int col = this->columnCount();

    this->setColumnCount(col + 1);

    QTableWidgetItem *stepItem = new QTableWidgetItem(step->name());
    stepItem->setData(Qt::UserRole, step->uuid());

    this->setHorizontalHeaderItem(col,stepItem);

    QList<QMetaObject::Connection> c;
    c << connect(step, &RamObject::changed, this, &ItemTableWidget::stepChanged);
    m_stepConnections[step->uuid()] = c;

    emit newStep(step);

    return col;
}

void ItemTableWidget::removeStep(RamObject *stepObj)
{
    if (!stepObj) return;

    disconnectStep( stepObj->uuid() );

    for (int i = this->columnCount() -1; i > 0; i--)
    {
        QString colUuid = stepUuid(i);
        if (colUuid == stepObj->uuid())
        {
            this->removeColumn(i);
            emit stepRemoved(stepObj);
            return;
        }
    }
}

void ItemTableWidget::stepChanged(RamObject *stepObj)
{
    int col = stepColumn(stepObj);
    this->horizontalHeaderItem(col)->setText(stepObj->name());
}

void ItemTableWidget::statusAdded(RamObject *statusObj, int index)
{
    Q_UNUSED(index);
    if(statusObj->objectType() != RamObject::Status) return;
    RamStatus *status = qobject_cast<RamStatus*>( statusObj );
    if (!status) return;
    RamItem *item = status->item();
    RamStep *step = status->step();

    setStatusWidget(item, step);
}

void ItemTableWidget::stepStatusHistoryAdded(RamObject *sshObj)
{
    if (sshObj->objectType() != RamObject::StepStatusHistory) return;

    RamStepStatusHistory *ssh = qobject_cast<RamStepStatusHistory*>( sshObj );
    if (!ssh) return;
    RamStep *step = ssh->step();
    if (step) addStep(step);
    return;
}

void ItemTableWidget::stepStatusHistoryRemoved(RamObject *sshObj)
{
    if (sshObj->objectType() != RamObject::StepStatusHistory) return;
    RamStepStatusHistory *ssh = qobject_cast<RamStepStatusHistory*>( sshObj );
    if (!ssh) return;
    RamStep *step = ssh->step();
    if (step) removeStep(step);
    return;
}

void ItemTableWidget::statusRemoved(RamObject *statusObj)
{
    if(statusObj->objectType() != RamObject::Status) return;
    RamStatus *status = qobject_cast<RamStatus*>( statusObj );
    if (!status) return;
    RamItem *item = status->item();
    RamStep *step = status->step();

    setStatusWidget(item, step);
}

void ItemTableWidget::objectChanged(RamObject *obj)
{
    int row = objRow(obj);
    this->item(row, 0)->setText("   " + obj->name());
    QTableWidgetItem *headerItem = this->verticalHeaderItem(row);
    headerItem->setText(obj->shortName());
}

void ItemTableWidget::objectUnassigned(RamObject *obj)
{
    disconnectObject(obj);

    int row = objRow(obj);
    this->removeRow(row);
}

void ItemTableWidget::objectAssigned(RamObject *obj)
{
    // Check type to create widget
    RamObject::ObjectType type = obj->objectType();
    RamObjectWidget *ow;

    switch (type)
    {
    case RamObject::Asset:
    {
        RamAsset *a = dynamic_cast<RamAsset*>(obj);
        if (a) ow = new RamAssetWidget(a, this);
        else ow = new RamObjectWidget(obj,this);
        break;
    }
    case RamObject::Shot:
    {
        RamShot *s = dynamic_cast<RamShot*>(obj);
        if (s) ow = new RamShotWidget(s, this);
        else ow = new RamObjectWidget(obj,this);
        break;
    }
    default:
        return;
    }

    //ow->setEditable(m_editableObjects);

    int row = this->rowCount();
    this->setRowCount( row + 1 );
    QTableWidgetItem *cellItem = new QTableWidgetItem("    " + obj->name());
    this->setItem(row, 0, cellItem);
    this->setCellWidget(row, 0, ow);
    QTableWidgetItem *header = new QTableWidgetItem(obj->shortName());
    header->setData(Qt::UserRole, obj->uuid() );
    this->setVerticalHeaderItem(row, header);

    RamItem *item = qobject_cast<RamItem*>( obj );


    this->resizeRowToContents(row);

    // Connect changes & status history
    QList<QMetaObject::Connection> c;
    c << connect(statusHistory, &RamObjectList::objectAdded, this, &ItemTableWidget::stepStatusHistoryAdded);
    c << connect(statusHistory, &RamObjectList::objectRemoved, this, &ItemTableWidget::stepStatusHistoryRemoved);
    c << connect(statusHistory, &RamObjectUberList::objectAdded, this, &ItemTableWidget::statusAdded);
    c << connect(statusHistory, &RamObjectUberList::objectRemoved, this, &ItemTableWidget::statusRemoved);
    c << connect(obj, &RamObject::changed, this, &ItemTableWidget::objectChanged);
    m_objectConnections[obj->uuid()] = c;
}


void ItemTableWidget::connectEvents()
{

}

void ItemTableWidget::disconnectObject(RamObject *obj)
{
    if (m_objectConnections.contains(obj->uuid()))
    {
        QList<QMetaObject::Connection> c = m_objectConnections.take(obj->uuid());
        while (!c.isEmpty()) disconnect( c.takeLast() );
    }
}

void ItemTableWidget::disconnectStep(QString stepUuid)
{
    if (m_stepConnections.contains( stepUuid ))
    {
        QList<QMetaObject::Connection> c = m_stepConnections.take(stepUuid);
        while (!c.isEmpty())
            disconnect( c.takeLast() );
    }
}

void ItemTableWidget::setStatusWidget(RamItem *item, RamStep *step)
{
    // Get row & col to add step if needed
    int row = objRow(item);
    if (row < 0) return;
    int col = addStep(step);
    if (col < 0) return;

    // Get the latest status or create one
    RamStatus *status = item->status(step);
    if (!status) status =  generateDefaultStatus(item, step);
    if (!status) return;

    // Add the new status widget (the previous one is automatically deleted)
    RamStatusWidget *sw = new RamStatusWidget(status, this);
    sw->showHistoryButton();
    sw->setAdditiveMode();
    QTableWidgetItem *cellItem = new QTableWidgetItem("    " + status->state()->name());
    cellItem->setData(Qt::UserRole, status->uuid());
    this->setItem(row, col, cellItem);
    this->setCellWidget(row, col, sw);
}

RamStatus *ItemTableWidget::generateDefaultStatus(RamItem *item, RamStep *step)
{
    RamStatus *status = nullptr;
    RamUser *u = Ramses::instance()->ramUser();
    if (!u) u = Ramses::instance()->currentUser();
    RamState *s = Ramses::instance()->noState();
    if (u && s && step) status = new RamStatus( u, s, step, item );
    return status;
}

QString ItemTableWidget::objUuid(int row)
{
    return this->verticalHeaderItem(row)->data(Qt::UserRole).toString();
}

QString ItemTableWidget::objShortName(int row)
{
    return this->verticalHeaderItem(row)->text();
}

QString ItemTableWidget::objName(int row)
{
    return this->item(row, 0)->text().trimmed();
}

int ItemTableWidget::objRow(RamObject *o)
{
    for(int row = 0; row < this->rowCount(); row++)
    {
        if (objUuid(row) == o->uuid())
        {
            return row;
        }
    }
    return -1;
}

QString ItemTableWidget::stepUuid(int col)
{
    return this->horizontalHeaderItem(col)->data(Qt::UserRole).toString();
}

int ItemTableWidget::stepColumn(QString stepUuid)
{
    for (int i =1; i<this->columnCount();i++)
    {
        if ( this->horizontalHeaderItem(i)->data(Qt::UserRole).toString() == stepUuid)
            return i;
    }
    return -1;
}

int ItemTableWidget::stepColumn(RamObject *stepObj)
{
    return stepColumn(stepObj->uuid());
}



void ItemTableWidget::setupUi()
{
    this->setFrameShape(QFrame::NoFrame);
    this->setDragDropMode(NoDragDrop);
    this->setDragEnabled(false);
    this->setDefaultDropAction(Qt::IgnoreAction);
    this->setShowGrid(false);

    this->setRowCount(0);
    this->setColumnCount(1);

    this->setHorizontalHeaderItem(0, new QTableWidgetItem("Item"));

    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    this->horizontalHeader()->setSectionsMovable(true);

    this->setAlternatingRowColors(true);

    int p = DuUI::getSize("padding", "large");
    int sp = DuUI::getSize("margin");
    QString padding = QString::number(p) + "px";
    QString smallPadding = QString::number(sp) + "px";
    QString style = "QTableWidget { gridline-color: rgba(0,0,0,0); selection-background-color: rgba(0,0,0,0); } ";
    style += "QTableWidget::item { padding-left: " + padding + "; padding-right: " + padding + "; padding-top: " + smallPadding + "; } ";
    style += "QTableWidget::item:hover { background-color: none; } ";

    this->setStyleSheet(style);
}
