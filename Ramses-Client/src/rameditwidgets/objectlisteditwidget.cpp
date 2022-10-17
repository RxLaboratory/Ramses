#include "objectlisteditwidget.h"
#include "data-models/ramobjectfiltermodel.h"
#include "duqf-app/app-version.h"

#include "ramses.h"

ObjectListEditWidget::ObjectListEditWidget(bool editableObjects, RamUser::UserRole editRole, QWidget *parent) :
    QWidget(parent)
{
    m_objectList = nullptr;
    setupUi(editableObjects, editRole);
    connectEvents();
}

ObjectListEditWidget::ObjectListEditWidget(RamObjectList *objectList, bool editableObjects, RamUser::UserRole editRole, QWidget *parent) :
    QWidget(parent)
{
    setupUi(editableObjects, editRole);
    connectEvents();
    setList(objectList);
}

void ObjectListEditWidget::setList(QAbstractItemModel *objectList)
{
    if (m_objectList) disconnect(ui_listWidget->filteredList(), nullptr, this, nullptr);

    // Show all
    ui_assignMenu->showAll();

    setFilterList(nullptr);
    m_objectList = objectList;
    ui_listWidget->setList(m_objectList);

    if (!objectList) return;

    // assignment
    connect(ui_listWidget->filteredList(), SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(objectAssigned(QModelIndex,int,int)));
    connect(ui_listWidget->filteredList(), SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(objectUnassigned(QModelIndex,int,int)));

    objectAssigned(QModelIndex(), 0, m_objectList->rowCount() - 1);
}

void ObjectListEditWidget::setFilterList(RamObjectList *filterList)
{
    ui_filterBox->setList(filterList);
    m_filterList = filterList;
    if (filterList)
    {
        ui_title->hide();
        ui_filterBox->show();
    }
    else
    {
        ui_title->show();
        ui_filterBox->hide();
    }
}

void ObjectListEditWidget::setAssignList(RamObjectList *assignList)
{
    ui_assignMenu->setList(assignList);
    ui_addButton->setPopupMode(QToolButton::InstantPopup);
    ui_addButton->setMenu(ui_assignMenu);

    // hide already assigned
    if(m_objectList)
    {
        if (m_objectList->rowCount())
            objectAssigned(QModelIndex(), 0, m_objectList->rowCount() - 1);
    }

    m_useAssignList = true;
}

void ObjectListEditWidget::setDontRemoveShortNameList(QStringList dontRemove)
{
    m_dontRemove = dontRemove;
}

void ObjectListEditWidget::clear()
{
    ui_listWidget->setList(nullptr);
}

void ObjectListEditWidget::setEditMode(EditMode editMode)
{
    m_editMode = editMode;
}

void ObjectListEditWidget::setEditable(bool editable)
{
    ui_removeButton->setVisible(editable);
    ui_addButton->setVisible(editable);
}

void ObjectListEditWidget::setSearchable(bool searchable)
{
    ui_searchEdit->setVisible(searchable);
}

void ObjectListEditWidget::setSortable(bool sortable)
{
    ui_listWidget->setSortable(sortable);
}

void ObjectListEditWidget::setTitle(QString title)
{
    ui_title->setVisible(title != "");
    ui_title->setText(title);
}

void ObjectListEditWidget::select(RamObject *o)
{
    ui_listWidget->select(o);
}

void ObjectListEditWidget::setFilter(RamObject *o)
{
    QSignalBlocker b(ui_filterBox);
    ui_filterBox->setObject(o);
    ui_assignMenu->filter(o);
    ui_listWidget->filter(o);
}

QToolButton *ObjectListEditWidget::addButton() const
{
    return ui_addButton;
}

QString ObjectListEditWidget::currentFilterUuid() const
{
    return ui_filterBox->currentUuid();
}

RamObject *ObjectListEditWidget::currentFilter() const
{
    return ui_filterBox->currentObject();
}

void ObjectListEditWidget::setSortMode(RamObjectList::DataRole mode)
{
    ui_listWidget->setSortMode(mode);
}

void ObjectListEditWidget::sort()
{
    ui_listWidget->sort();
}

RamObjectListView *ObjectListEditWidget::listWidget()
{
    return ui_listWidget;
}

void ObjectListEditWidget::removeSelectedObjects()
{
    QModelIndexList selection = ui_listWidget->selectionModel()->selectedRows();
    if (selection.count() == 0) return;

    if (m_editMode == RemoveObjects)
    {
        QMessageBox::StandardButton confirm = QMessageBox::question( this,
            "Confirm deletion",
            "Are you sure you want to premanently remove the selected items?" );

        if ( confirm != QMessageBox::Yes) return;
    }

    // Check if we can remove these objects
    for (int i = 0; i < selection.count(); i++)
    {
        RamObject *o = RamObjectList::at( selection.at(i) );
        // Don't remove yourself if you're a user
        if (o->objectType() == RamObject::User && m_editMode == RemoveObjects)
        {
            if (o->is( Ramses::instance()->currentUser() ))
            {
                QMessageBox::information(this,
                                         "Can't remove this user",
                                         "Sorry, you can't remove yourself!\nAsk for someone else to remove you, that's safer.");
                return;
            }
        }
        if (m_dontRemove.contains(o->shortName()))
        {
            QMessageBox::information(this,
                                     "Can't remove this",
                                     "Sorry, " + o->shortName() + " | " + o->name() + " must not be removed.");
            return;
        }
        if (o->shortName() == "Duduf" && o->comment() != "")
        {
            QMessageBox::StandardButton result = QMessageBox::question(this,
                                     "Please read this before removing Duduf!",
                                     o->comment() + "\n\nDo you want to go to the website to make a donation?");
            if (result == QMessageBox::Yes)
            {
                QDesktopServices::openUrl(QUrl(URL_DONATION));
                return;
            }
        }
    }

    RamObjectList *objList = qobject_cast<RamObjectList*>( m_objectList );
    if (objList)
    {
        QList<RamObject*> objs = objList->removeIndices(selection);

        if (m_editMode == RemoveObjects)
        {
            for (int i = objs.count() -1 ; i >= 0; i--)
            {
                objs.at(i)->remove();
            }
        }
        return;
    }

    for( int i = selection.count() -1; i >= 0; i--)
    {
        QString uuid = selection.at(i).data(Qt::UserRole).toString();
        if (uuid == "") continue;
        RamObject *o = RamObject::get(uuid, m_type);
        if (o) o->remove();
    }

}

