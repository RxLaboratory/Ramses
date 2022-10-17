#include "timelineproxy.h"

TimeLineProxy::TimeLineProxy(QObject *parent):
    QTransposeProxyModel(parent)
{

}

int TimeLineProxy::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}
