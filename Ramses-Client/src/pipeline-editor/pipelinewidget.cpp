#include "pipelinewidget.h"

#include "duqf-utils/guiutils.h"

#include "progressmanager.h"
#include "stepnode.h"
#include "ramses.h"
#include "rampipe.h"

PipelineWidget::PipelineWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    // Get the mainwindow to add the titlebar
    QMainWindow *mw = GuiUtils::appMainWindow();
    mw->addToolBarBreak(Qt::TopToolBarArea);

    ui_titleBar = new DuQFTitleBar("Pipeline Editor",false, mw);
    ui_titleBar->setObjectName("pipelineToolBar");
    ui_titleBar->showReinitButton(false);
    mw->addToolBar(Qt::TopToolBarArea,ui_titleBar);
    ui_titleBar->setFloatable(false);
    ui_titleBar->hide();

    // View menu
    QMenu *viewMenu = new QMenu(this);

    QAction *actionReinitView = new QAction("Center view", this);
    actionReinitView->setShortcut(QKeySequence("Home"));
    viewMenu->addAction(actionReinitView);

    QAction *actionViewAll = new QAction("Center selection", this);
    actionViewAll->setShortcut(QKeySequence("F"));
    viewMenu->addAction(actionViewAll);

    QToolButton *viewButton = new QToolButton(this);
    viewButton->setText("View");
    viewButton->setIcon(QIcon(":/icons/view"));
    viewButton->setIconSize(QSize(16,16));
    viewButton->setObjectName("menuButton");
    viewButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    viewButton->setPopupMode(QToolButton::InstantPopup);
    viewButton->setMenu(viewMenu);

    ui_titleBar->insertLeft(viewButton);

    // Select menu
    QMenu *selectMenu = new QMenu(this);

    QAction *actionSelectAll = new QAction("Select all steps", this);
    actionSelectAll->setShortcut(QKeySequence("A"));
    selectMenu->addAction(actionSelectAll);

    QAction *actionSelectChildren = new QAction("Select children steps", this);
    actionSelectChildren->setShortcut(QKeySequence("Ctrl+A"));
    selectMenu->addAction(actionSelectChildren);

    QAction *actionSelectParents = new QAction("Select parent steps", this);
    actionSelectParents->setShortcut(QKeySequence("Alt+A"));
    selectMenu->addAction(actionSelectParents);

    QToolButton *selectButton = new QToolButton(this);
    selectButton->setText("Select");
    selectButton->setIcon(QIcon(":/icons/select-menu"));
    selectButton->setIconSize(QSize(16,16));
    selectButton->setObjectName("menuButton");
    selectButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    selectButton->setPopupMode(QToolButton::InstantPopup);
    selectButton->setMenu(selectMenu);

    ui_titleBar->insertLeft(selectButton);

    // Layout menu
    QMenu *layMenu = new QMenu(this);

    QAction *actionLayoutAll = new QAction("Layout all steps", this);
    actionLayoutAll->setShortcut(QKeySequence("Shift+L"));
    layMenu->addAction(actionLayoutAll);

    QAction *actionLayoutSelected = new QAction("Layout selected steps", this);
    actionLayoutSelected->setShortcut(QKeySequence("Alt+L"));
    layMenu->addAction(actionLayoutSelected);

    QToolButton *layButton = new QToolButton(this);
    layButton->setText("Layout");
    layButton->setIcon(QIcon(":/icons/steps-menu"));
    layButton->setIconSize(QSize(16,16));
    layButton->setObjectName("menuButton");
    layButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    layButton->setPopupMode(QToolButton::InstantPopup);
    layButton->setMenu(layMenu);

    layMenu->addSeparator();

    QAction *actionLayoutLoad = new QAction("Reload project layout", this);
    actionLayoutLoad->setShortcut(QKeySequence("Shift+R"));
    layMenu->addAction(actionLayoutLoad);

    QAction *actionLayoutSave = new QAction("Save project layout", this);
    actionLayoutSave->setShortcut(QKeySequence("Shift+S"));
    layMenu->addAction(actionLayoutSave);

    ui_titleBar->insertLeft(layButton);

    // Step menu
    ui_stepMenu = new QMenu(this);

    QAction *actionAddStep = new QAction("Create new step", this);
    actionAddStep->setShortcut(QKeySequence("Shift+A"));
    ui_stepMenu->addAction(actionAddStep);
    ui_stepMenu->addSeparator();
    ui_stepMenuSeparator = ui_stepMenu->addSeparator();

    QAction *actionDeleteStep = new QAction("Remove selected steps", this);
    actionDeleteStep->setShortcut(QKeySequence("Shift+X"));
    ui_stepMenu->addAction(actionDeleteStep);

    QAction *actionDeleteSelection = new QAction("Delete selection", this);
    actionDeleteSelection->setShortcut(QKeySequence("Delete"));
    ui_stepMenu->addAction(actionDeleteSelection);

    QToolButton *stepButton = new QToolButton(this);
    stepButton->setText("Step");
    stepButton->setIcon(QIcon(":/icons/step"));
    stepButton->setIconSize(QSize(16,16));
    stepButton->setObjectName("menuButton");
    stepButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    stepButton->setPopupMode(QToolButton::InstantPopup);
    stepButton->setMenu(ui_stepMenu);

    ui_titleBar->insertLeft(stepButton);

    // Connections menu
    QMenu *coMenu = new QMenu(this);

    QAction *actionDeleteConnections = new QAction("Remove selected pipes", this);
    actionDeleteConnections->setShortcut(QKeySequence("Alt+X"));
    coMenu->addAction(actionDeleteConnections);

    QToolButton *coButton = new QToolButton(this);
    coButton->setText("Pipe");
    coButton->setIcon(QIcon(":/icons/connection"));
    coButton->setIconSize(QSize(16,16));
    coButton->setObjectName("menuButton");
    coButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    coButton->setPopupMode(QToolButton::InstantPopup);
    coButton->setMenu(coMenu);

    ui_titleBar->insertLeft(coButton);

    // Right buttons

    QToolButton *viewAllButton = new QToolButton(this);
    viewAllButton->setIcon(QIcon(":/icons/view-all"));
    ui_titleBar->insertRight(viewAllButton);
    QToolButton *viewSelectedButton = new QToolButton(this);
    viewSelectedButton->setIcon(QIcon(":/icons/view-selected"));
    ui_titleBar->insertRight(viewSelectedButton);

    DuQFSpinBox *zoomBox = new DuQFSpinBox(this);
    zoomBox->setMinimum(25);
    zoomBox->setMaximum(400);
    zoomBox->setSuffix("%");
    zoomBox->setPrefix("Zoom: ");
    zoomBox->setMaximumWidth(100);
    zoomBox->setValue(100);
    ui_titleBar->insertRight(zoomBox);

    ui_gridSizeBox = new DuQFSpinBox(this);
    ui_gridSizeBox->setMinimum(10);
    ui_gridSizeBox->setMaximum(100);
    ui_gridSizeBox->setMaximumWidth(100);
    ui_gridSizeBox->setValue(20);
    ui_gridSizeBox->setPrefix("Grid size: ");
    ui_titleBar->insertRight(ui_gridSizeBox);

    ui_snapButton = new QToolButton();
    ui_snapButton->setCheckable(true);
    ui_snapButton->setIcon(QIcon(":/icons/snap"));
    ui_titleBar->insertRight(ui_snapButton);

    ui_nodeView = new DuQFNodeView(this);
    m_nodeScene = ui_nodeView->nodeScene();
    mainLayout->addWidget(ui_nodeView);

    // Connections
    connect(ui_titleBar, &DuQFTitleBar::closeRequested, this, &PipelineWidget::closeRequested);
    connect(viewAllButton, SIGNAL(clicked()), ui_nodeView, SLOT(reinitTransform()));
    connect(actionReinitView, SIGNAL(triggered()), ui_nodeView, SLOT(reinitTransform()));
    connect(viewSelectedButton, SIGNAL(clicked()), ui_nodeView, SLOT(frameSelected()));
    connect(actionViewAll, SIGNAL(triggered()), ui_nodeView, SLOT(frameSelected()));
    connect(zoomBox, SIGNAL(valueChanged(int)), ui_nodeView, SLOT(setZoom(int)));
    connect(ui_nodeView, SIGNAL(scaled(int)), zoomBox, SLOT(setValue(int)));
    connect(actionAddStep, SIGNAL(triggered()), this, SLOT(createStep()));
    connect(actionAddStep, SIGNAL(triggered()), ui_nodeView, SLOT(reinitTransform()));
    connect(actionDeleteStep, SIGNAL(triggered()), m_nodeScene, SLOT(removeSelectedNodes()));
    connect(actionDeleteConnections, SIGNAL(triggered()), m_nodeScene, SLOT(removeSelectedConnections()));
    connect(actionDeleteSelection, SIGNAL(triggered()), m_nodeScene, SLOT(removeSelection()));
    connect(ui_snapButton, SIGNAL(clicked(bool)), this, SLOT(setSnapEnabled(bool)));
    connect(ui_gridSizeBox, SIGNAL(valueChanged(int)), this, SLOT(setGridSize(int)));
    connect(actionLayoutAll, SIGNAL(triggered()), m_nodeScene, SLOT(autoLayoutAll()));
    connect(actionLayoutAll, SIGNAL(triggered()), ui_nodeView, SLOT(frameSelected()));
    connect(actionLayoutSelected, SIGNAL(triggered()), m_nodeScene, SLOT(autoLayoutSelectedNodes()));
    connect(actionLayoutLoad, SIGNAL(triggered()), this, SLOT(loadProjectLayout()));
    connect(actionLayoutSave, SIGNAL(triggered()), this, SLOT(saveProjectLayout()));
    connect(actionSelectAll, SIGNAL(triggered()), m_nodeScene, SLOT(selectAllNodes()));
    connect(actionSelectChildren, SIGNAL(triggered()), m_nodeScene, SLOT(selectChildNodes()));
    connect(actionSelectParents, SIGNAL(triggered()), m_nodeScene, SLOT(selectParentNodes()));
    connect(m_nodeScene->connectionManager(), SIGNAL(newConnection(DuQFConnection*)), this, SLOT(stepsConnected(DuQFConnection*)));
    connect(m_nodeScene->connectionManager(), SIGNAL(connectionRemoved(DuQFConnection*)), this, SLOT(connectionRemoved(DuQFConnection*)));
    // Ramses connections
    connect(Ramses::instance(), SIGNAL(currentProjectChanged(RamProject*)), this, SLOT(setProject(RamProject*)));
    connect(Ramses::instance(), &Ramses::userChanged, this, &PipelineWidget::userChanged);
}

