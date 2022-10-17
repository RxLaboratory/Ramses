#ifndef OBJECTMANAGERWIDGET_H
#define OBJECTMANAGERWIDGET_H

#include <QWidget>
#include <QSplitter>

#include "objectlistwidget.h"

class ObjectManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ObjectManagerWidget(QString title, QIcon icon = QIcon(), QWidget *parent = nullptr);
    explicit ObjectManagerWidget(RamObjectModel *objectModel, QString title, QIcon icon = QIcon(), QWidget *parent = nullptr);
    void setObjectModel(RamObjectModel *objectModel);
    void clear();
    QString currentFilterUuid() const;
    RamObject *currentFilter() const;

    QToolButton *menuButton();

protected:
    ObjectListWidget *m_listWidget;

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

#endif // OBJECTMANAGERWIDGET_H
