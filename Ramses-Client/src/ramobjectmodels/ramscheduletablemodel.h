#ifndef RAMSCHEDULETABLEMODEL_H
#define RAMSCHEDULETABLEMODEL_H

#include <QStringBuilder>
#include "ramobjectmodel.h"

class RamScheduleTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit RamScheduleTableModel(QObject *parent = nullptr);
    void setObjectModel(RamObjectModel *userList, RamObjectModel *comments);

    // MODEL REIMPLEMENTATION
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public slots:
    void setStartDate(const QDate &newStartDate);
    void setEndDate(const QDate &newEndDate);

private slots:
    void insertUser(const QModelIndex &parent, int first, int last);
    void removeUser(const QModelIndex &parent, int first, int last);
    void resetUsers();

private:
     RamObjectModel *m_users = nullptr;
     RamObjectModel *m_comments = nullptr;

     QDate m_startDate;
     QDate m_endDate;
};

#endif // RAMSCHEDULETABLEMODEL_H
