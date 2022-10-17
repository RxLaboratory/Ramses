#include "itemmanagerwidget.h"

#include "duqf-utils/guiutils.h"
#include "ramasset.h"
#include "ramses.h"
#include "ramassetgroup.h"
#include "ramsequence.h"
#include "shotscreationdialog.h"
#include "progressmanager.h"

ItemManagerWidget::ItemManagerWidget(RamTemplateStep::Type type, QWidget *parent) : QWidget(parent)
{   
    m_productionType = type;
    setupUi();

    currentUserChanged(nullptr);
    filter(nullptr);

    connectEvents();
}

void ItemManagerWidget::selectAllSteps()
{
    QList<QAction*> actions = ui_stepMenu->actions();
    for (int i = 4; i < actions.count(); i++)
    {
        actions[i]->setChecked(true);
    }
}

void ItemManagerWidget::selectUserSteps()
{
    QList<QAction*> actions = ui_stepMenu->actions();
    RamUser *u = Ramses::instance()->currentUser();

    for (int i = 4; i < actions.count(); i++)
    {
        RamStep *step = RamStep::c( ui_stepMenu->objectAt(i) );
        if (!step) continue;
        if (u->isStepAssigned(step))
            actions[i]->setChecked(true);
        else
            actions[i]->setChecked(false);
    }
}

void ItemManagerWidget::deselectSteps()
{
    QList<QAction*> actions = ui_stepMenu->actions();
    for (int i = 4; i < actions.count(); i++)
    {
        actions[i]->setChecked(false);
    }
}

void ItemManagerWidget::selectAllUsers()
{
    QList<QAction*> actions = ui_userMenu->actions();
    for (int i = 4; i < actions.count(); i++)
    {
        actions[i]->setChecked(true);
    }
}

void ItemManagerWidget::selectMyself()
{
    QList<QAction*> actions = ui_userMenu->actions();
    RamUser *u = Ramses::instance()->currentUser();

    ui_actionNotAssigned->setChecked(false);

    for (int i = 4; i < actions.count(); i++)
    {
        RamUser *user = RamUser::c( ui_userMenu->objectAt(i) );
        if (!user) continue;
        if (user->is(u))
            actions[i]->setChecked(true);
        else
            actions[i]->setChecked(false);
    }
}

void ItemManagerWidget::deselectUsers()
{
    QList<QAction*> actions = ui_userMenu->actions();
    for (int i = 4; i < actions.count(); i++)
    {
        actions[i]->setChecked(false);
    }
}

void ItemManagerWidget::showUnassigned(bool show)
{
    ui_table->filteredModel()->showUnassigned(show);
    if (show) ui_table->resizeRowsToContents();
    checkUserFilters();
}

void ItemManagerWidget::selectAllStates()
{
    QList<QAction*> actions = ui_stateMenu->actions();
    for (int i = 3; i < actions.count(); i++)
    {
        actions[i]->setChecked(true);
    }
}

void ItemManagerWidget::deselectStates()
{
    QList<QAction*> actions = ui_stateMenu->actions();
    for (int i = 3; i < actions.count(); i++)
    {
        actions[i]->setChecked(false);
    }
}

void ItemManagerWidget::showEvent(QShowEvent *event)
{
    if (!event->spontaneous()) {
        ui_titleBar->show();
        changeProject();
    }
    QWidget::showEvent(event);
}

void ItemManagerWidget::hideEvent(QHideEvent *event)
{
    if (!event->spontaneous())
    {
        ui_titleBar->hide();
    }

    // Save filters and layout
    RamUser *user = Ramses::instance()->currentUser();
    if (user)
    {
        QSettings *uSettings = user->settings();

        if (m_productionType == RamStep::AssetProduction)
            uSettings->beginGroup("assetTable");
        else
            uSettings->beginGroup("shotTable");

        // View
        //uSettings->setValue("showTimeTracking", ui_actionTimeTracking->isChecked());
        //uSettings->setValue("showCompletion", ui_actionCompletionRatio->isChecked());
        uSettings->setValue("showDetails", ui_actionShowDetails->isChecked());
        // Users
        ui_userMenu->saveState(uSettings, "users");
        // Steps
        ui_stepMenu->saveState(uSettings, "steps");
        // States
        ui_stateMenu->saveState(uSettings, "states");

        uSettings->endGroup();
    }
    QWidget::hideEvent(event);
}

void ItemManagerWidget::projectChanged(RamProject *project, bool force)
{
    if (m_project) disconnect(m_project, nullptr, this, nullptr);

    if (!m_project && !project) return;
    if (m_project) if (m_project->is(project) && !force ) return;

    m_projectChanged = true;

    m_project = project;
    // Reload in the show event if not yet visible
    // to improve perf: do not refresh all the app when changing the project, only what's visible.
    if ( this->isVisible() ) changeProject();
}

void ItemManagerWidget::showUser(RamObject *user, bool s)
{
    if(!m_project) return;
    if(s)
    {
        ui_table->filteredModel()->showUser( user );
        //ui_table->resizeRowsToContents();
    }
    else
    {
        ui_table->filteredModel()->hideUser( user );
    }
    checkUserFilters();
}

void ItemManagerWidget::showStep(RamObject *step, bool s)
{
    if(!m_project) return;
    if (s)
    {
        ui_table->filteredModel()->showStep( step );
        ui_table->resizeColumnsToContents();
        //ui_table->resizeRowsToContents();
    }
    else
    {
        ui_table->filteredModel()->hideStep( step );
    }
    checkStepFilters();
}

