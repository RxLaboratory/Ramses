#ifndef DATABASEEDITWIDGET_H
#define DATABASEEDITWIDGET_H

#include <QScrollArea>

#include "duqf-widgets/duqffolderdisplaywidget.h"
#include "duqf-widgets/duqffolderselectorwidget.h"
#include "servereditwidget.h"

class DatabaseEditWidget : public QScrollArea
{
    Q_OBJECT
public:
    DatabaseEditWidget(QWidget *parent = nullptr);

    const QString &dbFile() const;
    void setDbFile(const QString &newDbFile);

signals:
    void applied();

private slots:
    void apply();
    void reset();

private:
    void setupUi();
    void connectEvents();

    QString m_dbFile;

    QCheckBox *ui_syncBox;
    ServerEditWidget *ui_serverEdit;
    QPushButton *ui_resetButton;
    QPushButton *ui_applyButton;
    DuQFFolderSelectorWidget *ui_folderSelector;
    DuQFFolderDisplayWidget *ui_folderDisplay;
};

#endif // DATABASEEDITWIDGET_H
