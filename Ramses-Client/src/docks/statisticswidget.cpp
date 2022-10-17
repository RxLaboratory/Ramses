#include "statisticswidget.h"

#include "ramses.h"

StatisticsWidget::StatisticsWidget(QWidget *parent) : QWidget(parent)
{
    setupUi();
    connectEvents();
}

void StatisticsWidget::projectChanged(RamProject *project)
{
    if (m_project) disconnect(m_project, nullptr, this, nullptr);

    m_project = project;

    if (!project)
    {
        ui_userBox->setObjectModel(nullptr, "Users");
        return;
    }

    ui_userBox->setObjectModel(project->users(), "Users");

    estimationChanged(project);
    connect(m_project,SIGNAL(estimationComputed(RamProject*)),this,SLOT(estimationChanged(RamProject*)));
}

void StatisticsWidget::estimationChanged(RamProject *project)
{  
    //ui_progressWidget->setLatenessRatio( project->latenessRatio() );
    //ui_progressWidget->setTimeSpent( project->timeSpent() );
    //ui_progressWidget->setEstimation( project->estimation() );

    if (!project)
    {
        ui_progressWidget->setCompletionRatio( 0 );

        ui_remainingTimeLabel->setText( "-- days");

        ui_scheduledWorkLabel->setText( "-- days");

        ui_completionLabel->setText( "-- %" );

        ui_remainingWorkLabel->setText( "-- days" );
        return;
    }

    int remainingDays = QDate::currentDate().daysTo( project->deadline() );

    RamUser *user = RamUser::c( ui_userBox->currentObject() );
    QVector<float> stats = project->stats(user);

    float estimation = stats.at(0);
    float daysSpent = stats.at(1);
    float future = stats.at(3);
    float completion = 100;
    if (estimation > 0)
        completion = daysSpent / estimation * 100.0;

    ui_progressWidget->setCompletionRatio( completion );

    ui_remainingTimeLabel->setText( QString::number(remainingDays, 'f', 0) + " days");

    ui_scheduledWorkLabel->setText( QString::number(future, 'f', 0) + " days");

    ui_completionLabel->setText( QString::number( completion, 'f', 0 ) + " %" );

    ui_remainingWorkLabel->setText( QString::number( daysSpent, 'f', 0) + " / " + QString::number( estimation, 'f', 0 ) + " days" );

    //ui_latenessLabel->setText( QString::number( project->latenessRatio(), 'f', 0) + " %");
}

void StatisticsWidget::changeUser(RamObject *userObj)
{
    RamUser *user = RamUser::c(userObj);
    ui_statsTable->setUser(user);
    ui_statsTable->resizeRowsToContents();
    estimationChanged(m_project);
}

void StatisticsWidget::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(3);

    ui_userBox = new RamObjectComboBox(this);
    mainLayout->addWidget(ui_userBox);

    QVBoxLayout *projectLayout = new QVBoxLayout();
    projectLayout->setContentsMargins(0,0,0,0);
    projectLayout->setSpacing(3);

    QGridLayout *detailsLayout = new QGridLayout();
    detailsLayout->setContentsMargins(0,0,0,0);
    detailsLayout->setSpacing(3);

    QLabel *timeRemaining = new QLabel("Actual remaining time: ", this);
    detailsLayout->addWidget(timeRemaining, 0, 1);

    ui_remainingTimeLabel = new QLabel("-- days", this);
    detailsLayout->addWidget(ui_remainingTimeLabel, 0, 2);

    QLabel *scheduled = new QLabel("Scheduled remaining time: ", this);
    detailsLayout->addWidget(scheduled, 1, 1);

    ui_scheduledWorkLabel = new QLabel("-- days", this);
    detailsLayout->addWidget(ui_scheduledWorkLabel, 1, 2);

    QLabel *completionLabel = new QLabel("Completion: ", this);
    detailsLayout->addWidget(completionLabel, 2,1);

    ui_completionLabel = new QLabel("-- %", this);
    detailsLayout->addWidget(ui_completionLabel, 2, 2);

    ui_remainingWorkLabel = new QLabel("-- days (done) / -- days (total)");
    detailsLayout->addWidget(ui_remainingWorkLabel, 3, 2);

    //QLabel *latenessLabel = new QLabel("Estimated lateness: ", this);
    //detailsLayout->addWidget(latenessLabel, 4,1);

    //ui_latenessLabel = new QLabel("-- %", this);
    //detailsLayout->addWidget(ui_latenessLabel, 4, 2);

    detailsLayout->setColumnStretch(0, 100);
    detailsLayout->setColumnStretch(1, 0);
    detailsLayout->setColumnStretch(2, 0);
    detailsLayout->setColumnStretch(3, 100);

    projectLayout->addLayout(detailsLayout);

    ui_progressWidget = new ProgressWidget(this);
    projectLayout->addWidget(ui_progressWidget);

    mainLayout->addLayout(projectLayout);

    // Add the stats table
    ui_statsTable = new StatisticsView(this);

    mainLayout->addWidget(ui_statsTable);
}

void StatisticsWidget::connectEvents()
{
    connect(Ramses::instance(), SIGNAL(currentProjectChanged(RamProject*)), this, SLOT(projectChanged(RamProject*)));
    connect(ui_userBox, SIGNAL(currentObjectChanged(RamObject*)), this, SLOT(changeUser(RamObject*)));
}
