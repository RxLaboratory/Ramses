#include "timelinemanager.h"

TimelineManager *TimelineManager::_instance = nullptr;

TimelineManager *TimelineManager::instance()
{
    if(!_instance) _instance = new TimelineManager();
    return _instance;
}

TimelineManager::TimelineManager(QObject *parent)
    : QObject{parent}
{

}

RamShot *TimelineManager::currentShot() const
{
    return m_currentShot;
}

void TimelineManager::setCurrentShot(RamShot *newCurrentShot)
{
    if (m_currentShot == newCurrentShot) return;
    m_currentShot = newCurrentShot;
    emit currentShotChanged(m_currentShot);
}
