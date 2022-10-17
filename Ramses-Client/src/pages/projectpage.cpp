#include "projectpage.h"

#include "assetgroupmanagerwidget.h"
#include "assetmanagerwidget.h"
#include "pipefilemanagerwidget.h"
#include "ramassetgroup.h"
#include "ramses.h"
#include "sequencemanagerwidget.h"
#include "shotmanagerwidget.h"
#include "shotscreationdialog.h"
#include "stepmanagerwidget.h"

ProjectPage::ProjectPage(QWidget *parent):
    SettingsWidget("Project Administration", parent)
{
    this->showReinitButton(false);
    this->titleBar()->setObjectName("projectToolBar");

    qDebug() << "> Project";
    // A better layout for project settings
    QWidget *pSettingsWidget = new QWidget(this);
    QHBoxLayout *pSettingsLayout = new QHBoxLayout(pSettingsWidget);
    pSettingsLayout->setContentsMargins(3,3,3,3);
    pSettingsLayout->addStretch();
    ui_currentProjectSettings = new ProjectEditWidget(this);
    pSettingsLayout->addWidget(ui_currentProjectSettings);
    pSettingsLayout->addStretch();
    pSettingsLayout->setStretch(0, 20);
    pSettingsLayout->setStretch(1, 80);
    pSettingsLayout->setStretch(2, 20);
    this->addPage( pSettingsWidget, "Settings", QIcon(":/icons/projects"));

    QMenu *projectMenu = new QMenu();
    QToolButton *projectButton = new QToolButton();
    projectButton->setIcon(QIcon(":icons/project"));
    projectButton->setText(" Project");
    projectButton->setMenu(projectMenu);
    projectButton->setIconSize(QSize(16,16));
    projectButton->setObjectName("menuButton");
    projectButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    projectButton->setPopupMode(QToolButton::InstantPopup);
    this->titleBar()->insertLeft(projectButton);
    ui_assignUserMenu = new RamObjectMenu(false, this);
    ui_assignUserMenu->setTitle("Assign user");
    ui_assignUserMenu->setObjectModel(Ramses::instance()->users());
    projectMenu->addMenu(ui_assignUserMenu);
    ui_unAssignUserMenu = new RamObjectMenu(false, this);
    ui_unAssignUserMenu->setTitle("Unassign user");
    projectMenu->addMenu(ui_unAssignUserMenu);

    qDebug() << "  > project settings ok";

    StepManagerWidget *stepManager = new StepManagerWidget(this);
    this->addPage(stepManager, "Steps", QIcon(":/icons/steps"));
    this->titleBar()->insertLeft(stepManager->menuButton());

    // Create step from template menu
    RamObjectMenu *stepTemplateMenu = new RamObjectMenu(false, this);
    stepTemplateMenu->setTitle("Create from template...");
    stepTemplateMenu->setObjectModel( Ramses::instance()->templateSteps() );
    stepManager->menuButton()->menu()->addMenu(stepTemplateMenu);

    qDebug() << "  > steps ok";

    PipeFileManagerWidget *pipeFileManager = new PipeFileManagerWidget(this);
    this->addPage(pipeFileManager, "Pipe Types", QIcon(":/icons/pipe-files"));
    this->titleBar()->insertLeft(pipeFileManager->menuButton());
    qDebug() << "  > pipe types ok";

    AssetGroupManagerWidget *assetGroupManager = new AssetGroupManagerWidget(this);
    this->addPage(assetGroupManager, "Asset Groups", QIcon(":/icons/asset-groups"));
    this->titleBar()->insertLeft(assetGroupManager->menuButton());

    RamObjectMenu *agTemplateMenu = new RamObjectMenu(false, this);
    agTemplateMenu->setTitle("Create from template...");
    agTemplateMenu->setObjectModel( Ramses::instance()->templateAssetGroups() );
    assetGroupManager->menuButton()->menu()->addMenu(agTemplateMenu);
    qDebug() << "  > asset groups ok";

    AssetManagerWidget *assetManager = new AssetManagerWidget(this);
    this->addPage(assetManager, "Assets", QIcon(":/icons/assets"));
    this->titleBar()->insertLeft(assetManager->menuButton());
    qDebug() << "  > assets ok";

    SequenceManagerWidget *sequenceManager = new SequenceManagerWidget(this);
    this->addPage(sequenceManager, "Sequences", QIcon(":/icons/sequences"));
    this->titleBar()->insertLeft(sequenceManager->menuButton());
    qDebug() << "  > sequences ok";

    ShotManagerWidget *shotManager = new ShotManagerWidget(this);
    this->addPage(shotManager, "Shots", QIcon(":/icons/shots"));
    this->titleBar()->insertLeft(shotManager->menuButton());

    // Create multiple shots
    QAction *createMultipleShotsAction = new QAction("Create multiple shots...", this);
    shotManager->menuButton()->menu()->addAction(createMultipleShotsAction);
    qDebug() << "  > shots ok";

    connect(Ramses::instance(), SIGNAL(currentProjectChanged(RamProject*)), this, SLOT(currentProjectChanged(RamProject*)));
    connect(ui_assignUserMenu, SIGNAL(assigned(RamObject*)), this, SLOT(assignUser(RamObject*)));
    connect(ui_unAssignUserMenu, SIGNAL(assigned(RamObject*)), this, SLOT(unAssignUser(RamObject*)));
    connect(stepTemplateMenu, SIGNAL(assigned(RamObject*)), this, SLOT(createStepFromTemplate(RamObject*)));
    connect(agTemplateMenu, SIGNAL(assigned(RamObject*)), this, SLOT(createAssetGroupFromTemplate(RamObject*)));
    connect(createMultipleShotsAction, SIGNAL(triggered()), this, SLOT(createShots()));
}

