#ifndef APPLICATIONEDITWIDGET_H
#define APPLICATIONEDITWIDGET_H

#include <QMenu>
#include <QSplitter>

#include "objecteditwidget.h"
#include "duqf-widgets/duqffolderselectorwidget.h"
#include "objectlistwidget.h"
#include "ramapplication.h"

class ApplicationEditWidget : public ObjectEditWidget
{
    Q_OBJECT

public:
    explicit ApplicationEditWidget(QWidget *parent = nullptr);
    explicit ApplicationEditWidget(RamApplication *app, QWidget *parent = nullptr);

    RamApplication *application() const;

protected:
    virtual void reInit(RamObject *o) override;

private slots:
    void createForNative();
    void createForImport();
    void createForExport();

private:
    RamApplication *m_application;

    void setupUi();
    void connectEvents();

    DuQFFolderSelectorWidget *m_folderSelector;
    ObjectListWidget *m_nativeList;
    ObjectListWidget *m_importList;
    ObjectListWidget *m_exportList;
};

#endif // APPLICATIONEDITWIDGET_H
