#ifndef DUQFFOLDERSELECTORWIDGET_H
#define DUQFFOLDERSELECTORWIDGET_H

#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>

class DuQFFolderSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    enum SelectorType { Folder, File };
    enum SelectorMode {  Save, Open };

    explicit DuQFFolderSelectorWidget(SelectorType type = Folder, QWidget *parent = nullptr);
    QString path();
    void setPath(QString p);
    void setPlaceHolderText(QString t);
    void setDialogTitle(QString t);
    void showDeleteButton(QString trashFolder, bool show = true);
    void showRevealButton(bool show = true);
    void setMode(SelectorMode newMode);
    void setFilter(const QString &filter);

signals:
    void pathChanged(QString);
    void pathChanging(QString);
    void fileRemoved();
private slots:
    void browseButton_clicked();
    void exploreButton_clicked();
    void folderEdit_editingFinished();
    void deleteButton_clicked();
private:
    QString m_dialogTitle;
    void setupUi();

    QLineEdit *ui_folderEdit;
    QToolButton *ui_browseButton;
    QToolButton *ui_exploreButton;
    QToolButton *ui_deleteButton;

    SelectorType m_type = Folder;
    SelectorMode m_mode = Open;
    QString m_filter = "All files (*.*)";
    // a path, relative to the selected folder/file, to use as a trash before the system trash
    QString m_trashFolder = "";
};

#endif // DUQFFOLDERSELECTORWIDGET_H