void ItemManagerWidget::showState(RamObject *state, bool s)
{
    if(!m_project) return;
    if(s)
    {
        ui_table->filteredModel()->showState(state);
        ui_table->resizeRowsToContents();
    }
    else
    {
        ui_table->filteredModel()->hideState(state);
    }
    checkStateFilters();
}

void ItemManagerWidget::checkStepFilters()
{
    bool ok = ui_stepMenu->isAllChecked();

    QString t = ui_stepButton->text().replace(" ▽","");
    if (!ok) t = t + " ▽";
    ui_stepButton->setText( t );
}

void ItemManagerWidget::checkUserFilters()
{
    bool ok = ui_userMenu->isAllChecked();

    QString t = ui_userButton->text().replace(" ▽","");
    if (!ok) t = t + " ▽";
    ui_userButton->setText( t );
}

void ItemManagerWidget::checkStateFilters()
{
    bool ok = ui_stateMenu->isAllChecked();

    QString t = ui_stateButton->text().replace(" ▽","");
    if (!ok) t = t + " ▽";
    ui_stateButton->setText( t );
}

void ItemManagerWidget::filter(RamObject *filterObj)
{
    ui_table->filter(filterObj);

    if (m_productionType == RamStep::AssetProduction)
    {
        RamAssetGroup *ag = RamAssetGroup::c( filterObj );
        int assetCount = 0;
        if (filterObj)
        {
            assetCount = ag->assetCount();
        }
        else if (m_project)
        {
            assetCount = m_project->assets()->rowCount();
        }
        if (assetCount == 1) ui_titleBar->setTitle(QString::number(assetCount) + " Asset");
        else if (assetCount > 0) ui_titleBar->setTitle(QString::number(assetCount) + " Assets");
        else ui_titleBar->setTitle("Assets");
    }
    else if (m_productionType == RamStep::ShotProduction)
    {
        RamSequence *seq = RamSequence::c( filterObj );
        int shotCount = 0;
        double duration = 0;
        if (filterObj)
        {
            shotCount = seq->shotCount();
            duration = seq->duration();
        }
        else if (m_project)
        {
            shotCount = m_project->shots()->rowCount();
            duration = m_project->duration();
        }
        QString title;
        if (shotCount == 1) title = QString::number(shotCount) + " Shot";
        else if (shotCount > 0) title = QString::number(shotCount) + " Shots";
        else title = "Shots";
        if (duration > 0) title += " (" + MediaUtils::durationToTimecode(duration) + ")";
        ui_titleBar->setTitle(title);
    }
}

void ItemManagerWidget::uncheckSort()
{
    ui_actionSortDefault->setChecked(false);
    ui_actionSortByShortName->setChecked(false);
    ui_actionSortByName->setChecked(false);
    ui_actionSortByDifficulty->setChecked(false);
    ui_actionSortByTimeSpent->setChecked(false);
    ui_actionSortByEstimation->setChecked(false);
    ui_actionSortByCompletion->setChecked(false);
}

void ItemManagerWidget::sortDefault(bool sort)
{
    Q_UNUSED(sort)
    uncheckSort();
    ui_actionSortDefault->setChecked(true);
    ui_header->setSortable(false);
    ui_table->filteredModel()->setSortMode(Qt::InitialSortOrderRole);
}

void ItemManagerWidget::sortByShortName(bool sort)
{
    uncheckSort();
    ui_actionSortByShortName->setChecked(sort);
    ui_header->setSortable(sort);
    ui_actionSortDefault->setChecked( !sort );
    ui_table->filteredModel()->setSortMode(RamObject::ShortName);
}

void ItemManagerWidget::sortByName(bool sort)
{
    uncheckSort();
    ui_actionSortByName->setChecked(sort);
    ui_header->setSortable(sort);
    ui_actionSortDefault->setChecked( !sort );
    ui_table->filteredModel()->setSortMode(RamObject::Name);
}

void ItemManagerWidget::sortByDifficulty(bool sort)
{
    uncheckSort();
    ui_actionSortByDifficulty->setChecked(sort);
    ui_header->setSortable(sort);
    ui_actionSortDefault->setChecked( !sort );
    ui_table->filteredModel()->setSortMode(RamObject::Difficulty);
}

void ItemManagerWidget::sortByTimeSpent(bool sort)
{
    uncheckSort();
    ui_actionSortByTimeSpent->setChecked(sort);
    ui_header->setSortable(sort);
    ui_actionSortDefault->setChecked( !sort );
    ui_table->filteredModel()->setSortMode(RamObject::TimeSpent);
}

void ItemManagerWidget::sortByEstimation(bool sort)
{
    uncheckSort();
    ui_actionSortByEstimation->setChecked(sort);
    ui_header->setSortable(sort);
    ui_actionSortDefault->setChecked( !sort );
    ui_table->filteredModel()->setSortMode(RamObject::Estimation);
}

void ItemManagerWidget::sortByCompletion(bool sort)
{
    uncheckSort();
    ui_actionSortByCompletion->setChecked(sort);
    ui_header->setSortable(sort);
    ui_actionSortDefault->setChecked( !sort );
    ui_table->filteredModel()->setSortMode(RamObject::Completion);
}

