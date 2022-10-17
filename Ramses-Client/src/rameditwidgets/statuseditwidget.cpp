#include "statuseditwidget.h"

#include "ramfilemetadatamanager.h"
#include "ramabstractitem.h"
#include "ramses.h"
#include "ramworkingfolder.h"

StatusEditWidget::StatusEditWidget(QWidget *parent) :
    ObjectEditWidget( parent)
{
    setupUi();
    connectEvents();
}

StatusEditWidget::StatusEditWidget(RamStatus *status, QWidget *parent) : ObjectEditWidget(parent)
{
    setupUi();
    connectEvents();
    setObject(status);
}

RamStatus *StatusEditWidget::status() const
{
    return m_status;
}

void StatusEditWidget::reInit(RamObject *o)
{
    m_status = qobject_cast<RamStatus*>(o);
    if (m_status)
    {
        QSignalBlocker b1(ui_userBox);
        QSignalBlocker b2(ui_stateBox);
        QSignalBlocker b3(ui_completionBox);
        QSignalBlocker b4(ui_versionBox);
        QSignalBlocker b5(ui_statusCommentEdit);
        QSignalBlocker b6(ui_folderWidget);
        QSignalBlocker b7(ui_publishedBox);
        QSignalBlocker b8(ui_difficultyBox);
        QSignalBlocker b9(ui_autoEstimationBox);
        QSignalBlocker b10(ui_timeSpent);

        // Get users from project
        RamProject *project = nullptr;
        if (m_status->item()) project = m_status->item()->project();
        if (!project && m_status->step()) project = m_status->step()->project();
        if (project) ui_userBox->setObjectModel( project->users(), "Users" );

        ui_userBox->setObject(m_status->assignedUser());
        ui_stateBox->setObject(m_status->state());
        ui_stateBox->update();
        ui_completionBox->setValue(m_status->completionRatio());
        ui_versionBox->setValue(m_status->version());
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    ui_statusCommentEdit->setPlainText(m_status->comment());
#else
    ui_statusCommentEdit->setMarkdown(m_status->comment());
#endif
        ui_folderWidget->setPath( m_status->path() );
        ui_publishedBox->setChecked( m_status->isPublished() );

        // Try to auto compute time spent from previous status
        qint64 timeSpent = m_status->timeSpent();
        if (!m_status->isTimeSpentManual() || timeSpent == 0)
        {
            RamObjectModel *history = m_status->item()->statusHistory( m_status->step() );
            if (history->rowCount() > 1)
            {
                RamStatus *previous = RamStatus::c( history->get( history->rowCount() -2) );
                timeSpent = previous->timeSpent();
                RamFileMetaDataManager mdm( m_status->path(RamObject::VersionsFolder ));
                if (mdm.isValid())
                {
                    timeSpent += mdm.getTimeRange( previous->date() );
                }
            }
        }

        // Get info from the files
        RamWorkingFolder statusFolder = m_status->workingFolder();

        // (try to) Auto-detect version
        int v = statusFolder.latestVersion("");
        if (v > m_status->version()) ui_versionBox->setValue(v);

        // List files
        // Working files
        ui_mainFileList->setList( statusFolder.workingFileInfos() );

        // Published versions and files
        ui_versionPublishBox->clear();
        QFileInfoList publishedVersionFolders = statusFolder.publishedVersionFolderInfos();
        for (int i = publishedVersionFolders.count()-1; i>=0; i-- )
        {
            QFileInfo folderInfo = publishedVersionFolders.at(i);
            QString title = folderInfo.fileName();
            // Let's split
            QStringList splitTitle = title.split("_");
            // Test length to know what we've got
            if (splitTitle.count() == 3) // resource, version, state
            {
                title = splitTitle[0] + " | v" + splitTitle[1] + " | " + splitTitle[2];
            }
            else if (splitTitle.count() < 3) // version (state)
            {
                if (splitTitle[0].toInt() != 0)
                    title = "v" + splitTitle.join(" | ");
            }
            else
            {
                title = splitTitle.join(" | ");
            }

            // Add date
            title = title + " | " + folderInfo.lastModified().toString(ui_mainFileList->dateFormat());

            ui_versionPublishBox->addItem( title, folderInfo.absoluteFilePath() );
        }
        ui_versionPublishBox->setCurrentIndex(0);
        loadPublishedFiles();

        // Preview files
        ui_previewFileList->setList(statusFolder.previewFileInfos());

        // List templates
        //Clear
        QList<QAction*> templateActions = ui_createFromTemplateMenu->actions();
        for (int i = 0; i < templateActions.count(); i++)
        {
            templateActions.at(i)->deleteLater();
        }
        //Add
        QSet<RamWorkingFolder> templateFolders = m_status->step()->templateWorkingFolders();

        RamNameManager nm;
        foreach(RamWorkingFolder templateFolder, templateFolders)
        {
            // Check if there's a file with the default resource
            QString file = templateFolder.defaultWorkingFile();
            if (file == "") continue;
            // enable the template button if we've found at least one file
            ui_createMainFileButton->setEnabled(true);

            nm.setFileName(file);
            QString label = nm.shortName();
            if (!templateFolder.isPublished()) label = label + " [Not published]";

            QAction *action = new QAction( label );
            action->setData( file );
            action->setToolTip( "Create from template:\n" + file );
            ui_createFromTemplateMenu->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(createFromTemplate()));
        }

        // Estimation
        ui_difficultyBox->setCurrentIndex( m_status->difficulty() );
        ui_autoEstimationBox->setChecked( m_status->useAutoEstimation() );
        setAutoEstimation( m_status->useAutoEstimation() );

        ui_timeSpent->setValue( timeSpent / 3600 );

        // User rights to assign
        ui_userBox->setEnabled(Ramses::instance()->isLead());

        // Rights to edit
        RamUser *u = Ramses::instance()->currentUser();
        if (u)
        {
            if (u->role() == RamUser::Standard) this->setEnabled( u->is( m_status->assignedUser() ) );
            else this->setEnabled(true);
            if (!m_status->assignedUser()) this->setEnabled(true);
        }
    }
    else
    {
        ui_stateBox->setObject(Ramses::instance()->stbState());
        ui_completionBox->setValue(0);
        ui_versionBox->setValue(1);
        ui_statusCommentEdit->setPlainText("");
        ui_publishedBox->setChecked(false);
        ui_userBox->setObject("");
        ui_timeSpent->setValue(0);
        ui_mainFileList->clear();
        ui_publishedFileList->clear();
        ui_previewFileList->clear();
        ui_folderWidget->setPath("");
        ui_difficultyBox->setCurrentIndex(2);
        ui_autoEstimationBox->setChecked(true);
        ui_estimationEdit->setValue(0);
        ui_versionPublishBox->clear();

        // Remove template list
        QList<QAction*> templateActions = ui_createFromTemplateMenu->actions();
        for (int i = 0; i < templateActions.count(); i++)
        {
            templateActions.at(i)->deleteLater();
        }

        this->setEnabled(false);
    }
}

