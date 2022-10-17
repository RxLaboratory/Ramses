#include "projecteditwidget.h"

#include "ramproject.h"
#include "ramses.h"

ProjectEditWidget::ProjectEditWidget(QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();
}

ProjectEditWidget::ProjectEditWidget(RamProject *project, QWidget *parent) :
    ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();

    setObject(project);
}

RamProject *ProjectEditWidget::project() const
{
    return m_project;
}

void ProjectEditWidget::reInit(RamObject *o)
{
    m_project = RamProject::c(o);
    if (m_project)
    {
        ui_resolutionWidget->setHeight(m_project->height());
        ui_resolutionWidget->setWidth(m_project->width());
        ui_framerateWidget->setFramerate(m_project->framerate());
        ui_deadlineEdit->setDate( m_project->deadline() );

        ui_folderSelector->setPath( m_project->path() );
        ui_folderSelector->setPlaceHolderText( m_project->defaultPath() );
        QString p = m_project->path();
        if (p.count() > 45) p = p.replace(0, p.count()-45, "(...)");
        ui_folderLabel->setText( p );
        ui_userList->setObjectModel(m_project->users());
    }
    else
    {
        //Reset values
        ui_resolutionWidget->setHeight(1080);
        ui_resolutionWidget->setWidth(1920);
        ui_framerateWidget->setFramerate(24.0);
        ui_folderSelector->setPath("");
        ui_folderSelector->setPlaceHolderText("Default (Ramses/Projects/Project_ShortName)");
        ui_deadlineEdit->setDate( QDate::currentDate().addDays(30) );
        ui_userList->clear();
    }
}

void ProjectEditWidget::setResolution(int w, int h)
{
    if(!m_project) return;
    m_project->setWidth(w);
    m_project->setHeight(h);
}

void ProjectEditWidget::setFramerate(double f)
{
    if (!m_project) return;
    m_project->setFramerate(f);
}

void ProjectEditWidget::updateFolderLabel(QString path)
{
    if (path != "") ui_folderLabel->setText( Ramses::instance()->pathFromRamses(path));
    else if (m_project) ui_folderLabel->setText( m_project->defaultPath() );
}

void ProjectEditWidget::setPath(QString p)
{
    if (!m_project) return;
    m_project->setFolderPath(p);
}

void ProjectEditWidget::setDeadline(QDate d)
{
    if (!m_project) return;
    m_project->setDeadline(d);
}

void ProjectEditWidget::currentUserChanged(RamUser *user)
{
    if (!user) return;
    QSettings settings;
    ui_deadlineEdit->setDisplayFormat( settings.value("appearance/dateFormat","yyyy-MM-dd hh:mm:ss").toString());
}

void ProjectEditWidget::createUser()
{
    if (!m_project) return;
    RamUser *user = new RamUser(
                "NEW",
                "John Doe");
    //Ramses::instance()->users()->append(user);
    m_project->users()->appendObject(user->uuid());
    user->edit();
}

void ProjectEditWidget::savePath()
{
    if (!m_project) return;

    QMessageBox::StandardButton confirm = QMessageBox::question( this,
        "Confirm overwrite",
        "This will overwrite the default path for this project, for all users.\nDo you want to continue?" );

    if ( confirm != QMessageBox::Yes) return;

    m_project->updatePath();
}

void ProjectEditWidget::reinitPath()
{
    QSignalBlocker b(m_project);
    m_project->resetDbFolderPath();
    ui_folderSelector->setPath( m_project->path() );
    ui_folderSelector->setPlaceHolderText( m_project->defaultPath() );
    ui_folderLabel->setText( m_project->path() );
}

void ProjectEditWidget::setupUi()
{
    QLabel *rLabel = new QLabel("Resolution", this);
    //rLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui_mainFormLayout->addWidget(rLabel, 3, 0);

    ui_resolutionWidget = new ResolutionWidget(this);
    ui_mainFormLayout->addWidget(ui_resolutionWidget, 3, 1);

    QLabel *frLabel = new QLabel("Framerate", this);
    //frLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui_mainFormLayout->addWidget(frLabel, 4, 0);

    ui_framerateWidget = new FramerateWidget(this);
    ui_mainFormLayout->addWidget(ui_framerateWidget, 4, 1);

    ui_deadlineEdit = new QDateEdit(this);
    ui_deadlineEdit->setCalendarPopup(true);
    ui_deadlineEdit->setDate( QDate::currentDate() );
    ui_mainFormLayout->addWidget(new QLabel("Deadline"), 5,0);
    ui_mainFormLayout->addWidget(ui_deadlineEdit, 5, 1);

    QLabel *fLabel = new QLabel("Folder", this);
    ui_mainFormLayout->addWidget(fLabel, 6, 0);

    ui_folderSelector = new DuQFFolderSelectorWidget(DuQFFolderSelectorWidget::Folder, this);
    ui_folderSelector->setPlaceHolderText("Default (Ramses/Users/User_ShortName)");
    ui_mainFormLayout->addWidget(ui_folderSelector,6, 1);

    QWidget *fWidget = new QWidget();
    fWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout *fLayout = new QHBoxLayout(fWidget);
    fLayout->setSpacing(3);
    fLayout->setContentsMargins(0,0,0,0);

    ui_folderLabel = new QLabel("", this);
    fLayout->addWidget(ui_folderLabel);

    ui_savePathButton = new QToolButton(this);
    ui_savePathButton->setToolTip("Save path as default");
    ui_savePathButton->setIcon(QIcon(":/icons/save"));
    ui_savePathButton->setIconSize(QSize(16,16));
    fLayout->addWidget(ui_savePathButton);

    ui_reinitPathButton = new QToolButton(this);
    ui_reinitPathButton->setToolTip("Restore default path");
    ui_reinitPathButton->setIcon(QIcon(":/icons/reinit"));
    ui_reinitPathButton->setIconSize(QSize(16,16));
    fLayout->addWidget(ui_reinitPathButton);

    ui_mainFormLayout->addWidget(fWidget, 7, 1);

    ui_userList = new ObjectListWidget(true, RamUser::ProjectAdmin, this);
    ui_userList->setEditMode(ObjectListWidget::UnassignObjects);
    ui_userList->setTitle("Users");
    ui_userList->setAssignList(Ramses::instance()->users());
    ui_userList->setSortable(true);
    ui_mainLayout->addWidget(ui_userList);
}

void ProjectEditWidget::connectEvents()
{
    connect(Ramses::instance(), &Ramses::userChanged, this, &ProjectEditWidget::currentUserChanged);
    connect(ui_resolutionWidget, SIGNAL(resolutionChanged(int,int)), this, SLOT(setResolution(int,int)));
    connect(ui_framerateWidget, SIGNAL(framerateChanged(double)), this, SLOT(setFramerate(double)));
    connect(ui_folderSelector, SIGNAL(pathChanging(QString)), this, SLOT(updateFolderLabel(QString)));
    connect(ui_folderSelector, SIGNAL(pathChanged(QString)), this, SLOT(setPath(QString)));
    connect(ui_deadlineEdit, SIGNAL(dateChanged(QDate)), this, SLOT(setDeadline(QDate)));
    connect(ui_userList, SIGNAL(add()), this, SLOT(createUser()));
    connect(ui_savePathButton, SIGNAL(clicked()), this, SLOT(savePath()));
    connect(ui_reinitPathButton, SIGNAL(clicked()), this, SLOT(reinitPath()));
}
