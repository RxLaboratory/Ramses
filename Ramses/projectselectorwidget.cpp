#include "projectselectorwidget.h"

ProjectSelectorWidget::ProjectSelectorWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    freeze = false;
}

ProjectSelectorWidget::~ProjectSelectorWidget()
{
    delete ui;
}


void ProjectSelectorWidget::addProject(RAMProject *project)
{
    freeze = true;
    int indexBefore = projectsComboBox->currentIndex();

    projectsList << project;
    //add to combo box (check if unique)
    bool ok = true;
    for (int i = 0 ; i < projectsComboBox->count() ; i++)
    {
        if (projectsComboBox->itemData(i).toInt() == project->getId())
        {
            ok = false;
            break;
        }
    }
    if (ok)
    {
        projectsComboBox->addItem(project->getShortName(),project->getId());
        //connect
        connect(project,SIGNAL(projectRemoved(RAMProject*)),this,SLOT(removeProject(RAMProject*)));
        connect(project,SIGNAL(shortNameChanged(RAMProject*,QString)),this,SLOT(projectShortNameChanged(RAMProject*,QString)));
        connect(project,SIGNAL(stageRemoved(RAMProject*,RAMStage*)),this,SLOT(removeStage(RAMProject*,RAMStage*)));
        connect(project,SIGNAL(stageAdded(RAMProject*,RAMStage*)),this,SLOT(addStage(RAMProject*,RAMStage*)));

    }

    freeze = false;
    if (projectsComboBox->currentIndex() != indexBefore) on_projectsComboBox_currentIndexChanged(projectsComboBox->currentIndex());
}

void ProjectSelectorWidget::removeProject(RAMProject *project)
{
    freeze = true;
    int indexBefore = projectsComboBox->currentIndex();

    projectsList.removeAll(project);
    //remove from combobox
    for (int i = 0 ; i < projectsComboBox->count() ; i++)
    {
        if (projectsComboBox->itemData(i).toInt() == project->getId())
        {
            projectsComboBox->removeItem(i);
            break;
        }
    }

    freeze = false;
    if (projectsComboBox->currentIndex() != indexBefore) on_projectsComboBox_currentIndexChanged(projectsComboBox->currentIndex());
}

void ProjectSelectorWidget::clear()
{
    stagesComboBox->clear();
    projectsComboBox->clear();
    projectsList.clear();
}

void ProjectSelectorWidget::removeStage(RAMProject *project,RAMStage *stage)
{
    freeze = true;
    int indexBefore = stagesComboBox->currentIndex();

    if (projectsComboBox->currentIndex() < 0) return;
    //if current
    if (project->getId() == projectsComboBox->currentData().toInt())
    {
        for (int i = 0 ; i < stagesComboBox->count() ; i++)
        {
            if (stagesComboBox->itemData(i).toInt() == stage->getId())
            {
                stagesComboBox->removeItem(i);
                break;
            }
        }
    }

    freeze = false;
    if (stagesComboBox->currentIndex() != indexBefore) on_stagesComboBox_currentIndexChanged(stagesComboBox->currentIndex());
}

void ProjectSelectorWidget::addStage(RAMProject *project,RAMStage *stage)
{
    freeze = true;
    int indexBefore = stagesComboBox->currentIndex();

    if (projectsComboBox->currentIndex() < 0) return;
    //if current
    if (project->getId() == currentProject->getId())
    {
        //if not already added
        bool ok = true;
        for (int i = 0 ; i < stagesComboBox->count() ; i++)
        {
            if (stagesComboBox->itemData(i).toInt() == stage->getId())
            {
                ok = false;
                break;
            }
        }
        if (ok) stagesComboBox->addItem(stage->getShortName(),stage->getId());
    }

    freeze = false;
    if (stagesComboBox->currentIndex() != indexBefore) on_stagesComboBox_currentIndexChanged(stagesComboBox->currentIndex());
}

void ProjectSelectorWidget::projectShortNameChanged(RAMProject *project, QString shortName)
{
    //update name in combo box
    for (int i = 0 ; i < projectsComboBox->count() ; i++)
    {
        if (projectsComboBox->itemData(i).toInt() == project->getId())
        {
            projectsComboBox->setItemText(i,shortName);
            break;
        }
    }
}

void ProjectSelectorWidget::on_projectsComboBox_currentIndexChanged(int index)
{
    if (freeze) return;
    if (index < 0)
    {
        emit 0;
    }
    stagesComboBox->clear();
    //get project
    foreach(RAMProject *project, projectsList)
    {
        if (project->getId() == projectsComboBox->itemData(index).toInt())
        {
            emit currentProjectChanged(project);
            currentProject = project;
            //add stages
            foreach(RAMStage *stage,project->getStages())
            {
                addStage(project,stage);
            }

            break;
        }
    }
}

void ProjectSelectorWidget::on_stagesComboBox_currentIndexChanged(int index)
{
    if (freeze) return;
    //get stage
    foreach(RAMStage *stage, currentProject->getStages())
    {
        if (stage->getId() == stagesComboBox->itemData(index).toInt())
        {
            emit currentStageChanged(stage);
        }
    }
}