void StatusEditWidget::setState(RamState *state)
{
    if (state)
    {
        ui_completionBox->setValue(state->completionRatio());
    }
    else
    {
        ui_completionBox->setValue(50);
    }
    if (!m_status) return;
    m_status->setState(state);
    m_status->setCompletionRatio(ui_completionBox->value());
}

void StatusEditWidget::refresh()
{
    setObject(m_status);
}

void StatusEditWidget::setVersion( int v )
{
    if (!m_status) return;
    bool p = m_status->workingFolder().isPublished(v);
    ui_publishedBox->setChecked(p);
    m_status->setVersion(v);
}

void StatusEditWidget::setCompletion(int c)
{
    if (!m_status) return;
    m_status->setCompletionRatio(c);
}

void StatusEditWidget::setComment()
{
    if (!m_status) return;
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    m_status->setComment( ui_statusCommentEdit->toPlainText() );
#else
    m_status->setComment( ui_statusCommentEdit->toMarkdown() );
#endif

}

void StatusEditWidget::assignUser(RamObject *u)
{
    if (!m_status) return;
    m_status->assignUser(u);
}

void StatusEditWidget::setPublished(bool p)
{
    if (!m_status) return;
    m_status->setPublished(p);
}

void StatusEditWidget::setAutoEstimation(bool a)
{
    if (!m_status) return;
    m_status->setUseAutoEstimation(a);

    float est = 0;

    if (a)
    {
        est = m_status->estimation( ui_difficultyBox->currentIndex() );
        ui_estimationLabel->setText("Estimation");
    }
    else
    {
        est = m_status->goal();
        ui_estimationLabel->setText("Goal");
    }

    ui_estimationEdit->setValue( est );
    ui_estimationEdit->setEnabled(!a);
}

