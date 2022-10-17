#include "duqfsslcheckbox.h"

DuQFSSLCheckbox::DuQFSSLCheckbox(QWidget *parent):
    QCheckBox(parent)
{
    this->setText("Use SSL");
    setupUi();
    connectEvents();
}

DuQFSSLCheckbox::DuQFSSLCheckbox(const QString &text, QWidget *parent):
    QCheckBox(text,parent)
{
    setupUi();
    connectEvents();
}

bool DuQFSSLCheckbox::ssl() const
{
    return this->isChecked();
}

void DuQFSSLCheckbox::setSSL(bool enable)
{
    m_settings.setValue("server/ssl", enable);
    this->setChecked(enable);
}

void DuQFSSLCheckbox::setupUi()
{
    this->setChecked( m_settings.value("server/ssl", true).toBool() );
}

void DuQFSSLCheckbox::connectEvents()
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(setSSL(bool)));
}
