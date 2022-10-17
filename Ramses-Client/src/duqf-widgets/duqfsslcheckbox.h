#ifndef DUQFSSLCHECKBOX_H
#define DUQFSSLCHECKBOX_H

#include <QCheckBox>

class DuQFSSLCheckbox : public QCheckBox
{
    Q_OBJECT
public:
    DuQFSSLCheckbox(QWidget *parent = nullptr);
    DuQFSSLCheckbox(const QString &text, QWidget *parent = nullptr);
    bool ssl() const;

public slots:
    void setSSL(bool enable);

private:
    void setupUi();
    void connectEvents();

    QSettings m_settings;
};

#endif // DUQFSSLCHECKBOX_H