void PipelineWidget::setProject(RamProject *project)
{
    if (m_project && project)
    {
        if (m_project->is(project)) return;
    }
    else if (!m_project && !project) return;

    m_projectChanged = true;

    // Disconnect
    if (m_project)
    {
        disconnect(m_project->steps(), nullptr, this, nullptr);
        disconnect(m_project->pipeline(), nullptr, this, nullptr);
    }

    // Recenter view
    ui_nodeView->reinitTransform();

    m_project = project;
    // Reload in the show event if not yet visible
    // to improve perf: do not refresh all the app when changing the project, only what's visible.
    if ( this->isVisible() ) changeProject();
}

void PipelineWidget::newStep(RamObject *obj)
{
    RamStep *step = qobject_cast<RamStep*>( obj );
    StepNode *stepNode = new StepNode(step);
    m_nodeScene->addNode( stepNode, false );

    // Reset position
    RamUser *u = Ramses::instance()->currentUser();
    bool ok = false;
    if (u)
    {
        QSettings *uSettings = u->settings();
        uSettings->beginGroup("nodeView");
        uSettings->beginGroup("nodeLocations");
        QPointF pos = uSettings->value(step->uuid(), QPointF(0.0,0.0)).toPointF();
        if (pos.x() != 0.0 && pos.y() != 0.0)
        {
            stepNode->setPos( pos );
            ok = true;
        }
        uSettings->endGroup();
        uSettings->endGroup();
    }
    // not found, try from project
    if (!ok)
    {
        QSettings *pSettings = m_project->settings();
        pSettings->beginGroup("nodeView");
        pSettings->beginGroup("nodeLocations");
        QPointF pos = pSettings->value(step->uuid(), QPointF(0.0,0.0)).toPointF();
        if (pos.x() != 0.0 && pos.y() != 0.0)
        {
            stepNode->setPos( pos );
        }
        pSettings->endGroup();
        pSettings->endGroup();
    }

    connect(stepNode, &DuQFNode::moved, this, &PipelineWidget::nodeMoved);
}

