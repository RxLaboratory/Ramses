#ifndef DUQFAUTOSIZETOOLBUTTON_H
#define DUQFAUTOSIZETOOLBUTTON_H

#include <QToolButton>
#include <QFontMetrics>

class DuQFAutoSizeToolButton : public QToolButton
{
public:
    DuQFAutoSizeToolButton(QWidget *parent = nullptr);

    void setText(const QString &text);

    int margins() const;
    void setMargins(int newMargins);

private:
    int m_margins = 10;
};

#endif // DUQFAUTOSIZETOOLBUTTON_H