void ItemManagerWidget::unassignUser()
{
    QVector<RamStatus*> status = beginEditSelectedStatus();
    for (int i = 0; i < status.count(); i++)
    {
        status.at(i)->assignUser(nullptr);
    }
}

void ItemManagerWidget::assignUser(RamObject *user)
{
    if (!user) return;

    QVector<RamStatus*> status = beginEditSelectedStatus();

    ProgressManager *pm = ProgressManager::instance();
    pm->setTitle(tr("Updating status"));
    pm->setText("Assigning user...");
    pm->setMaximum(status.count());
    pm->start();

    for (int i = 0; i < status.count(); i++)
    {
        pm->increment();
        status.at(i)->assignUser(user);
    }
    pm->finish();
}

void ItemManagerWidget::changeState(RamObject *stt)
{
    if (!stt) return;

    QVector<RamStatus*> status = beginEditSelectedStatus();

    ProgressManager *pm = ProgressManager::instance();
    pm->setTitle(tr("Updating status"));
    pm->setText("Changing status...");
    pm->setMaximum(status.count());
    pm->start();

    for (int i = 0; i < status.count(); i++)
    {
        pm->increment();
        status.at(i)->setState(RamState::c( stt ));
    }
    pm->finish();
}

void ItemManagerWidget::setVeryEasy()
{
    setDiffculty(RamStatus::VeryEasy);
}

void ItemManagerWidget::setEasy()
{
    setDiffculty(RamStatus::Easy);
}

void ItemManagerWidget::setMedium()
{
    setDiffculty(RamStatus::Medium);
}

void ItemManagerWidget::setHard()
{
    setDiffculty(RamStatus::Hard);
}

void ItemManagerWidget::setVeryHard()
{
    setDiffculty(RamStatus::VeryHard);
}

void ItemManagerWidget::setDiffculty(RamStatus::Difficulty difficulty)
{
    QVector<RamStatus*> status = beginEditSelectedStatus();

    ProgressManager *pm = ProgressManager::instance();
    pm->setTitle(tr("Updating status"));
    pm->setText("Changing difficulty...");
    pm->setMaximum(status.count());
    pm->start();

    for (int i = 0; i < status.count(); i++)
    {
        pm->increment();
        status.at(i)->setDifficulty( difficulty );
    }
    pm->finish();

}

void ItemManagerWidget::setCompletion()
{
    QAction* action = qobject_cast<QAction*>( sender() );
    int completion = action->data().toInt();
    QVector<RamStatus*> status = beginEditSelectedStatus();

    ProgressManager *pm = ProgressManager::instance();
    pm->setTitle(tr("Updating status"));
    pm->setText("Setting completion ratio...");
    pm->setMaximum(status.count());
    pm->start();

    for (int i = 0; i < status.count(); i++)
    {
        pm->increment();
        status.at(i)->setCompletionRatio( completion );
    }
    pm->finish();
}

void ItemManagerWidget::copyComment()
{
    QModelIndex currentIndex = ui_table->selectionModel()->currentIndex();
    if ( !currentIndex.isValid() ) return;

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText( currentIndex.data(RamObject::Comment).toString() );
}

void ItemManagerWidget::cutComment()
{
    QModelIndex currentIndex = ui_table->selectionModel()->currentIndex();
    if ( !currentIndex.isValid() ) return;

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText( currentIndex.data(RamObject::Comment).toString() );

    // Get the status to edit it
    RamStatus *status = getStatus(currentIndex);

    // If it's not the current user, create a new one
    RamUser *currentUser = Ramses::instance()->currentUser();
    if(!status->user()->is(currentUser))
        status = RamStatus::copy( status, currentUser );

    status->setComment("");
}

void ItemManagerWidget::pasteComment()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString comment = clipboard->text();
    if (comment == "") return;

    QVector<RamStatus*> status = beginEditSelectedStatus();

    ProgressManager *pm = ProgressManager::instance();
    pm->setTitle(tr("Updating status"));
    pm->setText("Setting comment...");
    pm->setMaximum(status.count());
    pm->start();

    for (int i = 0; i < status.count(); i++)
    {
        pm->increment();
        status.at(i)->setComment( comment );
    }
    pm->finish();
}

void ItemManagerWidget::createItem()
{
    RamProject *project = Ramses::instance()->currentProject();
    if (!project) return;

    if ( m_productionType == RamStep::AssetProduction )
    {
        if (project->assetGroups()->rowCount() == 0 ) return;

        RamObject *filterObj = ui_groupBox->currentObject();
        RamAssetGroup *ag = RamAssetGroup::c( filterObj );

        if (!ag) ag = RamAssetGroup::c( project->assetGroups()->get(0) );
        if (!ag) return;

        RamAsset *asset = new RamAsset(
                    "NEW",
                    "New Asset",
                    ag
                    );

        project->assets()->appendObject(asset->uuid());
        asset->edit();
    }
    else if ( m_productionType == RamStep::ShotProduction )
    {
        if (project->sequences()->rowCount() == 0 ) return;

        RamObject *filterObj = ui_groupBox->currentObject();
        RamSequence *seq = RamSequence::c( filterObj );
        if (!seq) seq = RamSequence::c( project->sequences()->get(0) );
        if(!seq) return;

        RamShot *shot = new RamShot(
                    "NEW",
                    "New Shot",
                    seq
                    );

        project->shots()->appendObject(shot->uuid());
        shot->edit();
    }
}