void PipelineWidget::newStep(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    for (int i = first; i <= last; i++)
    {
        RamObject *o = m_project->steps()->get(i);
        newStep(o);
    }
}

void PipelineWidget::nodeMoved(QPointF pos)
{
    ObjectNode *node = (ObjectNode*)sender();
    if (!node) return;
    RamObject *step = node->ramObject();

    // Save node location to user settings
    RamUser *u = Ramses::instance()->currentUser();
    if (!u) return;
    QSettings *uSettings = u->settings();
    uSettings->beginGroup("nodeView");
    uSettings->beginGroup("nodeLocations");
    uSettings->setValue(step->uuid(), pos);
    uSettings->endGroup();
    uSettings->endGroup();

    // Save node location to project settings if not already there
    QSettings *pSettings = m_project->settings();
    pSettings->beginGroup("nodeView");
    pSettings->beginGroup("nodeLocations");
    if (pSettings->value(step->uuid(), QPointF()).toPointF() == QPointF())
    {
        pSettings->setValue(step->uuid(), pos);
    }
    pSettings->endGroup();
    pSettings->endGroup();
}

void PipelineWidget::setSnapEnabled(bool enabled)
{
    QSignalBlocker b(ui_snapButton);

    ui_snapButton->setChecked(enabled);

    ui_nodeView->grid()->setSnapEnabled(enabled);

    RamUser *u = Ramses::instance()->currentUser();
    if (!u) return;
    QSettings *uSettings = u->settings();
    uSettings->setValue("nodeView/snapToGrid", enabled);
}

