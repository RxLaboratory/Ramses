#ifndef PROJECTEDITWIDGET_H
#define PROJECTEDITWIDGET_H

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateTimeEdit>

#include "objecteditwidget.h"
#include "resolutionwidget.h"
#include "frameratewidget.h"
#include "duqf-widgets/duqffolderselectorwidget.h"
#include "objectlistwidget.h"
#include "ramproject.h"

class ProjectEditWidget : public ObjectEditWidget
{
    Q_OBJECT

public:
    explicit ProjectEditWidget(QWidget *parent = nullptr);
    explicit ProjectEditWidget(RamProject *project, QWidget *parent = nullptr);

    RamProject *project() const;

protected:
    virtual void reInit(RamObject *o) override;

private slots:
    void setResolution(int w, int h);
    void setFramerate(double f);
    void updateFolderLabel(QString path);
    void setPath(QString p);
    void setDeadline(QDate d);
    void currentUserChanged(RamUser *user);
    void createUser();

    void savePath();
    void reinitPath();

private:
    RamProject *m_project;

    void setupUi();
    void connectEvents();

    DuQFFolderSelectorWidget *ui_folderSelector;
    QLabel *ui_folderLabel;
    ResolutionWidget *ui_resolutionWidget;
    FramerateWidget *ui_framerateWidget;
    QDateEdit *ui_deadlineEdit;
    QToolButton *ui_savePathButton;
    QToolButton *ui_reinitPathButton;
    ObjectListWidget *ui_userList;
};

#endif // PROJECTEDITWIDGET_H