void ItemManagerWidget::deleteItems()
{
    RamProject *project = Ramses::instance()->currentProject();
    if (!project) return;

    QModelIndexList selection = ui_table->selectionModel()->selectedIndexes();
    QModelIndexList selectedItems;

    if (selection.count() == 0) return;

    QMessageBox::StandardButton confirm = QMessageBox::question( this,
        "Confirm deletion",
        "Are you sure you want to premanently remove the selected items?" );

    if ( confirm != QMessageBox::Yes) return;

    for (int i = 0; i < selection.count(); i++)
    {
        QString objUuid = selection.at(i).data(RamObject::UUID).toString();
        if (objUuid == "") continue;
        if (m_productionType == RamStep::AssetProduction)
        {
            RamObject *o = RamObject::get(objUuid, RamObject::Asset);
            m_project->assets()->removeObjects(QStringList(objUuid));
            if (o) o->remove();
        }
        else if (m_productionType == RamStep::ShotProduction)
        {
            RamObject *o = RamObject::get(objUuid, RamObject::Shot);
            m_project->shots()->removeObjects(QStringList(objUuid));
            if (o) o->remove();
        }
    }
}

void ItemManagerWidget::createMultiple()
{
    RamProject *project = Ramses::instance()->currentProject();
    if (!project) return;

    ShotsCreationDialog dialog(project, this);
    dialog.exec();
}

void ItemManagerWidget::contextMenuRequested(QPoint p)
{
    // Call the context menu
    ui_contextMenu->popup(ui_table->viewport()->mapToGlobal(p));
}

void ItemManagerWidget::currentUserChanged(RamUser *user)
{
    ui_actionItem->setVisible(false);
    if (!user) return;
    if (user->role() >= RamUser::ProjectAdmin)
    {
        ui_actionItem->setVisible(true);
    }
    loadSettings();
    // Reload project
    //projectChanged(Ramses::instance()->currentProject(), true);
}

