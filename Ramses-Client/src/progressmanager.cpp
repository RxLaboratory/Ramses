#include "progressmanager.h"

ProgressManager* ProgressManager::_instance = nullptr;

ProgressManager::ProgressManager(QObject *parent) : DuQFLoggerObject("Ramses", parent)
{

}

bool ProgressManager::isBusy() const
{
    return m_busy;
}

ProgressManager *ProgressManager::instance()
{
    if (!_instance) _instance = new ProgressManager();
    return _instance;
}

void ProgressManager::setText(const QString &t)
{
    log(m_title + "\n" + t, DuQFLog::Debug);
    emit text(t);
}

void ProgressManager::setTitle(const QString &t)
{
    log(t, DuQFLog::Information);
    m_title = t;
    emit title(t);
}

void ProgressManager::setProgress(const int &p)
{
    m_val = p;
    emit progress(p);
}

void ProgressManager::setMaximum(const int &m)
{
    m_maximum = m;
    emit maximum(m);
}

void ProgressManager::freeze(const bool &f)
{
    reInit();
    if (f) emit started();
    else emit finished();
    emit freezeUI(f);
}

void ProgressManager::increment()
{
    m_busy = true;
    setProgress(m_val + 1);
}

void ProgressManager::addToMaximum(const int &m)
{
    setMaximum(m_maximum + m);
}

void ProgressManager::reInit()
{
    setText("");
    setTitle("");
    setProgress(0);
    setMaximum(0);
    m_busy = false;
}

void ProgressManager::finish()
{
    m_busy = false;
    emit finished();
}

void ProgressManager::start()
{
    reInit();
    m_busy = true;
    emit started();
}
