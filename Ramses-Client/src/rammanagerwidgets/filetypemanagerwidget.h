#ifndef FILETYPEMANAGERWIDGET_H
#define FILETYPEMANAGERWIDGET_H

#include "objectmanagerwidget.h"
#include "ramfiletype.h"

class FileTypeManagerWidget : public ObjectManagerWidget
{
    Q_OBJECT
public:
    FileTypeManagerWidget(QWidget *parent = nullptr);

protected slots:
    RamFileType *createObject() override;
};

#endif // FILETYPEMANAGERWIDGET_H