void ItemManagerWidget::setupUi()
{
    // Get the mainwindow to add the titlebar
    QMainWindow *mw = GuiUtils::appMainWindow();
    mw->addToolBarBreak(Qt::TopToolBarArea);

    ui_titleBar = new DuQFTitleBar("Assets",false, mw);
    ui_titleBar->setObjectName("assetToolBar");
    ui_titleBar->showReinitButton(false);
    mw->addToolBar(Qt::TopToolBarArea,ui_titleBar);
    ui_titleBar->setFloatable(false);
    ui_titleBar->hide();

    // group box
    ui_groupBox = new RamObjectComboBox(this);
    ui_titleBar->insertLeft(ui_groupBox);

    // Search field
    ui_searchEdit = new DuQFSearchEdit(this);
    ui_searchEdit->setMaximumWidth(150);
    ui_searchEdit->hideSearchButton();
    ui_titleBar->insertLeft(ui_searchEdit);

    // View Menu
    QMenu *viewMenu = new QMenu(this);

    /*ui_actionTimeTracking = new QAction("Show time tracking", this);
    ui_actionTimeTracking->setCheckable(true);
    ui_actionTimeTracking->setChecked(true);
    viewMenu->addAction(ui_actionTimeTracking);//*/

    /*ui_actionCompletionRatio = new QAction("Show completion", this);
    ui_actionCompletionRatio->setCheckable(true);
    ui_actionCompletionRatio->setChecked(true);
    viewMenu->addAction(ui_actionCompletionRatio);//*/

    ui_actionShowDetails = new QAction("Show details", this);
    ui_actionShowDetails->setCheckable(true);
    viewMenu->addAction(ui_actionShowDetails);

    QToolButton *viewButton = new QToolButton(this);
    viewButton->setText(" View");
    viewButton->setIcon(QIcon(":/icons/show"));
    viewButton->setMenu(viewMenu);
    viewButton->setIconSize(QSize(16,16));
    viewButton->setObjectName("menuButton");
    viewButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    viewButton->setPopupMode(QToolButton::InstantPopup);
    ui_titleBar->insertLeft(viewButton);

    // Sort Menu
    QMenu *sortMenu = new QMenu(this);

    ui_actionSortDefault = new QAction("Default", this);
    ui_actionSortDefault->setCheckable(true);
    ui_actionSortDefault->setChecked(true);
    sortMenu->addAction(ui_actionSortDefault);

    ui_actionSortByShortName = new QAction("By ID", this);
    ui_actionSortByShortName->setCheckable(true);
    sortMenu->addAction(ui_actionSortByShortName);

    ui_actionSortByName = new QAction("By name", this);
    ui_actionSortByName->setCheckable(true);
    sortMenu->addAction(ui_actionSortByName);

    ui_actionSortByDifficulty = new QAction("By difficulty", this);
    ui_actionSortByDifficulty->setCheckable(true);
    sortMenu->addAction(ui_actionSortByDifficulty);

    ui_actionSortByTimeSpent = new QAction("By time spent", this);
    ui_actionSortByTimeSpent->setCheckable(true);
    sortMenu->addAction(ui_actionSortByTimeSpent);

    ui_actionSortByEstimation = new QAction("By estimation / Goal", this);
    ui_actionSortByEstimation->setCheckable(true);
    sortMenu->addAction(ui_actionSortByEstimation);

    ui_actionSortByCompletion = new QAction("By completion ratio", this);
    ui_actionSortByCompletion->setCheckable(true);
    sortMenu->addAction(ui_actionSortByCompletion);

    QToolButton *sortButton = new QToolButton(this);
    sortButton->setText(" Sort");
    sortButton->setIcon(QIcon(":/icons/sort"));
    sortButton->setMenu(sortMenu);
    sortButton->setIconSize(QSize(16,16));
    sortButton->setObjectName("menuButton");
    sortButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    sortButton->setPopupMode(QToolButton::InstantPopup);
    ui_titleBar->insertLeft(sortButton);

    // Item Menu
    ui_itemMenu = new QMenu(this);

    setupItemMenu();

    // User Menu
    ui_userMenu = new RamObjectMenu(true, this);

    ui_actionSelectMyself = new QAction("Select myself", this);
    ui_userMenu->insertAction( ui_userMenu->actions().at(0), ui_actionSelectMyself);

    ui_actionNotAssigned = new QAction("Unassigned", this);
    ui_actionNotAssigned->setData("unassigned");
    ui_actionNotAssigned->setCheckable(true);
    ui_actionNotAssigned->setChecked(true);
    ui_userMenu->addAction(ui_actionNotAssigned);

    ui_userButton = new QToolButton(this);
    ui_userButton->setText(" Users");
    ui_userButton->setIcon(QIcon(":/icons/user"));
    ui_userButton->setIconSize(QSize(16,16));
    ui_userButton->setObjectName("menuButton");
    ui_userButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui_userButton->setPopupMode(QToolButton::InstantPopup);
    ui_userButton->setMenu(ui_userMenu);

    ui_titleBar->insertLeft(ui_userButton);

    // Step Menu
    ui_stepMenu = new RamObjectMenu(true, this);

    ui_actionSelectMySteps = new QAction("Select my steps", this);
    ui_stepMenu->insertAction( ui_stepMenu->actions().at(2), ui_actionSelectMySteps);

    ui_stepButton = new QToolButton(this);
    ui_stepButton->setText(" Steps");
    ui_stepButton->setIcon(QIcon(":/icons/step"));
    ui_stepButton->setIconSize(QSize(16,16));
    ui_stepButton->setObjectName("menuButton");
    ui_stepButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui_stepButton->setPopupMode(QToolButton::InstantPopup);
    ui_stepButton->setMenu(ui_stepMenu);

    ui_titleBar->insertLeft(ui_stepButton);

    // State menu

    ui_stateMenu = new RamObjectMenu(true, this);
    ui_stateMenu->setSortMode(RamObject::Completion);

    ui_stateButton = new QToolButton(this);
    ui_stateButton->setText(" States");
    ui_stateButton->setIcon(QIcon(":/icons/state-l"));
    ui_stateButton->setIconSize(QSize(16,16));
    ui_stateButton->setObjectName("menuButton");
    ui_stateButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui_stateButton->setPopupMode(QToolButton::InstantPopup);
    ui_stateButton->setMenu(ui_stateMenu);

    ui_stateMenu->setObjectModel( Ramses::instance()->states() );

    ui_titleBar->insertLeft(ui_stateButton);

    // Status menu
    QMenu *statusMenu = new QMenu(this);

    ui_copyComment = new QAction("Copy comment", this);
    ui_copyComment->setShortcut(QKeySequence("Ctrl+C"));
    ui_copyComment->setIcon(QIcon(":/icons/copy"));
    statusMenu->addAction(ui_copyComment);

    ui_cutComment = new QAction("Cut comment", this);
    ui_cutComment->setShortcut(QKeySequence("Ctrl+X"));
    ui_cutComment->setIcon(QIcon(":/icons/cut"));
    statusMenu->addAction(ui_cutComment);

    ui_pasteComment = new QAction("Paste as comment", this);
    ui_pasteComment->setShortcut(QKeySequence("Ctrl+V"));
    ui_pasteComment->setIcon(QIcon(":/icons/paste"));
    statusMenu->addAction(ui_pasteComment);

    statusMenu->addSeparator();

    ui_assignUserMenu = new RamObjectMenu(false, this);
    ui_assignUserMenu->setTitle("Assign user");
    ui_assignUserMenu->addCreateButton();
    ui_assignUserMenu->actions().at(0)->setText("None");
    statusMenu->addMenu(ui_assignUserMenu);

    ui_changeStateMenu = new RamObjectMenu(false, this);
    ui_changeStateMenu->setTitle("Change state");
    ui_changeStateMenu->setObjectModel(Ramses::instance()->states());
    statusMenu->addMenu(ui_changeStateMenu);

    ui_changeDifficultyMenu = new QMenu("Change difficulty", this);
    ui_veryEasy = new QAction("Very easy", this);
    ui_easy = new QAction("Easy", this);
    ui_medium = new QAction("Medium", this);
    ui_hard = new QAction("Hard", this);
    ui_veryHard = new QAction("Very hard", this);
    ui_changeDifficultyMenu->addAction(ui_veryEasy);
    ui_changeDifficultyMenu->addAction(ui_easy);
    ui_changeDifficultyMenu->addAction(ui_medium);
    ui_changeDifficultyMenu->addAction(ui_hard);
    ui_changeDifficultyMenu->addAction(ui_veryHard);
    statusMenu->addMenu(ui_changeDifficultyMenu);

    QMenu *completionMenu = new QMenu("Set completion", this);
    ui_completion0   = new QAction("0%",this);
    ui_completion10  = new QAction("10%",this);
    ui_completion25  = new QAction("25%",this);
    ui_completion50  = new QAction("50%",this);
    ui_completion75  = new QAction("75%",this);
    ui_completion90  = new QAction("90%",this);
    ui_completion100 = new QAction("100%",this);
    ui_completion0->setData(0);
    ui_completion10->setData(10);
    ui_completion25->setData(25);
    ui_completion50->setData(50);
    ui_completion75->setData(75);
    ui_completion90->setData(90);
    ui_completion100->setData(100);
    completionMenu->addAction(ui_completion0  );
    completionMenu->addAction(ui_completion10 );
    completionMenu->addAction(ui_completion25 );
    completionMenu->addAction(ui_completion50 );
    completionMenu->addAction(ui_completion75 );
    completionMenu->addAction(ui_completion90 );
    completionMenu->addAction(ui_completion100);
    statusMenu->addMenu(completionMenu);


    QToolButton *statusButton = new QToolButton(this);
    statusButton->setText(" Status");
    statusButton->setIcon(QIcon(":/icons/status"));
    statusButton->setIconSize(QSize(16,16));
    statusButton->setObjectName("menuButton");
    statusButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    statusButton->setPopupMode(QToolButton::InstantPopup);
    statusButton->setMenu(statusMenu);

    ui_titleBar->insertLeft(statusButton);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(3);
    mainLayout->setContentsMargins(0,0,0,0);

    ui_table = new RamItemView(this);
    ui_table->setEditableObjects(false);
    ui_table->setContextMenuPolicy(Qt::CustomContextMenu);
    ui_header = new RamStepHeaderView(ui_table);
    ui_table->setHorizontalHeader( ui_header );
    ui_table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //ui_table->setTimeTracking(false);
    ui_header->setTimeTracking(false);
    setupTable();
    mainLayout->addWidget(ui_table);

    ui_table->filteredModel()->useFilters(true);

    ui_contextMenu = new QMenu(this);

    ui_contextMenu->addAction(ui_copyComment);
    ui_contextMenu->addAction(ui_cutComment);
    ui_contextMenu->addAction(ui_pasteComment);

    ui_contextMenu->addSeparator();

    ui_assignUserContextMenu = new RamObjectMenu(false, this);
    ui_assignUserContextMenu->setTitle("Assign user");
    ui_assignUserContextMenu->addCreateButton();
    ui_assignUserContextMenu->actions().at(0)->setText("None");
    ui_contextMenu->addMenu(ui_assignUserMenu);

    ui_changeStateContextMenu = new RamObjectMenu(false, this);
    ui_changeStateContextMenu->setSortMode(RamObject::Completion);
    ui_changeStateContextMenu->setTitle("Change state");
    ui_changeStateContextMenu->setObjectModel(Ramses::instance()->states());
    ui_contextMenu->addMenu(ui_changeStateContextMenu);

    QMenu *changeDifficultyContextMenu = new QMenu("Change difficulty", this);
    changeDifficultyContextMenu->addAction(ui_veryEasy);
    changeDifficultyContextMenu->addAction(ui_easy);
    changeDifficultyContextMenu->addAction(ui_medium);
    changeDifficultyContextMenu->addAction(ui_hard);
    changeDifficultyContextMenu->addAction(ui_veryHard);
    ui_contextMenu->addMenu(changeDifficultyContextMenu);

    QMenu *completionContextMenu = new QMenu("Set completion", this);
    completionContextMenu->addAction(ui_completion0  );
    completionContextMenu->addAction(ui_completion10 );
    completionContextMenu->addAction(ui_completion25 );
    completionContextMenu->addAction(ui_completion50 );
    completionContextMenu->addAction(ui_completion75 );
    completionContextMenu->addAction(ui_completion90 );
    completionContextMenu->addAction(ui_completion100);
    ui_contextMenu->addMenu(completionContextMenu);
}

