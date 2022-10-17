#ifndef PROGRESSMANAGER_H
#define PROGRESSMANAGER_H

#include "duqf-utils/duqflogger.h"

class ProgressManager : public DuQFLoggerObject
{
    Q_OBJECT
public:
    static ProgressManager *instance();
    void setText(const QString &t);
    void setTitle(const QString &t);
    void setProgress(const int &p);
    void setMaximum(const int &m);
    void freeze(const bool &f=true);
    void increment();
    void addToMaximum(const int &m);
    void reInit();
    void finish();
    void start();
    bool isBusy() const;

signals:
    void progress(int);
    void text(QString);
    void title(QString);
    void maximum(int);
    void freezeUI(bool);
    void finished();
    void started();

protected:
    static ProgressManager *_instance;

private:
    explicit ProgressManager(QObject *parent = nullptr);

    int m_val = 0;
    int m_maximum = 0;
    bool m_busy = false;
    QString m_title = "";
};

#endif // PROGRESSMANAGER_H
