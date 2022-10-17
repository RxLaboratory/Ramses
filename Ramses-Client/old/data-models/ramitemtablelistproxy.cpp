#include "ramitemtablelistproxy.h"

RamItemTableListProxy::RamItemTableListProxy(QObject *parent):
    QIdentityProxyModel(parent)
{

}

int RamItemTableListProxy::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}