void ItemManagerWidget::connectEvents()
{
    // Item actions
    connect(ui_actionCreateItem,SIGNAL(triggered()),this,SLOT(createItem()));
    connect(ui_actionDeleteItem,SIGNAL(triggered()),this,SLOT(deleteItems()));
    if (m_productionType == RamStep::ShotProduction)
        connect(ui_actionCreateMultiple,SIGNAL(triggered()),this,SLOT(createMultiple()));
    // Status actions
    connect(ui_assignUserMenu,SIGNAL(createTriggered()),this,SLOT(unassignUser()));
    connect(ui_assignUserMenu,SIGNAL(assigned(RamObject*)),this,SLOT(assignUser(RamObject*)));
    connect(ui_changeStateMenu,SIGNAL(assigned(RamObject*)),this,SLOT(changeState(RamObject*)));
    connect(ui_assignUserContextMenu,SIGNAL(createTriggered()),this,SLOT(unassignUser()));
    connect(ui_assignUserContextMenu,SIGNAL(assigned(RamObject*)),this,SLOT(assignUser(RamObject*)));
    connect(ui_changeStateContextMenu,SIGNAL(assigned(RamObject*)),this,SLOT(changeState(RamObject*)));
    connect(ui_veryEasy,SIGNAL(triggered()),this,SLOT(setVeryEasy()));
    connect(ui_easy,SIGNAL(triggered()),this,SLOT(setEasy()));
    connect(ui_medium,SIGNAL(triggered()),this,SLOT(setMedium()));
    connect(ui_hard,SIGNAL(triggered()),this,SLOT(setHard()));
    connect(ui_veryHard,SIGNAL(triggered()),this,SLOT(setVeryHard()));
    connect(ui_completion0  , SIGNAL(triggered()), this, SLOT( setCompletion() ) );
    connect(ui_completion10 , SIGNAL(triggered()), this, SLOT( setCompletion() ) );
    connect(ui_completion25 , SIGNAL(triggered()), this, SLOT( setCompletion() ) );
    connect(ui_completion50 , SIGNAL(triggered()), this, SLOT( setCompletion() ) );
    connect(ui_completion75 , SIGNAL(triggered()), this, SLOT( setCompletion() ) );
    connect(ui_completion90 , SIGNAL(triggered()), this, SLOT( setCompletion() ) );
    connect(ui_completion100, SIGNAL(triggered()), this, SLOT( setCompletion() ) );
    connect(ui_table, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequested(QPoint)));
    // view actions
    //connect(ui_actionTimeTracking, SIGNAL(toggled(bool)), ui_table, SLOT(setTimeTracking(bool)));
    //connect(ui_actionCompletionRatio, SIGNAL(toggled(bool)), ui_table, SLOT(setCompletionRatio(bool)));
    //connect(ui_actionTimeTracking, SIGNAL(toggled(bool)), ui_header, SLOT(setTimeTracking(bool)));
    //connect(ui_actionCompletionRatio, SIGNAL(toggled(bool)), ui_header, SLOT(setCompletionRatio(bool)));
    connect(ui_actionShowDetails, SIGNAL(toggled(bool)), ui_table, SLOT(showDetails(bool)));
    // sort actions
    connect(ui_actionSortDefault, SIGNAL(triggered(bool)), this, SLOT(sortDefault(bool)));
    connect(ui_actionSortByShortName, SIGNAL(triggered(bool)), this, SLOT(sortByShortName(bool)));
    connect(ui_actionSortByName, SIGNAL(triggered(bool)), this, SLOT(sortByName(bool)));
    connect(ui_actionSortByDifficulty, SIGNAL(triggered(bool)), this, SLOT(sortByDifficulty(bool)));
    connect(ui_actionSortByTimeSpent, SIGNAL(triggered(bool)), this, SLOT(sortByTimeSpent(bool)));
    connect(ui_actionSortByEstimation, SIGNAL(triggered(bool)), this, SLOT(sortByEstimation(bool)));
    connect(ui_actionSortByCompletion, SIGNAL(triggered(bool)), this, SLOT(sortByCompletion(bool)));
    // step actions
    connect(ui_stepMenu,SIGNAL(assignmentChanged(RamObject*,bool)), this, SLOT(showStep(RamObject*,bool)));
    connect(ui_actionSelectMySteps, SIGNAL(triggered()), this, SLOT(selectUserSteps()));
    // user actions
    connect(ui_userMenu,SIGNAL(assignmentChanged(RamObject*,bool)), this, SLOT(showUser(RamObject*,bool)));
    connect(ui_actionNotAssigned, SIGNAL(toggled(bool)), this, SLOT(showUnassigned(bool)));
    connect(ui_actionSelectMyself, SIGNAL(triggered()), this, SLOT(selectMyself()));
    // state actions
    connect(ui_stateMenu,SIGNAL(assignmentChanged(RamObject*,bool)), this, SLOT(showState(RamObject*,bool)));
    // comment actions
    connect(ui_copyComment, SIGNAL(triggered()), this, SLOT(copyComment()));
    connect(ui_cutComment, SIGNAL(triggered()), this, SLOT(cutComment()));
    connect(ui_pasteComment, SIGNAL(triggered()), this, SLOT(pasteComment()));
    // search
    connect(ui_searchEdit, SIGNAL(changing(QString)), ui_table, SLOT(search(QString)));
    connect(ui_searchEdit, SIGNAL(changed(QString)), ui_table, SLOT(search(QString)));
    // group filter
    connect(ui_groupBox, SIGNAL(currentObjectChanged(RamObject*)), this, SLOT(filter(RamObject*)));
    // other
    connect(ui_titleBar, &DuQFTitleBar::closeRequested, this, &ItemManagerWidget::closeRequested);
    connect(Ramses::instance(), SIGNAL(currentProjectChanged(RamProject*)), this,SLOT(projectChanged(RamProject*)));
    connect(Ramses::instance(), &Ramses::userChanged, this, &ItemManagerWidget::currentUserChanged);
    connect(ui_header, SIGNAL(sort(int,Qt::SortOrder)), ui_table->filteredModel(), SLOT(resort(int,Qt::SortOrder)));
    connect(ui_header, SIGNAL(unsort()), ui_table->filteredModel(), SLOT(unsort()));
}

