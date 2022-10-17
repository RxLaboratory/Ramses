#ifndef ITEMMANAGERWIDGET_H
#define ITEMMANAGERWIDGET_H

#include <QWidget>
#include <QShowEvent>
#include <QHideEvent>
#include <QMenu>
#include <QComboBox>
#include <QClipboard>

#include "duqf-widgets/duqftitlebar.h"
#include "ramitemview.h"
#include "duqf-widgets/duqfsearchedit.h"

#include "ramstepheaderview.h"

#include "ramobjectcombobox.h"
#include "ramobjectmenu.h"
#include "ramstatus.h"
#include "ramstep.h"

class ItemManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ItemManagerWidget(RamStep::Type type, QWidget *parent = nullptr);

public slots:
    void selectAllSteps();
    void selectUserSteps();
    void deselectSteps();

    void selectAllUsers();
    void selectMyself();
    void deselectUsers();
    void showUnassigned(bool show);

    void selectAllStates();
    void deselectStates();

signals:
    void closeRequested();

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

    RamItemView *ui_table;
    DuQFTitleBar *ui_titleBar;
    RamObjectComboBox *ui_groupBox;

protected slots:
    virtual void projectChanged(RamProject *project, bool force = false);

private slots:
    void showUser(RamObject *user, bool s);
    void showStep(RamObject *step, bool s);
    void showState(RamObject *state, bool s);

    void checkStepFilters();
    void checkUserFilters();
    void checkStateFilters();

    // Filter
    void filter(RamObject *filterObj);

    // Sort
    void uncheckSort();
    void sortDefault(bool sort = true);
    void sortByShortName(bool sort = true);
    void sortByName(bool sort = true);
    void sortByDifficulty(bool sort = true);
    void sortByTimeSpent(bool sort = true);
    void sortByEstimation(bool sort = true);
    void sortByCompletion(bool sort = true);

    // Status
    void unassignUser();
    void assignUser(RamObject *user);
    void changeState(RamObject *stt);
    void setVeryEasy();
    void setEasy();
    void setMedium();
    void setHard();
    void setVeryHard();
    void setDiffculty(RamStatus::Difficulty difficulty);
    void setCompletion();
    void copyComment();
    void cutComment();
    void pasteComment();

    // Item
    void createItem();
    void deleteItems();
    void createMultiple();

    void contextMenuRequested(QPoint p);

    void currentUserChanged(RamUser *user);

private:
    void setupUi();
    void connectEvents();
    void loadSettings();

    void changeProject();
    bool m_projectChanged = false;

    // Specialized functions
    void setObjectModel();
    void setupItemMenu();
    void setupTable();

    DuQFSearchEdit *ui_searchEdit;
    QToolButton *ui_itemButton;
    QAction *ui_actionItem;
    QMenu *ui_itemMenu;
    QAction *ui_actionCreateItem;
    QAction *ui_actionDeleteItem;
    QAction *ui_actionCreateMultiple;
    QToolButton *ui_userButton;
    RamObjectMenu *ui_userMenu;
    QAction *ui_actionSelectMyself;
    QAction *ui_actionNotAssigned;
    QToolButton *ui_stateButton;
    RamObjectMenu *ui_stateMenu;
    QToolButton *ui_stepButton;
    RamObjectMenu *ui_stepMenu;
    QAction *ui_actionSelectMySteps ;
    //QAction *ui_actionTimeTracking ;
    //QAction *ui_actionCompletionRatio ;
    QAction *ui_actionShowDetails;
    QAction *ui_actionSortDefault;
    QAction *ui_actionSortByShortName;
    QAction *ui_actionSortByName;
    QAction *ui_actionSortByDifficulty;
    QAction *ui_actionSortByTimeSpent;
    QAction *ui_actionSortByEstimation;
    QAction *ui_actionSortByCompletion;
    RamObjectMenu *ui_assignUserMenu;
    RamObjectMenu *ui_changeStateMenu;
    QMenu *ui_changeDifficultyMenu;
    QAction *ui_veryEasy;
    QAction *ui_easy;
    QAction *ui_medium;
    QAction *ui_hard;
    QAction *ui_veryHard;
    RamStepHeaderView *ui_header;
    QAction *ui_completion0;
    QAction *ui_completion10;
    QAction *ui_completion25;
    QAction *ui_completion50;
    QAction *ui_completion75;
    QAction *ui_completion90;
    QAction *ui_completion100;
    QMenu *ui_contextMenu;
    QAction *ui_copyComment;
    QAction *ui_cutComment;
    QAction *ui_pasteComment;
    RamObjectMenu *ui_assignUserContextMenu;
    RamObjectMenu *ui_changeStateContextMenu;


    RamProject *m_project = nullptr;
    RamStep::Type m_productionType;

    // utils
    QVector<RamStatus*> beginEditSelectedStatus();
    RamStatus *getStatus(const QModelIndex &index);
};

#endif // ITEMMANAGERWIDGET_H
