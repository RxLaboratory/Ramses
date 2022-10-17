#ifndef PIPEFILEMANAGERWIDGET_H
#define PIPEFILEMANAGERWIDGET_H

#include "objectmanagerwidget.h"
#include "rampipefile.h"

class PipeFileManagerWidget : public ObjectManagerWidget
{
    Q_OBJECT
public:
    PipeFileManagerWidget(QWidget *parent = nullptr);

protected slots:
    RamPipeFile *createObject() override;

private slots:
    void changeProject(RamProject *project);

private:
};

#endif // PIPEFILEMANAGERWIDGET_H
