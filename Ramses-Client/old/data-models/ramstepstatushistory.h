#ifndef RAMSTATUSHISTORY_H
#define RAMSTATUSHISTORY_H

#include "data-models/ramobjectlist.h"

class RamStatus;
class RamStep;
class RamAbstractItem;

/**
 * @brief The RamStepStatusHistory class is a list of RamStatus for a specific step and item
 * It is just a simple heritance of RamObjectList used to sort the RamStatus according to their date,
 * and it also keeps a pointer to the corresponding item and step.
 * Furthermore, it implements and 'edit' method, similar to RamObject::edit() which shows a dock widget listing all the RamStatus
 */
class RamStepStatusHistory : public RamObjectList
{
    Q_OBJECT
public:
    static RamStepStatusHistory *get(QString uuid);
    static RamStepStatusHistory *c(QObject *o);

    RamStepStatusHistory(RamStep *step, RamAbstractItem *item);
    RamAbstractItem *item() const;
    RamStep *step() const;

    // MODEL REIMPLEMENTATION
    virtual void sort(int column = 0, Qt::SortOrder order = Qt::AscendingOrder) override;

public slots:
    void edit(bool show = true);

signals:
    void latestStatusChanged(RamStepStatusHistory*);

protected:
    static QMap<QString, RamStepStatusHistory*> m_existingObjects;
    RamStepStatusHistory(QString uuid, QObject *parent = nullptr);

private slots:
    // monitors the changes to trigger computeEstimations on the step
    void rowsChanged(QModelIndex parent, int start, int end);
    void changeData(QModelIndex topLeft, QModelIndex bottomRight, QVector<int> roles);

private:
    void construct();
    // monitors the changes to trigger computeEstimations on the step
    void connectEvents();

    RamStep *m_step;
    RamAbstractItem *m_item;

    QFrame *ui_editWidget = nullptr;
};

bool statusSorter(RamObject *a, RamObject *b);

#endif // RAMSTATUSHISTORY_H