void StatusEditWidget::setTimeSpent(int t)
{
    if (!m_status) return;
    m_status->setTimeSpent(t*3600);

    float days = RamStatus::hoursToDays(t);
    ui_timeSpent->setSuffix(" hours (" + QString::number(days, 'f', 2) + " days)");
}

void StatusEditWidget::setEstimation(double e)
{
    if (!m_status) return;
    m_status->setGoal(e);
}

void StatusEditWidget::setDifficulty(int d)
{
    if (!m_status) return;
    switch(d)
    {
    case 0:
    {
        m_status->setDifficulty(RamStatus::VeryEasy);
        break;
    }
    case 1:
    {
        m_status->setDifficulty( RamStatus::Easy);
        break;
    }
    case 2:
    {
        m_status->setDifficulty( RamStatus::Medium);
        break;
    }
    case 3:
    {
        m_status->setDifficulty( RamStatus::Hard);
        break;
    }
    case 4:
    {
        m_status->setDifficulty( RamStatus::VeryHard);
        break;
    }
    }

    if( !ui_autoEstimationBox->isChecked() ) return;

    float est = m_status->estimation( ui_difficultyBox->currentIndex() );
    ui_estimationEdit->setValue( est );
}

void StatusEditWidget::mainFileSelected()
{
    ui_versionFileBox->clear();

    if (!ui_mainFileList->currentItem())
    {
        ui_versionFileBox->setEnabled(false);
        ui_openMainFileButton->setEnabled(false);
        return;
    }

    ui_versionFileBox->setEnabled(true);
    ui_openMainFileButton->setEnabled(true);

    // List versions
    QString fileName = ui_mainFileList->currentFileName();
    RamNameManager nm;
    nm.setFileName(fileName);
    QString resource = nm.resource();

    RamFileMetaDataManager mdm(m_status->path(RamObject::VersionsFolder));

    ui_versionFileBox->addItem("Current version", "");

    // Use a map to automatically sort the result by title (version)
    QMap<QString, QString> files;
    foreach(QFileInfo file, m_status->workingFolder().versionFileInfos(resource))
    {
        nm.setFileName(file);
        QString title = "v" + QString::number(nm.version()) + " | " + nm.state();
        // Retrieve comment if any
        QString comment = mdm.getComment(file.fileName());
        if (comment != "") title += " | " + comment;
        title += " | " + file.lastModified().toString(ui_mainFileList->dateFormat());
        files[title] = file.absoluteFilePath();
    }

    QMapIterator<QString, QString> i(files);
    // Start by the end (most recent first)
    i.toBack();
    while(i.hasPrevious())
    {
        i.previous();
        ui_versionFileBox->addItem( i.key(), i.value() );
    }

}

void StatusEditWidget::openMainFile()
{
    if (!ui_mainFileList->currentItem()) return;

    QString filePathToOpen;

    // If current version, open the main file
    int versionIndex = ui_versionFileBox->currentIndex();
    if (versionIndex < 1)
        filePathToOpen = ui_mainFileList->currentFilePath();
    // Else, copy/rename from the versions folder
    else
        filePathToOpen = m_status->restoreVersionFile(
                    ui_versionFileBox->currentData(Qt::UserRole).toString()
                    );

    m_status->step()->openFile( filePathToOpen );

    DuQFLogger::instance()->log("Opening " + filePathToOpen + "...");

    refresh();
}

