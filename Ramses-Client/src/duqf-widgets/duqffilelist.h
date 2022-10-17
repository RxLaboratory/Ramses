#ifndef DUQFFILELIST_H
#define DUQFFILELIST_H

#include <QTreeWidget>
#include <QWidget>

class DuQFFileList : public QTreeWidget
{
    Q_OBJECT
public:
    DuQFFileList(QWidget *parent = nullptr);

    void setList(QFileInfoList fileList);
    void addFile(QFileInfo file);
    QFileInfo currentFile() const;
    QString currentFilePath() const;
    QString currentFileName() const;
    QFileInfo file(int index) const;
    QString filePath(int index) const;
    QString fileName(int index) const;

    const QString &dateFormat() const;
    void setDateFormat(const QString &newDateFormat);

    bool showDate() const;
    void setShowDate(bool newShowDate);

private:
    QString m_dateFormat = "yyyy-MM-dd hh:mm:ss";
    bool m_showDate = true;
};

#endif // DUQFFILELIST_H
