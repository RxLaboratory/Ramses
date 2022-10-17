#ifndef AUTOSELECTDOUBLESPINBOX_H
#define AUTOSELECTDOUBLESPINBOX_H

#include <QDoubleSpinBox>

#include "duqf-widgets/autoselectlineedit.h"

class AutoSelectDoubleSpinBox : public QDoubleSpinBox
{
public:
    AutoSelectDoubleSpinBox(QWidget *parent = nullptr);
};

#endif // AUTOSELECTDOUBLESPINBOX_H