void PipelineWidget::setGridSize(int size)
{
    QSignalBlocker b(ui_gridSizeBox);

    ui_gridSizeBox->setValue(size);
    ui_nodeView->grid()->setSize(size);
    ui_nodeView->update();

    RamUser *u = Ramses::instance()->currentUser();
    if (!u) return;
    QSettings *uSettings = u->settings();
    uSettings->setValue("nodeView/gridSize", size);

}

void PipelineWidget::userChanged(RamUser *u)
{
    if (!u) return;

    // Load template steps
    if (init)
    {
        for (int i = 0; i < Ramses::instance()->templateSteps()->rowCount(); i++) newTemplateStep( Ramses::instance()->templateSteps()->get(i) );
        connect(Ramses::instance()->templateSteps(), SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(templateStepInserted(QModelIndex,int,int)));
        connect(Ramses::instance()->templateSteps(), SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(templateStepRemoved(QModelIndex,int,int)));
        init = false;
    }

    QSettings *uSettings = u->settings();

    setSnapEnabled(uSettings->value("nodeView/snapToGrid", false).toBool());
    setGridSize(uSettings->value("nodeView/gridSize", 20).toInt());
}

void PipelineWidget::createStep()
{
    RamProject *project = Ramses::instance()->currentProject();
    if (!project) return;

    RamStep *step = new RamStep(
                "NEW",
                "New step",
                project);
    project->steps()->appendObject(step->uuid());
    step->edit();
}

void PipelineWidget::templateStepInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    for (int i = first; i <= last; i++)
    {
       RamObject *o = Ramses::instance()->templateSteps()->get(i);
        newTemplateStep(o);
    }
}

void PipelineWidget::newTemplateStep(RamObject *obj)
{
    if (!obj) return;
    if (obj->uuid() == "") return;
    QAction *stepAction = new QAction(obj->name());
    quintptr iptr = reinterpret_cast<quintptr>( obj );
    stepAction->setData(iptr);
    ui_stepMenu->insertAction(ui_stepMenuSeparator, stepAction);
    connect(stepAction, &QAction::triggered, this, &PipelineWidget::assignStep);
    connect(obj, &RamObject::dataChanged, this, &PipelineWidget::templateStepChanged);
}

void PipelineWidget::templateStepRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    QList<QAction *> actions = ui_stepMenu->actions();

    for (int i = first; i <= last; i++)
    {
        RamObject *removedObj = Ramses::instance()->templateSteps()->get(i);
        for (int j = actions.count() -1; j >= 0; j--)
        {
            quintptr iptr = actions.at(j)->data().toULongLong();
            RamObject *obj = reinterpret_cast<RamObject*>( iptr );

            if (removedObj->is(obj))
            {
                actions.at(j)->deleteLater();
                break;
            }
        }
    }
}

void PipelineWidget::templateStepChanged()
{
    RamObject *changedObj = qobject_cast<RamObject*>( sender() );
    QList<QAction *> actions = ui_stepMenu->actions();
    for (int i = actions.count() -1; i >= 0; i--)
    {
        quintptr iptr = actions.at(i)->data().toULongLong();
        RamObject *obj = reinterpret_cast<RamObject*>( iptr );

        if (changedObj->is(obj)) actions.at(i)->setText(changedObj->name());
    }
}

