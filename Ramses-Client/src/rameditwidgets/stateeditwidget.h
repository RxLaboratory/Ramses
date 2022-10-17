#ifndef STATEEDITWIDGET_H
#define STATEEDITWIDGET_H

#include <QColorDialog>

#include "objecteditwidget.h"
#include "duqf-widgets/duqfspinbox.h"
#include "duqf-widgets/duqfcolorselector.h"
#include "ramstate.h"

class StateEditWidget : public ObjectEditWidget
{
    Q_OBJECT

public:
    explicit StateEditWidget(RamState *state, QWidget *parent = nullptr);
    explicit StateEditWidget(QWidget *parent = nullptr);

    RamState *state() const;

protected:
    virtual void reInit(RamObject *o) override;

private slots:
    void setCompletion(int c);
    void setColor(QColor c);

private:
    RamState *m_state;

    void setupUi();
    void connectEvents();

    DuQFColorSelector *ui_colorSelector;
    DuQFSpinBox *ui_completionSpinBox;
};

#endif // STATEEDITWIDGET_H
