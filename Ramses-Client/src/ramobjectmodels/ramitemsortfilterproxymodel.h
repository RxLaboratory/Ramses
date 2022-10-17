#ifndef RAMITEMFILTERMODEL_H
#define RAMITEMFILTERMODEL_H

#include "ramobjectsortfilterproxymodel.h"
#include "ramstep.h"

/**
 * @brief The RamItemFilterModel class is used to filters items according to current state, step or assigned user.
 * It also sorts the items according to: their completion ratio, their estimation, their time spent, their difficulty, their name or their ID (or default)
 */
class RamItemSortFilterProxyModel : public RamObjectSortFilterProxyModel
{
    Q_OBJECT
public:

    explicit RamItemSortFilterProxyModel(QObject *parent = nullptr);

    void freeze();
    void unFreeze();

    void useFilters(bool use = true);

    void hideUser(RamObject *u);
    void showUser(RamObject *u);
    void clearUsers();
    void showUnassigned(bool show);

    void hideState(RamObject *s);
    void showState(RamObject *s);
    void clearStates();

    void setStepType(RamStep::Type t);
    void hideStep(RamObject *s);
    void showStep(RamObject *s);
    void showAllSteps();

    int sortMode() const;
    void setSortMode(int newSortMode);

public slots:
    void resort(int col, Qt::SortOrder order = Qt::AscendingOrder);
    void unsort();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool filterAcceptsColumn(int sourceRow, const QModelIndex &sourceParent) const override;

    QVector<RamObject*> m_states;
    QVector<RamObject*> m_users;
    QVector<RamObject*> m_hiddenSteps;
    bool m_showUnassigned = true;
    RamStep::Type m_stepType = RamStep::All;

private:
    /**
     * @brief step Get a step given a source column
     * @param column The column of the step in the source model
     * @return The RamStep* or nullptr if the step is filtered
     */
    RamStep *step( int column ) const;
    bool m_userFilters = false;

    bool m_frozen = false;

    int m_sortMode = Qt::InitialSortOrderRole;
};

#endif // RAMITEMFILTERMODEL_H
