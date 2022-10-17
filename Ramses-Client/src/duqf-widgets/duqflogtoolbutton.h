#ifndef DUQFLOGTOOLBUTTON_H
#define DUQFLOGTOOLBUTTON_H

#include <QToolButton>
#include <QDialog>
#include <QVBoxLayout>
#include <QSettings>
#include "duqf-utils/duqflogger.h"
#include "duqf-widgets/duqfloggingtextedit.h"
#include "duqf-app/app-style.h"

class DuQFLogToolButton : public QToolButton
{
    Q_OBJECT
public:
    DuQFLogToolButton(QWidget *parent = nullptr);

private slots:
    void log(DuQFLog m);
    void showLog();
    void clear();
private:
    void setupUi();
    DuQFLog::LogType _currentLevel;
    QDialog *_logDialog;
};

#endif // DUQFLOGTOOLBUTTON_H
