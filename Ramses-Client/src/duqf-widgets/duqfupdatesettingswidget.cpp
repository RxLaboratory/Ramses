#include "duqfupdatesettingswidget.h"

#include "duqf-app/app-utils.h"

DuQFUpdateSettingsWidget::DuQFUpdateSettingsWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi();

    m_settings.beginGroup("updates");

    ui_checkAtStartupBox->setChecked(m_settings.value("checkUpdateAtStartup",true).toBool());

    connectEvents();
}

void DuQFUpdateSettingsWidget::checkAtStartup(bool c)
{
    m_settings.setValue("checkUpdateAtStartup", c);
}

void DuQFUpdateSettingsWidget::setupUi()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    mainLayout->addStretch();

    QVBoxLayout *vLayout = new QVBoxLayout();
    mainLayout->addLayout(vLayout);
    vLayout->setSpacing(0);
    vLayout->addStretch();

    QFormLayout *formLayout = new QFormLayout();
    vLayout->addLayout(formLayout);
    formLayout->setHorizontalSpacing(3);
    formLayout->setVerticalSpacing(3);
    formLayout->setContentsMargins(0,0,0,0);

    ui_checkAtStartupBox = new QCheckBox("Check during startup", this);
    formLayout->addRow("Updates", ui_checkAtStartupBox);

    ui_checkNowButton = new QPushButton("Check now", this);
    ui_checkNowButton->setIcon(QIcon(":/icons/check-update"));
    vLayout->addWidget(ui_checkNowButton);

    vLayout->addStretch();
    mainLayout->addStretch();
}

void DuQFUpdateSettingsWidget::connectEvents()
{
    DuApplication *app = qobject_cast<DuApplication*>(qApp);

    connect( ui_checkAtStartupBox, SIGNAL(clicked(bool)), this, SLOT(checkAtStartup(bool)));
    connect( ui_checkNowButton, SIGNAL(clicked()), app, SLOT(checkUpdate()));
}
