#include "duqffilelist.h"

DuQFFileList::DuQFFileList(QWidget *parent):
    QTreeWidget(parent)
{
    this->setColumnCount(2);
    this->setHeaderLabels(QStringList() << "Name" << "Date");
}

void DuQFFileList::setList(QFileInfoList fileList)
{
    this->clear();
    foreach(QFileInfo file, fileList)
    {
        addFile(file);
    }
    this->header()->resizeSections(QHeaderView::ResizeToContents);
}

void DuQFFileList::addFile(QFileInfo file)
{
    // TODO if folder: show as parent with files inside
    QStringList label;
    label << file.fileName() << file.lastModified().toString(m_dateFormat);
    QTreeWidgetItem *item = new QTreeWidgetItem(label);
    item->setData(0, Qt::UserRole, file.absoluteFilePath() );
    item->setData(0, Qt::UserRole + 1, file.fileName() );
    if (file.isSymLink()) item->setIcon(0, QIcon(":/icons/shortcut"));
    else if (file.isFile()) item->setIcon(0, QIcon(":icons/file"));
    else if (file.isDir()) item->setIcon(0, QIcon(":icons/folder"));
    this->addTopLevelItem(item);
}

QFileInfo DuQFFileList::currentFile() const
{
    return QFileInfo( currentFilePath() );
}

QString DuQFFileList::currentFilePath() const
{
    return this->currentItem()->data(0, Qt::UserRole).toString();
}

QString DuQFFileList::currentFileName() const
{
    return this->currentItem()->data(0, Qt::UserRole + 1).toString();
}

QFileInfo DuQFFileList::file(int index) const
{
    return QFileInfo(filePath(index));
}

QString DuQFFileList::filePath(int index) const
{
    return this->topLevelItem(index)->data(0, Qt::UserRole).toString();
}

QString DuQFFileList::fileName(int index) const
{
    return this->topLevelItem(index)->data(0, Qt::UserRole + 1).toString();
}

const QString &DuQFFileList::dateFormat() const
{
    return m_dateFormat;
}

void DuQFFileList::setDateFormat(const QString &newDateFormat)
{
    m_dateFormat = newDateFormat;
}

bool DuQFFileList::showDate() const
{
    return m_showDate;
}

void DuQFFileList::setShowDate(bool newShowDate)
{
    m_showDate = newShowDate;
}
