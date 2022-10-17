#include "ramscheduletablemodel.h"

#include "ramschedulecomment.h"
#include "ramscheduleentry.h"
#include "ramses.h"

RamScheduleTableModel::RamScheduleTableModel(QObject *parent) : QAbstractTableModel(parent)
{
    m_startDate = QDate::currentDate().addDays(-5);
    m_endDate = QDate::currentDate();
}

void RamScheduleTableModel::setObjectModel(RamObjectModel *userList, RamObjectModel *comments)
{
    beginResetModel();

    if (m_users) disconnect(m_users, nullptr, this, nullptr);

    m_users = userList;
    m_comments = comments;

    if (m_users)
    {
        connect( m_users, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(insertUser(QModelIndex,int,int)));
        connect( m_users, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(removeUser(QModelIndex,int,int)));
        connect( m_users, SIGNAL(modelReset()), this, SLOT(resetUsers()));
    }

    endResetModel();
}

int RamScheduleTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if (!m_users) return 0;

    return m_startDate.daysTo( m_endDate ) + 1;
}

int RamScheduleTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int c = 0;

    if (!m_users && !m_comments) return c;

    // Two rows per user: AM and PM
    if (m_users) c = m_users->rowCount()*2;
    // If there are comments, it's on an additionnal row
    if (m_comments) c++;

    return c;
}

QVariant RamScheduleTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!m_users) return QVariant();

    if (orientation == Qt::Vertical)
    {
        // Invalid index
        if (section < 0 || section > m_users->rowCount()*2)
        {
            //if (role == Qt::DisplayRole) return "Invalid";
            return QVariant();
        }

        // General
        if (role == Qt::TextAlignmentRole) return Qt::AlignCenter;

        // The comments
        if (section == 0 && m_comments) {
            if ( role == Qt::DisplayRole )
                return "NOTES";
            if ( role == Qt::ToolTipRole )
                return QString("Use this line to add comments for specific dates.");
            if ( role == RamObject::IsPM )
                return false;
            return QAbstractTableModel::headerData(section, orientation, role);
        }

        if (m_comments) section--;
        RamObject *usrObj = m_users->get(section / 2);

        // Add AMP/PM Info to user data

        QString ampm = "AM";
        if (section % 2 == 1) ampm = "PM";

        if ( role == RamObject::IsPM)
            return section % 2 == 1;

        if ( role == Qt::DisplayRole )
            return QString(usrObj->shortName() % "\n" % ampm);

        if ( role == Qt::ToolTipRole )
            return QString(usrObj->roleData(role).toString() % " | " % ampm);

        if ( role == Qt::StatusTipRole )
            return QString(usrObj->roleData(role).toString() % " | " % ampm);

        // Or return default

        return usrObj->roleData(role);
    }
    else
    {
        QDate date = m_startDate.addDays(section);

        if ( role == Qt::DisplayRole )
        {
            return QString(date.toString("MMMM\ndddd\nyyyy-MM-dd"));
        }

        if (role == Qt::ForegroundRole)
        {
            if (date == QDate::currentDate())
                return QBrush(QColor(213,136,241));

            RamProject *proj = Ramses::instance()->currentProject();
            if (proj)
            {
                if (date == proj->deadline()) return QBrush(QColor(249,105,105));
            }

            if (date.weekNumber() % 2 == 1)
                return QBrush(QColor(130,130,130));
            else
                return QBrush(QColor(170,170,170));
        }

        if (role ==  RamObject::Date)
            return date;
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant RamScheduleTableModel::data(const QModelIndex &index, int role) const
{
    if (!m_users) return QVariant();

    int row = index.row();
    int col = index.column();

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QDateTime date = QDateTime( m_startDate.addDays(col) );
#else
    QDateTime date = QDate( m_startDate.addDays(col) ).startOfDay();
#endif

    // THE DATE
    QString ampm = "am";
    if ( headerData(row, Qt::Vertical,  RamObject::IsPM).toBool() )
    {
        ampm = "pm";
        date.setTime(QTime(12,0));
    }

    if (role ==  RamObject::Date ) {
        return date;
    }
    // THE COMMENT
    if (row == 0 && m_comments)
    {
        for(int i = 0; i < m_comments->rowCount(); i++)
        {
            RamScheduleComment *c = RamScheduleComment::c(m_comments->get(i));
            if (!c) continue;
            if (c->date() == date)
                return c->roleData(role);
        }

        // Needed Default values

        if (role == Qt::BackgroundRole )
        {
            if (date.date().weekNumber() % 2 == 1)
                return QColor(51,51,51);
            else
                return QColor(42,42,42);
        }

        if (role == RamObject::IsComment) return true;

        return QVariant();
    }
    // THE ENTRY
    RamObject *usrObj = m_users->get((row-1) / 2);
    RamUser *user = RamUser::c( usrObj );
    if (!user) return QVariant();
    RamObjectModel *schedule = user->schedule();

    RamProject *currentProject = Ramses::instance()->currentProject();

    for (int i = 0; i < schedule->rowCount(); i++)
    {
        RamScheduleEntry *entry = RamScheduleEntry::c(schedule->get(i));
        if (!entry) continue;
        RamStep *entryStep = entry->step();
        if (!entryStep) continue;
        RamProject *entryProj = entryStep->project();
        if (!entryProj) continue;
        // For current project only
        if (!entryProj->is(currentProject)) continue;

        if (entry->date() == date)
        {
            // Add AMP/PM Info to entry data
            if ( role == RamObject::IsPM)
                return ampm == "pm";

            if ( role == Qt::ToolTipRole )
                return QString(entry->roleData(role).toString() % "\n" % ampm);

            if ( role == Qt::StatusTipRole )
                return QString(entry->roleData(role).toString() % " | " % ampm);

            // Or return default
            return entry->roleData(role);
        }//*/
    }

    if (role == Qt::EditRole) {
        return "";
    }

    if (role == Qt::BackgroundRole )
    {
        if (date.date().weekNumber() % 2 == 1)
            return QBrush(QColor(51,51,51));
        else
            return QBrush(QColor(42,42,42));
    }

    return QVariant();
}

void RamScheduleTableModel::insertUser(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);

    if (!m_users) return;

    //We're inserting new rows
    beginInsertRows(QModelIndex(), first*2, last*2+1);

    // Finished!
    endInsertRows();
}