void StatusEditWidget::removeSelectedMainFile()
{
    if (!ui_mainFileList->currentItem()) return;

    QString fileName = ui_mainFileList->currentFileName();

    QMessageBox::StandardButton confirm = QMessageBox::question( this,
        "Confirm deletion",
        "Are you sure you want to delete the selected file?\n► " + fileName );
    if ( confirm != QMessageBox::Yes ) return;

    m_status->deleteFile( fileName );

    DuQFLogger::instance()->log("Deleting " + fileName + "...");

    refresh();
}

void StatusEditWidget::createFromTemplate()
{
    QAction *action = qobject_cast<QAction*>( sender() );
    QString filePath = action->data().toString();

    DuQFLogger::instance()->log("Creating " + action->text() + "...");

    QString templateFile = m_status->createFileFromResource( filePath );

    DuQFLogger::instance()->log("Opening " + templateFile + "...");

    if (templateFile != "") m_status->step()->openFile(templateFile);

    refresh();
}

void StatusEditWidget::loadPublishedFiles()
{
    ui_publishedFileList->clear();
    if (ui_versionPublishBox->currentIndex() < 0) return;
    QString versionFolder = ui_versionPublishBox->currentData(Qt::UserRole).toString();
    ui_publishedFileList->setList(RamWorkingFolder::listFileInfos(versionFolder));
}

void StatusEditWidget::publishedFileSelected()
{
    if ( ui_publishedFileList->currentItem() ) ui_openPublishedFileButton->setEnabled(true);
    else ui_openPublishedFileButton->setEnabled(false);
}

void StatusEditWidget::openPublishedFile()
{
    if (!ui_publishedFileList->currentItem()) return;

    QString filePathToOpen = ui_publishedFileList->currentFilePath();

    m_status->step()->openFile( filePathToOpen );

    DuQFLogger::instance()->log("Opening " + filePathToOpen + "...");

    refresh();
}

void StatusEditWidget::removeSelectedPublishedFile()
{
    if (!ui_publishedFileList->currentItem()) return;

    QString fileName = ui_publishedFileList->currentFileName();

    QMessageBox::StandardButton confirm = QMessageBox::question( this,
        "Confirm deletion",
        "Are you sure you want to delete the selected file?\n► " + fileName );
    if ( confirm != QMessageBox::Yes ) return;

    m_status->deleteFile( fileName, RamObject::PublishFolder );

    DuQFLogger::instance()->log("Deleting " + fileName + "...");

    refresh();
}

void StatusEditWidget::previewFileSelected()
{
    if ( ui_previewFileList->currentItem() ) ui_openPreviewFileButton->setEnabled(true);
    else ui_openPreviewFileButton->setEnabled(false);
}

void StatusEditWidget::openPreviewFile()
{
    if (!ui_previewFileList->currentItem()) return;

    QString filePathToOpen = ui_previewFileList->currentFilePath();

    // Try with the default app on the system
    QDesktopServices::openUrl(QUrl("file:///" + filePathToOpen));

    DuQFLogger::instance()->log("Opening " + filePathToOpen + "...");

    refresh();
}

void StatusEditWidget::removeSelectedPreviewFile()
{
    if (!ui_previewFileList->currentItem()) return;

    QString fileName = ui_previewFileList->currentFileName();

    QMessageBox::StandardButton confirm = QMessageBox::question( this,
        "Confirm deletion",
        "Are you sure you want to delete the selected file?\n► " + fileName );
    if ( confirm != QMessageBox::Yes ) return;

    m_status->deleteFile( fileName, RamObject::PreviewFolder );

    DuQFLogger::instance()->log("Deleting " + fileName + "...");

    refresh();
}

