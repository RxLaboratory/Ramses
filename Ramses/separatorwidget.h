#ifndef SEPARATORWIDGET_H
#define SEPARATORWIDGET_H

#include "ui_separatorwidget.h"

class SeparatorWidget : public QWidget, private Ui::SeparatorWidget
{
    Q_OBJECT

public:
    explicit SeparatorWidget(QWidget *parent = 0);
};

#endif // SEPARATORWIDGET_H
