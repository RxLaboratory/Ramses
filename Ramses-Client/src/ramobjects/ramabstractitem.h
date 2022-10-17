#ifndef RAMABSTRACTITEM_H
#define RAMABSTRACTITEM_H

#include "ramstep.h"

#include "ramstatus.h"
#include "stepstatushistorywidget.h"

class RamState;

/**
 * @brief The RamItem class is the base class for RamShot and RamAsset
 * It handles the status history of the Shot or the Item
 */
class RamAbstractItem : public RamObject
{
    Q_OBJECT
public:
    // OTHER //

    RamAbstractItem(QString shortName, QString name, ObjectType type, RamProject *project );

    virtual RamObject *objectForColumn(QString columnUuid) const;

    RamProject *project() const;
    RamStep::Type productionType() const;

    QMap<QString, RamObjectModel *> statusHistory() const;
    RamObjectModel *statusHistory(RamObject *stepObj) const;

    RamStatus *setStatus(RamUser *user, RamState *state, RamStep *step, int completionRatio = -1, QString comment = "", int version = 1);
    void addStatus(RamStatus *status);

    /**
     * @brief status The latest (current) status for a specific step
     * @param step
     * @return The status
     */
    RamStatus *status(RamStep *step) const;
    /**
     * @brief status All the latest (current) status
     * @return The latest status for each step
     */
    QVector<RamStatus*> status();

    RamUser *assignedUser(RamStep *step);
    bool isUserAssigned(RamObject *u, RamStep *step = nullptr);
    bool isUnassigned(RamStep *step = nullptr);

    bool hasState(RamObject *state, RamStep *step = nullptr);

    void editHistory(RamStep *step);

signals:
    void statusChanged(RamAbstractItem *, RamStep *);

protected:
    RamAbstractItem(QString uuid, ObjectType type);

    static StepStatusHistoryWidget *ui_historyWidget;

private slots:
    void stepInserted(const QModelIndex &parent, int first, int last);
    void stepRemoved(const QModelIndex &parent, int first, int last);

private:
    void construct();
    void connectProject(RamProject *proj);
    void createStepHistory(RamStep *step, QJsonObject d = QJsonObject());
    QMap<QString, RamObjectModel*> m_history;
};

#endif // RAMABSTRACTITEM_H
