#include "ramitemtable.h"
#include "ramproject.h"
#include "ramstate.h"
#include "ramstatus.h"
#include "ramshot.h"
#include "ramasset.h"
#include "ramobjectmodel.h"

// PUBLIC //

QMap<QString, RamItemTable*> RamItemTable::m_existingObjects = QMap<QString, RamItemTable*>();

RamItemTable *RamItemTable::get(QString uuid)
{
    if (!checkUuid(uuid, ItemTable)) return nullptr;

    if (m_existingObjects.contains(uuid)) return m_existingObjects.value(uuid);

    // Finally return a new instance
    return new RamItemTable(uuid);
}

RamItemTable *RamItemTable::c(RamObjectList *o)
{
    return qobject_cast<RamItemTable*>(o);
}

RamItemTable::RamItemTable(QString shortName, QString name, ObjectType type, QObject *parent, DataListMode mode):
    RamObjectList(shortName, name, type, mode, parent, ItemTable)
{
    m_existingObjects[m_uuid] = this;
    connectEvents();
}

RamItemTable::RamItemTable(QString uuid, QObject *parent):
    RamObjectList(uuid, parent, ItemTable)
{
    m_existingObjects[m_uuid] = this;
    connectEvents();
}

int RamItemTable::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (m_objectList.isEmpty()) return 0;

    RamProject *p = project();

    return p->steps()->rowCount() + 1;
}

QVariant RamItemTable::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if (row < 0 || col < 0)
    {
        if (role > Qt::UserRole) return INT32_MIN;
        return 0;
    }

    // Return
    if (col == 0)
        return RamObjectList::data(index, role);

    // Get the item
    RamAbstractItem *item = itemAt(row);

    if (role == ShortName)
    {
        return item->shortName();
    }
    else if (role == Name )
    {
        return item->name();
    }
    else if (role == Qt::InitialSortOrderRole)
    {
        return row;
    }

    // Get the step
    RamStep *step = stepAt(col);

    // Get the status
    RamStatus *status = item->status(step);

    if (!status)
    {
        status = RamStatus::noStatus(item, step);
    }

    if (role == Qt::DisplayRole)
        return status->name();

    if (role == Qt::StatusTipRole)
    {
        QString tip = status->state()->shortName() % " | " % status->state()->name();
        int timeSpent = status->timeSpent()/3600;
        tip = tip % " (" % QString::number(status->completionRatio()) % "%) | " %
                QString::number(timeSpent) % " hours (" %
                QString::number( RamStatus::hoursToDays(timeSpent), 'f', 2) % " days)";
        return tip;
    }

    if (role == Qt::ToolTipRole)
    {
        int timeSpent = status->timeSpent()/3600;
        float est = 0;
        if (status->useAutoEstimation()) est = status->estimation();
        else est = status->goal();
        return QString(
                    status->state()->shortName() %
                    " | " %
                    status->state()->name() %
                    "\nCompletion: " %
                    QString::number( status->completionRatio() ) %
                    "%\nTime spent: " %
                    QString::number(timeSpent) % " hours (" %
                    QString::number( RamStatus::hoursToDays(timeSpent), 'f', 2) % " days)" %
                    " / " % QString::number( est, 'f', 2 ) % " days" %
                    "\n" %
                    status->comment() );
    }

    if (role == Difficulty) // Difficulty order
    {
        return status->difficulty();
    }
    else if (role == TimeSpent) // Time spent order
    {
        return status->timeSpent();
    }
    else if (role == Estimation) // Estimation order
    {
        if (status->useAutoEstimation()) return status->estimation();
        else return status->goal();
    }
    else if (role == Completion) // Completion order
    {
        return status->completionRatio();
    }
    else if (role == Pointer)
    {
        return reinterpret_cast<quintptr>(status);
    }

    return QVariant();
}

QVariant RamItemTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical) return RamObjectList::headerData(section, orientation, role);
    if (section == 0)
    {
        if ( role == Qt::DisplayRole )
        {
            return name();
        }
        return QVariant();
    }

    if ( role == Qt::DisplayRole )
        return stepAt(section)->name();

    if ( role == Qt::ForegroundRole )
        return QBrush(stepAt(section)->color());

    if ( role == Pointer)
        return reinterpret_cast<quintptr>( stepAt(section) );

    return QAbstractTableModel::headerData(section, orientation, role);
}

void RamItemTable::newSteps(const QModelIndex &parent, int first, int last)
{
    beginInsertColumns(parent, first+1, last+1);
    endInsertColumns();
}

void RamItemTable::removeSteps(const QModelIndex &parent, int first, int last)
{
    beginRemoveColumns(parent, first+1, last+1);
    endRemoveColumns();
}

void RamItemTable::moveSteps(const QModelIndex &parent, int first, int last, const QModelIndex &dest, int destRow)
{
    beginMoveColumns(parent, first+1, last+1, dest, destRow+1);
    endMoveColumns();
}

// PRIVATE SLOTS //

void RamItemTable::removeItem(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    for (int i = first; i <= last; i++)
    {
        RamAbstractItem *item = itemAt(i);
        Q_ASSERT(item);

        // Disonnect the item
        disconnect(item, nullptr, this, nullptr);
    }

}

void RamItemTable::statusChanged(RamAbstractItem *item, RamStep *step)
{
    // We just need to notify that the data has changed for the item/step

    int row = objRow( item );
    int col = stepCol( step );

    if (col < 0) return;
    if (row < 0) return;

    QModelIndex index = createIndex(row, col);

    emit dataChanged( index, index, {Qt::DisplayRole});

    emit headerDataChanged(Qt::Horizontal, col,col);
}

void RamItemTable::inserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    for (int i = first; i <= last; i++)
    {
        RamAbstractItem *item = itemAt(i);
        Q_ASSERT(item);

        // Connect the item to monitor its status
        connect( item, &RamAbstractItem::statusChanged, this, &RamItemTable::statusChanged );
    }
}

// PRIVATE //

void RamItemTable::connectEvents()
{
    connect( this, &RamItemTable::rowsInserted, this, &RamItemTable::inserted);
    connect( this, &RamItemTable::rowsAboutToBeRemoved, this, &RamItemTable::removeItem);
}

RamAbstractItem *RamItemTable::itemAt(int row) const
{
    if (m_contentType == Shot) return RamShot::c( m_objectList.at(row ) );
    else return RamAsset::c( m_objectList.at(row ) );
}

RamProject *RamItemTable::project() const
{
    if (m_objectList.isEmpty()) return nullptr;

    RamAbstractItem *item = itemAt(0);
    Q_ASSERT(item);

    RamProject *p = item->project();
    Q_ASSERT(p);

    return p;
}

RamStep *RamItemTable::stepAt(int col) const
{
    if (m_objectList.isEmpty()) return nullptr;

    RamProject *p = project();

    RamObject *o = p->steps()->get(col-1);
    return RamStep::c( o );
}

int RamItemTable::stepCol(RamStep *step) const
{
    if (m_objectList.isEmpty()) return -1;

    RamProject *p = project();

    for (int i = 0; i < p->steps()->rowCount(); i++)
    {
        RamObject *obj = p->steps()->get(i);
        if (obj->is(step)) return i+1;
    }
    return -1;
}
