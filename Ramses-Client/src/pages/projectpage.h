#ifndef PROJECTPAGE_H
#define PROJECTPAGE_H

#include "duqf-widgets/settingswidget.h"
#include "projecteditwidget.h"
#include "ramobjectmenu.h"

class ProjectPage : public SettingsWidget
{
    Q_OBJECT
public:
    ProjectPage(QWidget *parent = nullptr);
private slots:
    void currentProjectChanged(RamProject *project);

    void assignUser(RamObject *user);
    void unAssignUser(RamObject *user);
    void userAssigned(const QModelIndex &parent,int first,int last);
    void userUnassigned(const QModelIndex &parent,int first,int last);

    void createStepFromTemplate(RamObject *templateStepObj);
    void createAssetGroupFromTemplate(RamObject *templateAGObj);

    void createShots();

private:
    ProjectEditWidget *ui_currentProjectSettings;

    QList<QMetaObject::Connection> m_userConnections;

    RamObjectMenu *ui_assignUserMenu;
    RamObjectMenu *ui_unAssignUserMenu;
};

#endif // PROJECTPAGE_H
