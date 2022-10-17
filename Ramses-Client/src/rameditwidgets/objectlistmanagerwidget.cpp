#include "objectlistmanagerwidget.h"

ObjectListManagerWidget::ObjectListManagerWidget(QString title, QIcon icon, QWidget *parent) :
    QWidget(parent)
{
    setupUi(title,icon);
    connectEvents();
    m_listEditWidget->setTitle(title);
    clear();
}

ObjectListManagerWidget::ObjectListManagerWidget(QAbstractItemModel *objectList, QString title, QIcon icon, QWidget *parent) :
    QWidget(parent)
{
    setupUi(title,icon);
    connectEvents();
    m_listEditWidget->setTitle(title);
    setList(objectList);
}

void ObjectListManagerWidget::setList(QAbstractItemModel *objectList)
{
    m_listEditWidget->setList( objectList );
    if (!objectList) return;
    this->setEnabled(true);
}

void ObjectListManagerWidget::clear()
{
    this->setEnabled(false);
    m_listEditWidget->clear();
}

void ObjectListManagerWidget::setupUi(QString title, QIcon icon)
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setContentsMargins(3,0,0,0);

    m_listEditWidget = new ObjectListEditWidget( false, RamUser::Admin, this);
    m_listEditWidget->setMaximumWidth(500);
    lay->addWidget(m_listEditWidget);

    lay->addStretch();

    // Menu
    QMenu *itemMenu = new QMenu();

    ui_createAction = new QAction("Create new", this);
    itemMenu->addAction(ui_createAction);

    ui_itemButton = new QToolButton();
    ui_itemButton->setIcon(icon);
    ui_itemButton->setText(" " + title);
    ui_itemButton->setMenu(itemMenu);
    ui_itemButton->setIconSize(QSize(16,16));
    ui_itemButton->setObjectName("menuButton");
    ui_itemButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui_itemButton->setPopupMode(QToolButton::InstantPopup);

    // Shortcuts
    ui_createShortcut = new QShortcut(QKeySequence("Shift+A"),this);
    ui_removeShortcut = new QShortcut(QKeySequence("Shift+X"),this);
}

void ObjectListManagerWidget::connectEvents()
{
    connect( m_listEditWidget, SIGNAL(add()), this, SLOT(createObject()) );
    connect( ui_createAction, SIGNAL(triggered()), this, SLOT(createEditObject()) );
    connect( ui_createShortcut, SIGNAL(activated()), this, SLOT(createObject()));
    connect( ui_removeShortcut, SIGNAL(activated()), m_listEditWidget, SLOT(removeSelectedObjects()));
}

QString ObjectListManagerWidget::currentFilterUuid() const
{
    return m_listEditWidget->currentFilterUuid();
}

RamObject *ObjectListManagerWidget::currentFilter() const
{
    return m_listEditWidget->currentFilter();
}

QToolButton *ObjectListManagerWidget::menuButton()
{
    return ui_itemButton;
}

void ObjectListManagerWidget::createEditObject()
{
    RamObject *o = createObject();
    if (o) o->edit();
}
