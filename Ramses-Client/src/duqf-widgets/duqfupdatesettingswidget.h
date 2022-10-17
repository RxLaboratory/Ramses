#ifndef DUQFUPDATESETTINGSWIDGET_H
#define DUQFUPDATESETTINGSWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QSettings>

class DuQFUpdateSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    DuQFUpdateSettingsWidget(QWidget *parent = nullptr);

private slots:
    void checkAtStartup(bool c);

private:
    void setupUi();
    void connectEvents();

    QSettings m_settings;

    QCheckBox *ui_checkAtStartupBox;
    QPushButton *ui_checkNowButton;
};

#endif // DUQFUPDATESETTINGSWIDGET_H