void PipelineWidget::assignStep()
{
    RamProject *project = Ramses::instance()->currentProject();
    if (!project) return;
    QAction *stepAction = (QAction*)sender();
    quintptr iptr = stepAction->data().toULongLong();
    RamStep *templateStep = reinterpret_cast<RamStep*>( iptr );
    if (!templateStep) return;
    RamStep *step = RamStep::createFromTemplate(templateStep, project);
    project->steps()->appendObject(step->uuid());
    step->edit();
}

void PipelineWidget::newPipe(RamObject *p)
{
    RamPipe *pipe = qobject_cast<RamPipe*>(p);
    if (!pipe) return;

    // Get nodes
    DuQFNode *inputNode = nullptr;
    DuQFNode *outputNode = nullptr;
    foreach(DuQFNode *n, m_nodeScene->nodes())
    {
        StepNode *is = (StepNode*)n;
        if (is) if (is->step()->uuid() == pipe->inputStep()->uuid()) inputNode = n;

        StepNode *os = (StepNode*)n;
        if (os) if (os->step()->uuid() == pipe->outputStep()->uuid()) outputNode = n;

        if (inputNode && outputNode) break;
    }

    if (!outputNode) return;
    if (!inputNode) return;

    QSignalBlocker b(m_nodeScene->connectionManager());

    // Get or create the node connections
    DuQFConnection *co = m_nodeScene->connectNodes(outputNode, inputNode);
    if (!co) return;

    // Title
    co->connector()->setTitle( p->name() );

    QList<QMetaObject::Connection> c;
    c << connect(pipe, &RamPipe::dataChanged, this, &PipelineWidget::pipeChanged);
    c << connect(co->connector(), SIGNAL(clicked()), pipe, SLOT(edit()));
    m_pipeObjectConnections[pipe->uuid()] = c;

    m_pipeConnections[pipe->uuid()] = co;
}

void PipelineWidget::newPipe(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    for (int i = first; i <= last; i++)
    {
        RamObject *o = m_project->pipeline()->get(i);
        newPipe(o);
    }
}

void PipelineWidget::stepsConnected(DuQFConnection *co)
{
    RamProject *project = Ramses::instance()->currentProject();
    if (!project) return;
    // Get steps
    StepNode *outputNode = (StepNode*)co->outputNode();
    StepNode *inputNode = (StepNode*)co->inputNode();
    if (!outputNode) return;
    if (!inputNode) return;
    RamStep *output = outputNode->step();
    RamStep *input = inputNode->step();
    if (!output) return;
    if (!input) return;

    RamPipe *pipe = new RamPipe(output, input);
    project->pipeline()->appendObject(pipe->uuid());
    pipe->edit();
}

void PipelineWidget::connectionRemoved(DuQFConnection *co)
{
    RamProject *project = Ramses::instance()->currentProject();
    if (!project) return;
    // Get steps
    StepNode *outputNode = (StepNode*)co->outputNode();
    StepNode *inputNode = (StepNode*)co->inputNode();
    if (!outputNode) return;
    if (!inputNode) return;
    RamStep *output = outputNode->step();
    RamStep *input = inputNode->step();
    if (!output) return;
    if (!input) return;

    RamPipe *p = project->pipe(output, input);
    project->pipeline()->removeObjects(QStringList(p->uuid()));
    if (p) p->remove();
}

void PipelineWidget::loadProjectLayout()
{
    foreach(DuQFNode *n, m_nodeScene->nodes())
    {
        ObjectNode *node = qgraphicsitem_cast<ObjectNode*>( n );
        RamObject *step = node->ramObject();

        QSettings *pSettings = m_project->settings();
        pSettings->beginGroup("nodeView");
        pSettings->beginGroup("nodeLocations");

        QPointF pos = pSettings->value(step->uuid(), QPointF(0.0,0.0)).toPointF();
        if (pos.x() != 0.0 && pos.y() != 0.0)
        {
            node->setPos( pos );
        }

        pSettings->endGroup();
        pSettings->endGroup();
    }
}