void ItemManagerWidget::loadSettings()
{
    // Freeze filters to improve performance
    ui_table->filteredModel()->freeze();

    RamUser *u = Ramses::instance()->currentUser();
    if (!u) return;
    QSettings *uSettings = u->settings();

    if (m_productionType == RamStep::AssetProduction)
        uSettings->beginGroup("assetTable");
    else
        uSettings->beginGroup("shotTable");

    // View
    //ui_actionTimeTracking->setChecked( uSettings->value("showTimeTracking", true).toBool() );
    //ui_actionCompletionRatio->setChecked( uSettings->value("showCompletion", true).toBool() );
    //ui_actionShowDetails->setChecked( uSettings->value("showDetails", true).toBool() );
    // Users
    ui_userMenu->restoreState(uSettings, "users");
    // Steps
    ui_stepMenu->restoreState(uSettings, "steps");
    // States
    ui_stateMenu->restoreState(uSettings, "states");

    uSettings->endGroup();

    // Unfreeze
    ui_table->filteredModel()->unFreeze();
}

void ItemManagerWidget::changeProject()
{
    if (!m_projectChanged) return;
    m_projectChanged = false;

    // Don't show details by default (performance issue)
    ui_actionShowDetails->setChecked(false);

    // Clear step list
    QList<QAction*> actions = ui_stepMenu->actions();
    for (int i = actions.count() -1; i >= 4; i--)
    {
        actions.at(i)->deleteLater();
    }
    ui_table->filteredModel()->showAllSteps();

    // Clear user list
    actions = ui_userMenu->actions();
    for (int i = actions.count() -1; i >= 5; i--)
    {
        actions.at(i)->deleteLater();
    }
    ui_table->filteredModel()->clearUsers();



    if(!m_project)
    {
        ui_table->setObjectModel(nullptr);
        return;
    }

    // Populate list and table
    setObjectModel();

    ui_userMenu->setObjectModel( m_project->users() );
    ui_stepMenu->setObjectModel( m_project->steps() );
    ui_assignUserMenu->setObjectModel(m_project->users());
    ui_assignUserContextMenu->setObjectModel(m_project->users());

    loadSettings();
}

