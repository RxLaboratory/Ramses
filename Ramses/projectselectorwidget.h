#ifndef PROJECTSELECTORWIDGET_H
#define PROJECTSELECTORWIDGET_H

#include "ui_projectselectorwidget.h"
#include <QWidget>
#include <QComboBox>
#include "ramproject.h"
#include "ramstage.h"

class ProjectSelectorWidget : public QWidget, private Ui::ProjectSelectorWidget
{
    Q_OBJECT

public:
    explicit ProjectSelectorWidget(QWidget *parent = 0);
    ~ProjectSelectorWidget();

public slots:
    void addProject(RAMProject *project);
    void removeProject(RAMProject *project);
    void clear();

signals:
    void currentProjectChanged(RAMProject*);
    void currentStageChanged(RAMStage*);

private:
    Ui::ProjectSelectorWidget *ui;
    QList<RAMProject*> projectsList;
    RAMProject *currentProject;
    bool freeze;

private slots:
    void projectShortNameChanged(RAMProject*project, QString shortName);
    void addStage(RAMProject *project, RAMStage *stage);
    void removeStage(RAMProject *project, RAMStage *stage);
    void on_projectsComboBox_currentIndexChanged(int index);
    void on_stagesComboBox_currentIndexChanged(int index);
};

#endif // PROJECTSELECTORWIDGET_H