void PipelineWidget::saveProjectLayout()
{
    QMessageBox::StandardButton confirm = QMessageBox::question( this,
        "Confirm overwrite",
        "This will overwrite the default layout for this project, for all users.\nDo you want to continue?" );

    if ( confirm != QMessageBox::Yes) return;

    foreach(DuQFNode *n, m_nodeScene->nodes())
    {
        ObjectNode *node = qgraphicsitem_cast<ObjectNode*>( n );
        RamObject *step = node->ramObject();
        QPointF pos = node->pos();

        QSettings *pSettings = m_project->settings();
        pSettings->beginGroup("nodeView");
        pSettings->beginGroup("nodeLocations");
        pSettings->setValue(step->uuid(), pos);
        pSettings->endGroup();
        pSettings->endGroup();
    }
}

void PipelineWidget::pipeChanged(RamObject *p)
{

    RamPipe *pipe = qobject_cast<RamPipe*>( p );

    if (!pipe) return;

    if (m_pipeConnections.contains(pipe->uuid()))
    {
        DuQFConnection *co = m_pipeConnections.value(pipe->uuid());

        //update stepnodes
        bool outputOk = false;
        bool inputOk = false;

        foreach(DuQFNode *n, m_nodeScene->nodes())
        {
            StepNode *outputNode = (StepNode*)n;
            if (outputNode) if (outputNode->step()->uuid() == pipe->outputStep()->uuid())
            {
                co->setOutput(outputNode->defaultOutputSlot());
                outputOk = true;
            }

            StepNode *inputNode = (StepNode*)n;
            if (inputNode) if (inputNode->step()->uuid() == pipe->inputStep()->uuid())
            {
                co->setInput(inputNode->defaultInputSlot());
                inputOk = true;
            }

            if (inputOk && outputOk) break;
        }

        co->connector()->setTitle( p->name() );
    }
}

void PipelineWidget::pipeRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    for (int i = first; i <= last; i++)
    {
        RamObject *p = m_project->pipeline()->get(i);
        if (m_pipeObjectConnections.contains(p->uuid()))
        {
            QList<QMetaObject::Connection> c = m_pipeObjectConnections.take(p->uuid());
            while(!c.isEmpty()) disconnect(c.takeLast());
        }
        if (m_pipeConnections.contains(p->uuid()))
        {
            //remove connection
            m_pipeConnections.value(p->uuid())->remove();
            //and remove its pointer from the list
            m_pipeConnections.remove(p->uuid());
        }
    }
}

void PipelineWidget::showEvent(QShowEvent *event)
{
    if (!event->spontaneous())
    {
        ui_titleBar->show();
        changeProject();
    }
    QWidget::showEvent(event);
}

void PipelineWidget::hideEvent(QHideEvent *event)
{
    if (!event->spontaneous())
    {
        m_nodeScene->clearSelection();
        m_nodeScene->clearFocus();
        ui_titleBar->hide();
    }
    QWidget::hideEvent(event);
}

void PipelineWidget::resetProject()
{
    m_projectChanged = true;
    if (this->isVisible()) changeProject();
}

void PipelineWidget::changeProject()
{
    if (!m_projectChanged) return;
    m_projectChanged = false;

    ProgressManager *pm = ProgressManager::instance();

    QSignalBlocker b1(m_nodeScene);
    QSignalBlocker b2(ui_nodeView);
    QSignalBlocker b3(m_nodeScene->connectionManager());

    this->setEnabled(false);

    // Clear scene
    m_nodeScene->clear();
    m_pipeConnections.clear();

    if (!m_project) return;

    pm->start();
    pm->setText("Loading project...");
    pm->setMaximum( m_project->steps()->rowCount() + m_project->pipeline()->rowCount() );

    // add steps
    for(int i = 0; i < m_project->steps()->rowCount(); i++)
    {
        pm->setText("Building step nodes...");
        pm->increment();
        newStep( m_project->steps()->get(i) );
    }

    // add pipes
    for ( int i = 0; i < m_project->pipeline()->rowCount(); i++ )
    {
        pm->setText("Building pipes...");
        pm->increment();
        newPipe( m_project->pipeline()->get(i) );
    }

    connect(m_project->steps(), SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(newStep(QModelIndex,int,int)));
    connect(m_project->pipeline(), SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(newPipe(QModelIndex,int,int)));
    connect(m_project->pipeline(), SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(pipeRemoved(QModelIndex,int,int)));
    connect(m_project->pipeline(), &RamObjectModel::modelReset, this, &PipelineWidget::resetProject);

    // Layout
    m_nodeScene->clearSelection();
    //_nodeView->frameSelected();

    this->setEnabled(true);

    pm->finish();
}