void ItemManagerWidget::setObjectModel()
{
    ui_table->setObjectModel( nullptr );
    ui_groupBox->setObjectModel( nullptr, "" );
    if (!m_project) return;
    if (m_productionType == RamStep::AssetProduction)
    {
        ui_table->setObjectModel( m_project->assets() );
        ui_groupBox->setObjectModel( m_project->assetGroups(), "Assets" );
    }
    else if (m_productionType == RamStep::ShotProduction)
    {
        ui_table->setObjectModel( m_project->shots() );
        ui_groupBox->setObjectModel( m_project->sequences(), "Shots" );
    }
}

void ItemManagerWidget::setupItemMenu()
{
    if (m_productionType == RamStep::AssetProduction)
    {
        QString createItemLabel = "Create new asset";
        QString deleteItemLabel = "Remove selected assets";

        ui_actionCreateItem = new QAction(QIcon(":/icons/add"), createItemLabel, this);
        ui_itemMenu->addAction(ui_actionCreateItem);

        ui_actionDeleteItem = new QAction(QIcon(":/icons/remove"), deleteItemLabel, this);
        ui_itemMenu->addAction(ui_actionDeleteItem);

        ui_itemButton = new QToolButton(this);
        ui_itemButton->setText(" Assets");
        ui_itemButton->setIcon(QIcon(":/icons/asset"));
        ui_itemButton->setMenu(ui_itemMenu);
        ui_itemButton->setIconSize(QSize(16,16));
        ui_itemButton->setObjectName("menuButton");
        ui_itemButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        ui_itemButton->setPopupMode(QToolButton::InstantPopup);
        ui_actionItem = ui_titleBar->insertLeft(ui_itemButton);
    }
    else if (m_productionType == RamStep::ShotProduction)
    {
        QString createItemLabel = "Create new shot";
        QString deleteItemLabel = "Remove selected shots";

        ui_actionCreateItem = new QAction(QIcon(":/icons/add"), createItemLabel, this);
        ui_itemMenu->addAction(ui_actionCreateItem);

        ui_actionDeleteItem = new QAction(QIcon(":/icons/remove"), deleteItemLabel, this);
        ui_itemMenu->addAction(ui_actionDeleteItem);

        ui_actionCreateMultiple = new QAction("Create multiple shots...", this);
        ui_itemMenu->addAction(ui_actionCreateMultiple);

        ui_itemButton = new QToolButton(this);
        ui_itemButton->setText(" Shots");
        ui_itemButton->setIcon(QIcon(":/icons/shot"));
        ui_itemButton->setMenu(ui_itemMenu);
        ui_itemButton->setIconSize(QSize(16,16));
        ui_itemButton->setObjectName("menuButton");
        ui_itemButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        ui_itemButton->setPopupMode(QToolButton::InstantPopup);
        ui_actionItem = ui_titleBar->insertLeft(ui_itemButton);
    }
}

void ItemManagerWidget::setupTable()
{
    ui_table->filteredModel()->setStepType(m_productionType);
    //if (m_productionType == RamStep::ShotProduction)
    ui_table->setSortable(true);
}

QVector<RamStatus *> ItemManagerWidget::beginEditSelectedStatus()
{
    QVector<RamStatus*> statuses;
    RamUser *currentUser = Ramses::instance()->currentUser();
    QModelIndexList selection = ui_table->selectionModel()->selectedIndexes();
    for (int i = 0; i < selection.count(); i++)
    {
        RamStatus *status = getStatus(selection.at(i));
        if (!status->user()->is( currentUser ))
            status = RamStatus::copy( status, currentUser );
        statuses << status;
    }
    return statuses;
}

RamStatus *ItemManagerWidget::getStatus(const QModelIndex &index)
{
    QString uuid = index.data(RamObject::UUID).toString();
    if (uuid== "") return nullptr;
    RamStatus *status = RamStatus::get(uuid);
    return status;
}