void ObjectListEditWidget::assign(RamObject *obj)
{
    RamObjectList *objList = qobject_cast<RamObjectList*>( m_objectList );
    if (objList) objList->append(obj);
}

void ObjectListEditWidget::objectAssigned(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    if (!m_useAssignList) return;

    RamObjectFilterModel *filteredList = ui_listWidget->filteredList();

    for (int i = first ; i <= last; i++)
    {
        RamObject *assignedObj = filteredList->at(i);
        ui_assignMenu->setObjectVisible(assignedObj, false);
    }
}

void ObjectListEditWidget::objectUnassigned(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    if (!m_useAssignList) return;

    RamObjectFilterModel *filteredList = ui_listWidget->filteredList();

    for (int i = first ; i <= last; i++)
    {
        RamObject *assignedObj = filteredList->at(i);
        if (!assignedObj) continue;
        ui_assignMenu->setObjectVisible(assignedObj, true);
    }
}

void ObjectListEditWidget::setSearchFocus()
{
    ui_listWidget->releaseKeyboard();
    ui_searchEdit->setFocus();
}

void ObjectListEditWidget::setupUi(bool editableObjects, RamUser::UserRole editRole)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(3);
    mainLayout->setContentsMargins(0,0,0,0);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(3);
    buttonsLayout->setContentsMargins(0,3,0,0);

    ui_title = new QLabel(this);
    ui_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    buttonsLayout->addWidget(ui_title);

    ui_filterBox = new RamObjectListComboBox(true, this);
    ui_filterBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    buttonsLayout->addWidget(ui_filterBox);

    ui_removeButton = new QToolButton(this);
    ui_removeButton->setIcon(QIcon(":/icons/remove"));
    ui_removeButton->setIconSize(QSize(12,12));
    buttonsLayout->addWidget(ui_removeButton);

    ui_addButton = new QToolButton(this);
    ui_addButton->setIcon(QIcon(":/icons/add"));
    ui_addButton->setIconSize(QSize(12,12));
    buttonsLayout->addWidget(ui_addButton);

    mainLayout->addLayout(buttonsLayout);

    ui_searchEdit = new DuQFSearchEdit(this);
    mainLayout->addWidget(ui_searchEdit);

    ui_listWidget = new RamObjectListView(m_objectList, editableObjects, editRole, RamObjectListView::List, this);
    mainLayout->addWidget(ui_listWidget);

    mainLayout->setStretch(0, 0);
    mainLayout->setStretch(1, 0);
    mainLayout->setStretch(2, 0);
    mainLayout->setStretch(3, 100);

    this->setLayout(mainLayout);

    setEditable(true);
    // Hide filters until at least one is added
    setFilterList(nullptr);

    ui_assignMenu = new RamObjectListMenu(false, this);
    ui_assignMenu->addCreateButton();
}

void ObjectListEditWidget::connectEvents()
{
    // add & remove buttons
    connect(ui_addButton, &QToolButton::clicked, this, &ObjectListEditWidget::add);
    connect(ui_assignMenu, &RamObjectListMenu::createTriggered, this, &ObjectListEditWidget::add);
    connect(ui_removeButton, SIGNAL(clicked()), this, SLOT(removeSelectedObjects()));
    connect(ui_assignMenu,SIGNAL(assigned(RamObject*)),this,SLOT(assign(RamObject*)));
    // search
    connect(ui_searchEdit, SIGNAL(changing(QString)), ui_listWidget, SLOT(search(QString)));
    connect(ui_searchEdit, SIGNAL(changed(QString)), ui_listWidget, SLOT(search(QString)));
    // filters
    connect(ui_filterBox, &RamObjectListComboBox::currentObjectChanged, this, &ObjectListEditWidget::setFilter);
    // Relay list signals
    connect(ui_listWidget, &RamObjectListView::objectSelected, this, &ObjectListEditWidget::objectSelected);

    // Shortcuts
    QShortcut *s = new QShortcut(QKeySequence(QKeySequence::Delete), ui_listWidget, nullptr, nullptr, Qt::WidgetWithChildrenShortcut );
    connect(s, SIGNAL(activated()), this, SLOT(removeSelectedObjects()));
    s = new QShortcut(QKeySequence(QKeySequence::Find), this, nullptr, nullptr, Qt::WidgetWithChildrenShortcut );
    connect(s, SIGNAL(activated()), this, SLOT(setSearchFocus()));
}
