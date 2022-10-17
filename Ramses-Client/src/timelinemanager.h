#ifndef TIMELINEMANAGER_H
#define TIMELINEMANAGER_H

#include <QObject>

#include "ramshot.h"

class TimelineManager : public QObject
{
    Q_OBJECT
public:
    static TimelineManager *instance();

    RamShot *currentShot() const;

public slots:
    void setCurrentShot(RamShot *newCurrentShot);

signals:
    void currentShotChanged(RamShot*);

protected:
    static TimelineManager *_instance;

private:
    /**
     * @brief TimelineManager is a singleton
     * @param parent
     */
    explicit TimelineManager(QObject *parent = nullptr);

    RamShot *m_currentShot = nullptr;

};

#endif // TIMELINEMANAGER_H
