#include "statisticsmodel.h"

#include "ramses.h"

StatisticsModel::StatisticsModel(QObject *parent) : QAbstractTableModel(parent)
{
    connectEvents();
}

int StatisticsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

int StatisticsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (!m_project) return 0;

    return m_project->steps()->rowCount();
}

QVariant StatisticsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!m_project) return QVariant();

    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole) return "Completion";

        return QVariant();
    }

    RamStep *step = RamStep::c( m_project->steps()->get(section) );

    if (!step) return QVariant();

    if (role == Qt::DisplayRole) return step->shortName();

    if (role == Qt::ForegroundRole)
    {
        QColor stepColor = step->color();
        if (stepColor.lightness() < 150) stepColor.setHsl( stepColor.hue(), stepColor.saturation(), 150);
        return QBrush(stepColor);
    }

    return QVariant();
}

QVariant StatisticsModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    RamStep *step = RamStep::c( m_project->steps()->get( row ) );
    if (!step) return QVariant();

    QVector<float> userStats = step->stats(m_user);
    float estimation = userStats.at(0);
    float daysSpent = userStats.at(1);
    float assigned = userStats.at(2);
    float needed = estimation - daysSpent;
    float futureUnassigned = needed - userStats.at(3);
    float unassignedDays = estimation - assigned;
    float completion = 100;
    if (estimation > 0)
        completion = daysSpent / estimation * 100;

    if (role == Qt::DisplayRole)
    {
        QString text;
        if ((step->type() == RamStep::ShotProduction || step->type() == RamStep::AssetProduction) && estimation > 0)
        {
            text = "Completion: " %
                    QString::number( completion, 'f', 0 ) % " % (" %
                    QString::number( daysSpent, 'f', 1 ) % " / " % QString::number(estimation, 'f', 1) % " days)";

            if (completion > 99.9)
            {
                text = text % "\n\nFinished!\n\n";
            }
            else if (futureUnassigned > 0.4)
            {
                text = text % "\n\nRemaining: " % QString::number( needed, 'f', 1  ) % " days\nMissing: " % QString::number( futureUnassigned, 'f', 1  ) % " days\n";
            }
            else if ( futureUnassigned < -0.4 )
            {
                text = text % "\n\nThere are " % QString::number( -futureUnassigned, 'f', 1  ) % " extra days.\n\n";
            }
            else
            {
                text = text % "\n\nSchedule seems OK\n\n";
            }

            text = text % "\nScheduled: " % QString::number( assigned, 'f', 1) % " days";

            if (unassignedDays > 0) text = text %
                                                "\nMissing: " %
                                                QString::number( unassignedDays, 'f', 1) %
                                                " days";
            else if (unassignedDays < 0) text = text %
                                                    "\nExtra: " %
                                                    QString::number( -unassignedDays, 'f', 1) %
                                                    " days";
        }
        else
        {
            text = "Scheduled: " % QString::number( assigned, 'f', 1) % " days";
        }

        return text;
    }
    if (role == Qt::ForegroundRole)
    {       
        float latenessRatio = step->latenessRatio();
        QColor timeColor;
        if ( latenessRatio < -0.05 ) timeColor = QColor(44,98,230);
        else if ( latenessRatio < -0.01 ) timeColor = QColor(50,100,186);
        else if ( latenessRatio < 0.01 ) timeColor = QColor(157,157,157);
        else if ( latenessRatio < 0.02 ) timeColor = QColor(191,177,72);
        else if ( latenessRatio < 0.05 ) timeColor = QColor(191,148,61);
        else if ( latenessRatio < 0.10 ) timeColor = QColor(186,100,50);
        else if ( latenessRatio < 0.15 ) timeColor = QColor(213,98,44);
        else if ( latenessRatio < 0.20 ) timeColor = QColor(216,62,31);
        else if ( latenessRatio < 0.25 ) timeColor = QColor(230,31,17);
        else if ( latenessRatio < 0.33 ) timeColor = QColor(244,2,2);
        else timeColor = QColor(255,0,0);
        return timeColor;
    }

    if (role == Qt::ToolTipRole) return QString( step->name() %
                                                 "\nCompletion: " %
                                                 QString::number( completion, 'f', 0) %
                                                 " %\nLateness: " %
                                                 QString::number( (step->latenessRatio() -1) * 100, 'f', 0) %
                                                 " %");

    if (role == Qt::StatusTipRole) return QString( step->shortName() % " | " % step->name() %
                                                   " | Completion: " %
                                                   QString::number( completion ) %
                                                   " % | Lateness: " %
                                                   QString::number( (step->latenessRatio() -1) * 100, 'f', 0) %
                                                   " %");

    if (role == Qt::UserRole) return completion;
    if (role == Qt::UserRole +1) return step->latenessRatio();
    if (role == Qt::UserRole +2) return estimation;
    if (role == Qt::UserRole +3) return step->timeSpent();
    if (role == Qt::UserRole +4) return assigned;
    if (role == Qt::UserRole +5) return unassignedDays;
    if (role == Qt::UserRole +6) return step->type();
    if (role == Qt::UserRole +7) return futureUnassigned;

    return QVariant();
}

void StatisticsModel::changeProject(RamProject *project)
{
    beginResetModel();

    m_project = project;

    endResetModel();

    if (!project) return;

    connect( m_project, SIGNAL(estimationComputed(RamProject*)),this,SLOT(estimationComputed()));
    connect( project->steps(), SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(insertStep(QModelIndex,int,int)));
    connect( project->steps(), SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(removeStep(QModelIndex,int,int)));
}

void StatisticsModel::removeStep(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);

    // We're removing rows
    beginRemoveRows(QModelIndex(), first, last);
    endRemoveRows();
}

void StatisticsModel::estimationComputed()
{
    emit dataChanged(createIndex(0,0), createIndex(rowCount()-1, 0));
}

void StatisticsModel::setUser(RamUser *newUser)
{
    beginResetModel();
    m_user = newUser;
    endResetModel();
}

void StatisticsModel::insertStep(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);

    //We're inserting new rows
    beginInsertRows(QModelIndex(), first, last);
    // Finished!
    endInsertRows();
}

void StatisticsModel::connectEvents()
{
    connect(Ramses::instance(), SIGNAL(currentProjectChanged(RamProject*)), this, SLOT(changeProject(RamProject*)));
}