void ProjectPage::currentProjectChanged(RamProject *project)
{
    while(!m_userConnections.isEmpty()) disconnect(m_userConnections.takeLast());

    ui_currentProjectSettings->setObject(project);
    if(project)
    {
        ui_unAssignUserMenu->setObjectModel(project->users());
        // hide already assigned
        if (project->users()->rowCount())
            userAssigned(QModelIndex(), 0, project->users()->rowCount() - 1);
        m_userConnections << connect(project->users(), SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(userAssigned(QModelIndex,int,int)));
        m_userConnections << connect(project->users(), SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(userUnassigned(QModelIndex,int,int)));
    }
    else
        ui_unAssignUserMenu->setObjectModel(nullptr);
}

void ProjectPage::assignUser(RamObject *user)
{
    RamProject *proj = Ramses::instance()->currentProject();
    if (!proj) return;
    proj->users()->appendObject( user->uuid() );
}

void ProjectPage::unAssignUser(RamObject *user)
{
    RamProject *proj = Ramses::instance()->currentProject();
    if (!proj) return;
    proj->users()->removeObjects(QStringList(user->uuid()));
}

void ProjectPage::userAssigned(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    RamProject *proj = Ramses::instance()->currentProject();
    if (!proj) return;

    for (int i = first ; i <= last; i++)
    {
        RamObject *user = proj->users()->get(i);
        ui_assignUserMenu->setObjectVisible(user, false);
    }
}

void ProjectPage::userUnassigned(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    RamProject *proj = Ramses::instance()->currentProject();
    if (!proj) return;

    for (int i = first ; i <= last; i++)
    {
        RamObject *user = proj->users()->get(i);
        ui_assignUserMenu->setObjectVisible(user, true);
    }
}

void ProjectPage::createStepFromTemplate(RamObject *templateStepObj)
{
    RamTemplateStep *templateStep = RamTemplateStep::c(templateStepObj);
    if (!templateStep) return;

    RamProject *project = Ramses::instance()->currentProject();
    if (!project) return;

    RamStep *step = RamStep::createFromTemplate(templateStep, project);
    project->steps()->appendObject(step->uuid());
    step->edit();
}

void ProjectPage::createAssetGroupFromTemplate(RamObject *templateAGObj)
{
    RamTemplateAssetGroup *templateAG = RamTemplateAssetGroup::c(templateAGObj);
    if (!templateAG) return;

    RamProject *project = Ramses::instance()->currentProject();
    if (!project) return;

    RamAssetGroup *ag = RamAssetGroup::createFromTemplate(templateAG, project);
    project->assetGroups()->appendObject(ag->uuid());
    ag->edit();
}

void ProjectPage::createShots()
{
    RamProject *proj = Ramses::instance()->currentProject();
    if (!proj) return;

    ShotsCreationDialog dialog(proj, this);
    dialog.exec();
}
