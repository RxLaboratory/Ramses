#ifndef RAMPROJECTFILTERMODEL_H
#define RAMPROJECTFILTERMODEL_H

#include <QSortFilterProxyModel>

class RamProjectFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit RamProjectFilterModel(QObject *parent = nullptr);
    void addUser(QString uuid);
    void removeUser(QString uuid);
    void clearUsers();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private slots:
    void doInvalidateFilter();

private:
    QStringList m_userUuids;
};

#endif // RAMPROJECTFILTERMODEL_H
