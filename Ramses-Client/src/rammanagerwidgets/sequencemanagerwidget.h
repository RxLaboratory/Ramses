#ifndef SEQUENCEMANAGERWIDGET_H
#define SEQUENCEMANAGERWIDGET_H

#include "objectmanagerwidget.h"
#include "ramsequence.h"

class SequenceManagerWidget : public ObjectManagerWidget
{
    Q_OBJECT
public:
    SequenceManagerWidget(QWidget *parent = nullptr);

protected slots:
    RamSequence *createObject() override;

private slots:
    void changeProject(RamProject *project);

private:

};

#endif // SEQUENCEMANAGERWIDGET_H
