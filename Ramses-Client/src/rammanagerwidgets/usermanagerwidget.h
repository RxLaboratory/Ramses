#ifndef USERMANAGERWIDGET_H
#define USERMANAGERWIDGET_H

#include "objectmanagerwidget.h"
#include "ramuser.h"

class UserManagerWidget : public ObjectManagerWidget
{
    Q_OBJECT
public:
    explicit UserManagerWidget(QWidget *parent = nullptr);

protected slots:
    RamUser *createObject() override;
};

#endif // USERMANAGERWIDGET_H