void StatusEditWidget::setupUi()
{
    //this->setWidgetResizable(true);

    this->hideName();
    ui_commentEdit->hide();
    ui_commentLabel->hide();

    QHBoxLayout *cLayout = new QHBoxLayout();
    cLayout->setContentsMargins(0,0,0,0);
    cLayout->setSpacing(3);

    ui_stateBox = new RamStateBox(this);
    cLayout->addWidget(ui_stateBox);

    ui_completionBox = new DuQFSpinBox(this);
    ui_completionBox->setMaximum(100);
    ui_completionBox->setMinimum(0);
    ui_completionBox->setSuffix("%");
    ui_completionBox->setValue(50);
    ui_completionBox->setFixedHeight(ui_stateBox->height());
    cLayout->addWidget(ui_completionBox);

    cLayout->setStretch(0,0);
    cLayout->setStretch(1,100);

    ui_mainLayout->addLayout(cLayout);

    ui_commentSplitter = new QSplitter();
    ui_commentSplitter->setOrientation(Qt::Vertical);

    ui_statusCommentEdit = new DuQFTextEdit(this);
    ui_statusCommentEdit->setPlaceholderText("Comment...");
    ui_statusCommentEdit->setMinimumHeight(30);
    ui_statusCommentEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui_commentEdit->setObjectName("commentEdit");

    ui_commentSplitter->addWidget( ui_statusCommentEdit );

    QWidget *bottomWidget = new QWidget(this);
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottomWidget);
    bottomLayout->setSpacing(3);
    bottomLayout->setContentsMargins(0,0,0,0);

    QFormLayout *detailsLayout = new QFormLayout();
    detailsLayout->setSpacing(3);
    detailsLayout->setContentsMargins(0,0,0,0);

    ui_difficultyBox = new QComboBox(this);
    ui_difficultyBox->addItem("Very easy");
    ui_difficultyBox->addItem("Easy");
    ui_difficultyBox->addItem("Medium");
    ui_difficultyBox->addItem("Hard");
    ui_difficultyBox->addItem("Very hard");
    detailsLayout->addRow("Difficulty", ui_difficultyBox);

    QWidget *estimationWidget = new QWidget(this);
    QHBoxLayout *estimationLayout = new QHBoxLayout(estimationWidget);
    estimationLayout->setContentsMargins(0,0,0,0);
    estimationLayout->setSpacing(3);

    ui_estimationEdit = new AutoSelectDoubleSpinBox(this);
    ui_estimationEdit->setMinimum(-1);
    ui_estimationEdit->setMaximum(999);
    ui_estimationEdit->setSuffix(" days");
    ui_estimationEdit->setEnabled(false);
    estimationLayout->addWidget( ui_estimationEdit );

    ui_autoEstimationBox = new QCheckBox("Auto", this);
    ui_autoEstimationBox->setChecked(true);
    estimationLayout->addWidget( ui_autoEstimationBox );

    estimationLayout->setStretch(0,1);
    estimationLayout->setStretch(1,0);

    ui_estimationLabel = new QLabel("Estimation", this);
    detailsLayout->addRow(ui_estimationLabel, estimationWidget);

    ui_timeSpent = new AutoSelectSpinBox(this);
    ui_timeSpent->setMinimum(0);
    ui_timeSpent->setMaximum(9999);
    ui_timeSpent->setSuffix(" hours (0 days)");
    detailsLayout->addRow("Time spent", ui_timeSpent);

    ui_versionBox = new AutoSelectSpinBox(this);
    ui_versionBox->setMaximum(1000);
    ui_versionBox->setValue(1);
    ui_versionBox->setPrefix("v");
    detailsLayout->addRow("Version", ui_versionBox);

    ui_publishedBox = new QCheckBox("Published",this);
    detailsLayout->addRow("Publication", ui_publishedBox);

    ui_userBox = new RamObjectComboBox(this);
    detailsLayout->addRow("Assigned user", ui_userBox);

    bottomLayout->addLayout( detailsLayout);

    QTabWidget *tabWidget = new QTabWidget(this);

    QWidget *mainFilesWidget = new QWidget(tabWidget);
    QVBoxLayout *mainFileLayout = new QVBoxLayout(mainFilesWidget);
    mainFileLayout->setContentsMargins(0,0,0,0);
    mainFileLayout->setSpacing(3);

    QSettings settings;
    QString dateFormat = settings.value("ramses/dateFormat", "yyyy-MM-dd hh:mm:ss").toString();

    ui_mainFileList = new DuQFFileList(this);
    ui_mainFileList->setDateFormat(dateFormat);
    ui_mainFileList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainFileLayout->addWidget(ui_mainFileList);

    QHBoxLayout *mainFileButtonLayout = new QHBoxLayout();
    mainFileButtonLayout->setContentsMargins(0,0,0,0);
    mainFileButtonLayout->setSpacing(3);
    mainFileLayout->addLayout(mainFileButtonLayout);

    ui_versionFileBox = new QComboBox(this);
    ui_versionFileBox->setEnabled(false);
    mainFileButtonLayout->addWidget(ui_versionFileBox);

    ui_createMainFileButton = new QToolButton(this);
    ui_createMainFileButton->setText("Create");
    ui_createMainFileButton->setToolTip("Create\nCreate a new file from the corresponding step template.");
    ui_createMainFileButton->setStatusTip("Create a new file from template.");
    ui_createMainFileButton->setIcon(QIcon(":/icons/create"));
    ui_createMainFileButton->setEnabled(false);
    mainFileButtonLayout->addWidget(ui_createMainFileButton);

    ui_createFromTemplateMenu = new QMenu(this);
    ui_createMainFileButton->setMenu(ui_createFromTemplateMenu);
    ui_createMainFileButton->setPopupMode(QToolButton::InstantPopup);

    ui_openMainFileButton = new QToolButton(this);
    ui_openMainFileButton->setText("Open");
    ui_openMainFileButton->setToolTip("Open\nOpen the file.");
    ui_openMainFileButton->setStatusTip("Open the file.");
    ui_openMainFileButton->setIcon(QIcon(":/icons/open"));
    ui_openMainFileButton->setEnabled(false);
    mainFileButtonLayout->addWidget(ui_openMainFileButton);

    tabWidget->addTab(mainFilesWidget, QIcon(":/icons/files"), "Work");

    QWidget *publishedFilesWidget = new QWidget(tabWidget);
    QVBoxLayout *publishedFileLayout = new QVBoxLayout(publishedFilesWidget);
    publishedFileLayout->setContentsMargins(0,3,0,0);
    publishedFileLayout->setSpacing(3);

    ui_versionPublishBox = new QComboBox(this);
    publishedFileLayout->addWidget(ui_versionPublishBox);

    ui_publishedFileList = new DuQFFileList(this);
    ui_publishedFileList->setDateFormat(dateFormat);
    ui_publishedFileList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    publishedFileLayout->addWidget(ui_publishedFileList);

    QHBoxLayout *publishedFileButtonLayout = new QHBoxLayout();
    publishedFileButtonLayout->setContentsMargins(0,0,0,0);
    publishedFileButtonLayout->setSpacing(3);
    publishedFileLayout->addLayout(publishedFileButtonLayout);
    publishedFileButtonLayout->addStretch();

    ui_openPublishedFileButton = new QToolButton(this);
    ui_openPublishedFileButton->setText("Open");
    ui_openPublishedFileButton->setToolTip("Open\nOpen the published file.");
    ui_openPublishedFileButton->setStatusTip("Open the file.");
    ui_openPublishedFileButton->setIcon(QIcon(":/icons/open"));
    ui_openPublishedFileButton->setEnabled(false);
    publishedFileButtonLayout->addWidget(ui_openPublishedFileButton);

    tabWidget->addTab(publishedFilesWidget, QIcon(":/icons/files"), "Published");

    QWidget *previewFilesWidget = new QWidget(tabWidget);
    QVBoxLayout *previewFileLayout = new QVBoxLayout(previewFilesWidget);
    previewFileLayout->setContentsMargins(0,0,0,0);
    previewFileLayout->setSpacing(3);

    ui_previewFileList = new DuQFFileList(this);
    ui_previewFileList->setDateFormat(dateFormat);
    ui_previewFileList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    previewFileLayout->addWidget(ui_previewFileList);

    QHBoxLayout *previewFileButtonLayout = new QHBoxLayout();
    previewFileButtonLayout->setContentsMargins(0,0,0,0);
    previewFileButtonLayout->setSpacing(3);
    previewFileLayout->addLayout(previewFileButtonLayout);
    previewFileButtonLayout->addStretch();

    ui_openPreviewFileButton = new QToolButton(this);
    ui_openPreviewFileButton->setText("Open");
    ui_openPreviewFileButton->setToolTip("Open\nOpen the preview file.");
    ui_openPreviewFileButton->setStatusTip("Open the file.");
    ui_openPreviewFileButton->setIcon(QIcon(":/icons/open"));
    ui_openPreviewFileButton->setEnabled(false);
    previewFileButtonLayout->addWidget(ui_openPreviewFileButton);

    tabWidget->addTab(previewFilesWidget, QIcon(":/icons/files"), "Preview");

    bottomLayout->addWidget(tabWidget);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(3);
    buttonsLayout->setContentsMargins(0,0,0,0);

    ui_folderWidget = new DuQFFolderDisplayWidget(this);
    buttonsLayout->addWidget(ui_folderWidget);

    ui_revertButton = new QToolButton(this);
    ui_revertButton->setText("Reload");
    ui_revertButton->setToolTip("Reloads the list of available files.");
    ui_revertButton->setStatusTip("Reload the list of available files.");
    ui_revertButton->setIcon(QIcon(":/icons/undo"));
    buttonsLayout->addWidget(ui_revertButton);

    bottomLayout->addLayout(buttonsLayout);

    ui_commentSplitter->addWidget(bottomWidget);

    ui_mainLayout->addWidget( ui_commentSplitter );
}

