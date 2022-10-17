#ifndef RAMSTATISTICSTABLE_H
#define RAMSTATISTICSTABLE_H

#include <QAbstractTableModel>
#include <QStringBuilder>

#include "ramproject.h"
#include "ramstep.h"

class StatisticsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StatisticsModel(QObject *parent = nullptr);

    // MODEL REIMPLEMENTATION
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setUser(RamUser *newUser);

private slots:
    void changeProject(RamProject *project);

    void insertStep(const QModelIndex &parent, int first, int last);
    void removeStep(const QModelIndex &parent, int first, int last);

    void estimationComputed();

private:

    RamProject *m_project = nullptr;
    RamUser *m_user = nullptr;

    // Connect submodels and relay events
    void connectEvents();
};

#endif // RAMSTATISTICSTABLE_H
