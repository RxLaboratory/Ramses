#include "daemonsettingswidget.h"

DaemonSettingsWidget::DaemonSettingsWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    portSpinBox->setValue(QSettings().value("daemon/port", 18185).toInt());

    connect(portSpinBox, SIGNAL(valueChanged(int)), this, SLOT(portChanged(int)));
    connect(restartButton, &QPushButton::clicked, this, &DaemonSettingsWidget::restartDaemon);
}

void DaemonSettingsWidget::portChanged(int p)
{
    QSettings().setValue("daemon/port", p);
}

void DaemonSettingsWidget::restartDaemon()
{
    Daemon::instance()->restart();
}
