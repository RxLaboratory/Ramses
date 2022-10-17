#include "filetypemanagerwidget.h"

#include "ramses.h"

FileTypeManagerWidget::FileTypeManagerWidget(QWidget *parent) :
    ObjectManagerWidget(
        Ramses::instance()->fileTypes(),
        "File types",
        QIcon(":icons/file"),
        parent )
{
    m_listWidget->setEditMode(ObjectListWidget::RemoveObjects);
    m_listWidget->setSortMode(RamAbstractObject::ShortName);
    m_listWidget->sort();
}

RamFileType *FileTypeManagerWidget::createObject()
{
    RamFileType *ft = new RamFileType(
                "NEW",
                "New file type");
    //Ramses::instance()->fileTypes()->append(ft);
    ft->edit();
    return ft;
}

