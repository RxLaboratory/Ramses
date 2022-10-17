#ifndef SHOTMANAGERWIDGET_H
#define SHOTMANAGERWIDGET_H

#include "objectmanagerwidget.h"
#include "ramshot.h"

class ShotManagerWidget : public ObjectManagerWidget
{
    Q_OBJECT
public:
    ShotManagerWidget(QWidget *parent = nullptr);

protected slots:
    RamShot *createObject() override;

private slots:
    void changeProject(RamProject *project);
    void batchCreate();

};

#endif // SHOTMANAGERWIDGET_H