void RamScheduleTableModel::removeUser(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);

    if (!m_users) return;

    // We're removing rows
    beginRemoveRows(QModelIndex(), first*2, last*2+1);

    endRemoveRows();
}

void RamScheduleTableModel::resetUsers()
{
    beginResetModel();
    endResetModel();
}

void RamScheduleTableModel::setEndDate(const QDate &newEndDate)
{
    if (newEndDate < m_startDate) return;

    if (newEndDate > m_endDate)
    {
        beginInsertColumns(QModelIndex(), columnCount(), columnCount() + m_endDate.daysTo(newEndDate) -1);
        m_endDate = newEndDate;
        endInsertColumns();
    }
    else if (newEndDate < m_endDate)
    {
        beginRemoveColumns(QModelIndex(), columnCount() - newEndDate.daysTo(m_endDate) , columnCount() -1);
        m_endDate = newEndDate;
        endRemoveColumns();
    }
}

void RamScheduleTableModel::setStartDate(const QDate &newStartDate)
{
    if (newStartDate > m_endDate) return;

    if (newStartDate < m_startDate)
    {
        beginInsertColumns(QModelIndex(), 0, newStartDate.daysTo(m_startDate)-1);
        m_startDate = newStartDate;
        endInsertColumns();
    }
    else if (newStartDate > m_startDate)
    {
        beginRemoveColumns(QModelIndex(), 0, m_startDate.daysTo(newStartDate) -1);
        m_startDate = newStartDate;
        endRemoveColumns();
    }
}

