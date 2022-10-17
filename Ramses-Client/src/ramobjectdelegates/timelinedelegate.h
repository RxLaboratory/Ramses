#ifndef TIMELINEDELEGATE_H
#define TIMELINEDELEGATE_H

#include "ramobjectdelegate.h"

/**
 * @brief The TimelineDelegate class paints the timeline (shots in a row)
 */
class TimelineDelegate : public RamObjectDelegate
{
public:
    explicit TimelineDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};

#endif // TIMELINEDELEGATE_H
