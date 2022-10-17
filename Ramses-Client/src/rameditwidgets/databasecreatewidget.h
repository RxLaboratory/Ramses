#ifndef DATABASECREATEWIDGET_H
#define DATABASECREATEWIDGET_H

#include <QScrollArea>

#include "duqf-widgets/duqffolderselectorwidget.h"
#include "localdatainterface.h"
#include "servereditwidget.h"

class DatabaseCreateWidget : public QScrollArea
{
    Q_OBJECT
public:
    DatabaseCreateWidget(QWidget *parent = nullptr);

private slots:
    void checkPath(QString p);
    void createDB();

private:
    void setupUi();
    void connectEvents();

    bool createNewDB();
    bool createNewDB(ServerConfig s);

    DuQFFolderSelectorWidget *ui_fileSelector;
    DuQFFolderSelectorWidget *ui_folderSelector;
    QTabWidget *ui_tabWidget;
    QLineEdit *ui_shortNameEdit;
    QLineEdit *ui_nameEdit;
    ServerEditWidget *ui_serverEdit;
    QPushButton *ui_createButton;
};

#endif // DATABASECREATEWIDGET_H
