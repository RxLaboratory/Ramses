#include "ramprojectfiltermodel.h"

#include "data-models/ramobjectlist.h"
#include "ramses.h"

RamProjectFilterModel::RamProjectFilterModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    RamObjectModel *projects = Ramses::instance()->projects();
    this->setDynamicSortFilter(true);
    this->setSourceModel(projects);
    connect(projects, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(doInvalidateFilter()));
    connect(projects, SIGNAL(modelReset()), this, SLOT(doInvalidateFilter()));
}

void RamProjectFilterModel::addUser(QString uuid)
{
    if(!m_userUuids.contains(uuid))
    {
        m_userUuids << uuid;
        invalidateFilter();
    }
}

void RamProjectFilterModel::removeUser(QString uuid)
{
    m_userUuids.removeAll(uuid);
    invalidateFilter();
}

void RamProjectFilterModel::clearUsers()
{
    m_userUuids.clear();
    invalidateFilter();
}

bool RamProjectFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    if ( m_userUuids.count() == 0 ) return true;

    RamObjectModel *projects = Ramses::instance()->projects();
    RamProject *project = RamProject::c( projects->get(sourceRow) );

    if (!project) return false;

    for (int i = 0; i < m_userUuids.count(); i++)
    {
        if ( project->users()->contains(m_userUuids.at(i))) return true;
    }

    return false;
}

void RamProjectFilterModel::doInvalidateFilter()
{
    invalidateFilter();
}
