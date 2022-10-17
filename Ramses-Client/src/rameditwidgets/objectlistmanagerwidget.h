#ifndef OBJECTLISTMANAGERWIDGET_H
#define OBJECTLISTMANAGERWIDGET_H

#include <QWidget>
#include <QSplitter>

#include "objectlisteditwidget.h"

class ObjectListManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ObjectListManagerWidget(QString title, QIcon icon = QIcon(), QWidget *parent = nullptr);
    explicit ObjectListManagerWidget(QAbstractItemModel *objectList, QString title, QIcon icon = QIcon(), QWidget *parent = nullptr);
    void setList(QAbstractItemModel *objectList);
    void clear();
    QString currentFilterUuid() const;
    RamObject *currentFilter() const;

    QToolButton *menuButton();

protected:
    ObjectListEditWidget *m_listEditWidget;

protected slots:
    virtual RamObject *createObject() { return nullptr; };
    void createEditObject();

private:
    void setupUi(QString title, QIcon icon);
    void connectEvents();

    QToolButton *ui_itemButton;
    QAction *ui_createAction;

    QShortcut *ui_createShortcut;
    QShortcut *ui_removeShortcut;
};

#endif // OBJECTLISTMANAGERWIDGET_H
