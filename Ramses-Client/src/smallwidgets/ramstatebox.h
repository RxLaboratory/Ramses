#ifndef STATEBOX_H
#define STATEBOX_H

#include <QComboBox>
#include <QPaintEvent>
#include <QStylePainter>

#include "ramobjectcombobox.h"
#include "ramstate.h"

class RamStateBox : public RamObjectComboBox
{
    Q_OBJECT
public:
    RamStateBox(QWidget *parent = nullptr);
    void update();

signals:
    void currentStateChanged(RamState *state);

private slots:
    void changeCurrentState(RamObject *state);

};

#endif // STATEBOX_H