void StatusEditWidget::connectEvents()
{
    connect(ui_stateBox, &RamStateBox::currentStateChanged, this, &StatusEditWidget::setState);
    connect(ui_revertButton, &QToolButton::clicked, this, &StatusEditWidget::refresh);
    connect(ui_versionBox, SIGNAL(valueChanged(int)), this, SLOT(setVersion(int)));
    connect( ui_completionBox, SIGNAL(valueChanged(int)), this, SLOT(setCompletion(int)));
    connect( ui_statusCommentEdit, SIGNAL(editingFinished()), this, SLOT(setComment()));
    connect( ui_userBox, &RamObjectComboBox::currentObjectChanged, this, &StatusEditWidget::assignUser);
    connect( ui_publishedBox, SIGNAL(clicked(bool)), this, SLOT(setPublished(bool)));
    connect( ui_autoEstimationBox, SIGNAL(clicked(bool)), this, SLOT(setAutoEstimation(bool)));
    connect( ui_timeSpent, SIGNAL(valueChanged(int)), this, SLOT(setTimeSpent(int)));
    connect( ui_estimationEdit, SIGNAL(valueChanged(double)), this, SLOT(setEstimation(double)));
    connect( ui_difficultyBox, SIGNAL(activated(int)), this, SLOT(setDifficulty(int)));

    connect(ui_mainFileList, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this, SLOT(mainFileSelected()));
    connect(ui_mainFileList,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(openMainFile()));
    connect(ui_openMainFileButton, SIGNAL(clicked()),this,SLOT(openMainFile()));

    connect(ui_versionPublishBox, SIGNAL(currentIndexChanged(int)), this, SLOT(loadPublishedFiles()));
    connect(ui_publishedFileList, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this, SLOT(publishedFileSelected()));
    connect(ui_previewFileList, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this, SLOT(previewFileSelected()));
    connect(ui_openPublishedFileButton, SIGNAL(clicked()),this,SLOT(openPublishedFile()));
    connect(ui_openPreviewFileButton, SIGNAL(clicked()),this,SLOT(openPreviewFile()));
    connect(ui_publishedFileList,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(openPublishedFile()));
    connect(ui_previewFileList,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(openPreviewFile()));

    // Shortcuts
    QShortcut *s;
    s = new QShortcut(QKeySequence(QKeySequence::Delete), ui_mainFileList, nullptr, nullptr, Qt::WidgetWithChildrenShortcut );
    connect( s, SIGNAL(activated()), this, SLOT(removeSelectedMainFile()) );
    s = new QShortcut(QKeySequence(QKeySequence::Delete), ui_publishedFileList, nullptr, nullptr, Qt::WidgetWithChildrenShortcut );
    connect( s, SIGNAL(activated()), this, SLOT(removeSelectedPublishedFile()) );
    s = new QShortcut(QKeySequence(QKeySequence::Delete), ui_previewFileList, nullptr, nullptr, Qt::WidgetWithChildrenShortcut );
    connect( s, SIGNAL(activated()), this, SLOT(removeSelectedPreviewFile()) );
}
