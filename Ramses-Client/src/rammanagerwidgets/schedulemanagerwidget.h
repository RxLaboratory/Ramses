#ifndef SCHEDULEMANAGERWIDGET_H
#define SCHEDULEMANAGERWIDGET_H

#include <QWidget>
#include <QDateEdit>
#include <QShortcut>
#include <QInputDialog>
#include <QClipboard>

#include "dbinterface.h"
#include "duqf-widgets/duqftitlebar.h"
#include "ramproject.h"
#include "ramscheduletableview.h"
#include "ramscheduletablemodel.h"
#include "ramobjectmenu.h"
#include "ramschedulefilterproxymodel.h"

class ScheduleManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScheduleManagerWidget(QWidget *parent = nullptr);

signals:
    void closeRequested();

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private slots:
    void checkUserFilter();
    void usersInserted(const QModelIndex &parent, int first, int last);

    void projectChanged(RamProject *project);
    void projectUpdated(RamObject*projObj);
    void userChanged(RamUser *user);

    void assignStep(RamObject *step = nullptr);

    void filterUser(RamObject *user, bool filter);
    void filterMe();

    void showMonday(bool show);
    void showTuesday(bool show);
    void showWednesday(bool show);
    void showThursday(bool show);
    void showFriday(bool show);
    void showSaturday(bool show);
    void showSunday(bool show);

    void goTo(QDate date);
    void updateCurrentDate();
    void goToToday();
    void goToDeadline();
    void goToNextMonth();
    void goToPreviousMonth();

    void copyComment();
    void cutComment();
    void pasteComment();

    void contextMenuRequested(QPoint p);
    void comment();
    void removeCommment();

    void color();

private:
    void setupUi();
    void connectEvents();
    void loadSettings();
    void setComment(QString comment, QModelIndex index);

    void changeProject();
    bool m_projectChanged = false;

    DuQFTitleBar *ui_titleBar;
    QAction *ui_actionShowDetails;
    RamScheduleTableView *ui_table;
    QDateEdit *ui_startDateEdit;
    QDateEdit *ui_endDateEdit;
    RamObjectMenu *ui_stepMenu;
    RamObjectMenu *ui_stepContextMenu;
    QToolButton *ui_userButton;
    RamObjectMenu *ui_userMenu;
    QAction *ui_meAction;
    QAction *ui_monday;
    QAction *ui_tuesday;
    QAction *ui_wednesday;
    QAction *ui_thursday;
    QAction *ui_friday;
    QAction *ui_saturday;
    QAction *ui_sunday;
    QToolButton *ui_today;
    QToolButton *ui_deadline;
    QToolButton *ui_nextMonth;
    QToolButton *ui_prevMonth;
    QDateEdit *ui_goTo;
    QLabel *ui_timeRemaining;
    QMenu *ui_contextMenu;
    QAction *ui_copyComment;
    QAction *ui_cutComment;
    QAction *ui_pasteComment;
    QAction *ui_commentAction;
    QAction *ui_colorAction;
    QAction *ui_removeCommentAction;
    QMenu *ui_commentContextMenu;

    RamScheduleTableModel *m_schedule;
    RamScheduleFilterProxyModel *m_scheduleFilter;

    RamProject *m_project = nullptr;

    DBInterface *m_dbi;
};

#endif // SCHEDULEMANAGERWIDGET_H
