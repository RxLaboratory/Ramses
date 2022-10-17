#ifndef DAEMONSETTINGSWIDGET_H
#define DAEMONSETTINGSWIDGET_H

#include <QSettings>

#include "ui_daemonsettingswidget.h"

#include "daemon.h"

class DaemonSettingsWidget : public QWidget, private Ui::DaemonSettingsWidget
{
    Q_OBJECT

public:
    explicit DaemonSettingsWidget(QWidget *parent = nullptr);

private slots:
    void portChanged(int p);
    void restartDaemon();
};

#endif // DAEMONSETTINGSWIDGET_H
