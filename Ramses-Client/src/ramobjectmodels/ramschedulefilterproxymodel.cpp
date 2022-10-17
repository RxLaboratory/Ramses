#include "ramschedulefilterproxymodel.h"

#include "ramobject.h"

RamScheduleFilterProxyModel::RamScheduleFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

bool RamScheduleFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    // Check user
    QString userUuid = sourceModel()->headerData(sourceRow, Qt::Vertical, RamObject::UUID).toString();
    if(m_filterUserUuids.contains( userUuid )) return false;
    return true;
}

bool RamScheduleFilterProxyModel::filterAcceptsColumn(int sourceCol, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    // check date
    QDate date = sourceModel()->headerData( sourceCol, Qt::Horizontal, RamObject::Date).value<QDate>();

    if (m_hiddenDays.contains(date.dayOfWeek())) return false;
    return true;
}

void RamScheduleFilterProxyModel::ignoreUserUuid(QString uuid)
{
    if (!m_filterUserUuids.contains(uuid))
    {
        m_filterUserUuids << uuid;
        invalidateFilter();
    }
}

void RamScheduleFilterProxyModel::acceptUserUuid(QString uuid)
{
    m_filterUserUuids.removeAll(uuid);
    invalidateFilter();
}

void RamScheduleFilterProxyModel::hideDay(int d)
{
    m_hiddenDays << d;
    invalidateFilter();
}

void RamScheduleFilterProxyModel::showDay(int d)
{
    m_hiddenDays.removeAll(d);
    invalidateFilter();
}
