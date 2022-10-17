#include "objecteditwidget.h"
#include "duqf-utils/utils.h"
#include "objectupdateblocker.h"

ObjectEditWidget::ObjectEditWidget(QWidget *parent) :
    QScrollArea(parent)
{
    setupUi();

    m_object = nullptr;

    connectEvents();
}

RamObject *ObjectEditWidget::object() const
{
    return m_object;
}

void ObjectEditWidget::hideName(bool hide)
{
    ui_nameLabel->setVisible(!hide);
    ui_nameEdit->setVisible(!hide);
    ui_shortNameLabel->setVisible(!hide);
    ui_shortNameEdit->setVisible(!hide);
    ui_lockShortNameButton->setVisible(!hide);
}

void ObjectEditWidget::setObject(RamObject *object)
{
    // disconnect
    if (m_object) disconnect(m_object, nullptr, this, nullptr);
    m_object = object;

    ObjectUpdateBlocker b(object);

    if (object) {
        ui_nameEdit->setText(object->name());
        ui_shortNameEdit->setText(object->shortName());
        ui_commentEdit->setText(object->comment());

        this->setEnabled(object->canEdit());
    }
    else {
        ui_nameEdit->setText("");
        ui_shortNameEdit->setText("");
        ui_commentEdit->setText("");

        this->setEnabled(false);
    }

    reInit(object);

    if (object) {
        checkPath();

        connect( object, &RamObject::removed, this, &ObjectEditWidget::objectRemoved);
        connect( object, &RamObject::dataChanged, this, &ObjectEditWidget::objectChanged);
    }
}

void ObjectEditWidget::lockShortName(bool lock)
{
    ui_shortNameEdit->setEnabled(!lock);
    ui_lockShortNameButton->setVisible(lock);
}

void ObjectEditWidget::setShortName()
{
    if (!m_object) return;

    if (ui_shortNameEdit->text() == "")
    {
        // bug in Qt, signal is fired twice when showing the message box
        if (!ui_shortNameEdit->isModified()) return;
        ui_shortNameEdit->setModified(false);

        QMessageBox::warning(this, "Missing ID", "You need to set an ID for this item." );
        ui_shortNameEdit->setText(m_object->shortName());
        ui_shortNameEdit->setFocus(Qt::OtherFocusReason);

        return;
    }

    if (!m_object->validateShortName(ui_shortNameEdit->text()))
    {
        QMessageBox::warning(this, "Invalid ID", "Sorry, this ID is invalid, please choose another one.\n\n"
                                                 "⬣ IDs should be unique\n"
                                                 "⬣ IDs must contain only: \"A-Z\", \"a-z\", \"0-9\", \"-\", \"+\" characters\n"
                                                 "⬣ IDs must be less than 10-character long" );
        ui_shortNameEdit->setText(m_object->shortName());
        ui_shortNameEdit->setFocus(Qt::OtherFocusReason);

        return;
    }

    m_object->setShortName(ui_shortNameEdit->text());
}

void ObjectEditWidget::setName()
{
    if (!m_object) return;

    if (ui_nameEdit->text() == "")
    {
        // bug in Qt, signal is fired twice when showing the message box
        if (!ui_nameEdit->isModified()) return;
        ui_nameEdit->setModified(false);

        QMessageBox::warning(this, "Missing Name", "You need to set a name for this item." );
        ui_nameEdit->setText(m_object->name());
        ui_nameEdit->setFocus(Qt::OtherFocusReason);

        return;
    }

    m_object->setName(ui_nameEdit->text());
}

void ObjectEditWidget::setComment()
{
    if (!m_object) return;

#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    m_object->setComment(ui_commentEdit->toPlainText());
#else
    m_object->setComment(ui_commentEdit->toMarkdown());
#endif

}

void ObjectEditWidget::objectRemoved(RamObject *o)
{
    Q_UNUSED(o);
    setObject(nullptr);
}

void ObjectEditWidget::unlockShortName()
{
    QMessageBox::Button ok = QMessageBox::question(
                this,
                tr("Confirm ID change"),
                tr("If you change the ID, you may need to manually rename and move files.\n\n"
                   "Are you sure you want to edit this ID?")
                );
    if (ok != QMessageBox::Yes) return;
    lockShortName(false);
}

void ObjectEditWidget::objectChanged(RamObject *o)
{
    Q_UNUSED(o);
    setObject(m_object);
}

void ObjectEditWidget::checkPath()
{
    if(!m_object) return;
    lockShortName( m_dontRename.contains(m_object->shortName()) );
    ui_nameEdit->setEnabled(true);
    // If the folder already exists, freeze the ID or the name (according to specific types)
    if ( m_object->path() != "" &&  QFileInfo::exists( m_object->path() ) && m_object->shortName() != "NEW" )
    {
        lockShortName(true);
    }
    // Always freeze step short names
    else if (m_object->objectType() == RamObject::Step) lockShortName(true);
}

void ObjectEditWidget::showEvent(QShowEvent *event)
{
    if(!event->spontaneous()) this->setObject(m_object);
}

void ObjectEditWidget::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)
    update();
}

void ObjectEditWidget::setupUi()
{
    //this->setMaximumWidth(500);

    QWidget *dummy = new QWidget(this);
    dummy->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    ui_mainLayout = new QVBoxLayout(dummy);
    ui_mainLayout->setSpacing(3);
    ui_mainLayout->setContentsMargins(3, 3, 3, 3);

    ui_mainFormLayout = new QGridLayout();
    ui_mainFormLayout->setSpacing(3);

    ui_nameLabel = new QLabel("Name", dummy);
    ui_mainFormLayout->addWidget(ui_nameLabel, 0, 0);

    ui_nameEdit = new QLineEdit(dummy);
    ui_mainFormLayout->addWidget(ui_nameEdit, 0, 1);

    // Name validator
    QRegExp rxn = RegExUtils::getRegExp("name");
    QValidator *nameValidator = new QRegExpValidator(rxn, this);
    ui_nameEdit->setValidator(nameValidator);

    ui_shortNameLabel = new QLabel("ID", dummy);
    ui_mainFormLayout->addWidget(ui_shortNameLabel, 1, 0);

    QHBoxLayout *shortNameLayout = new QHBoxLayout();
    ui_shortNameEdit = new QLineEdit(dummy);
    shortNameLayout->addWidget(ui_shortNameEdit);

    ui_lockShortNameButton = new QToolButton(this);
    ui_lockShortNameButton->setIcon(QIcon(":/icons/lock"));
    shortNameLayout->addWidget(ui_lockShortNameButton);
    ui_lockShortNameButton->hide();

    ui_mainFormLayout->addLayout(shortNameLayout, 1, 1);

    // Short Name validator
    QRegExp rxsn = RegExUtils::getRegExp("shortname");
    QValidator *shortNameValidator = new QRegExpValidator(rxsn, this);
    ui_shortNameEdit->setValidator(shortNameValidator);

    ui_commentLabel = new QLabel("Comment", dummy);
    ui_mainFormLayout->addWidget(ui_commentLabel, 2, 0);

    ui_commentEdit = new DuQFTextEdit(dummy);
    ui_commentEdit->setMaximumHeight(80);
    ui_commentEdit->setObjectName("commentEdit");
    ui_mainFormLayout->addWidget(ui_commentEdit, 2, 1);

    ui_mainLayout->addLayout(ui_mainFormLayout);

    this->setWidget(dummy);
    this->setWidgetResizable(true);
    this->setFrameStyle(QFrame::NoFrame);
}

void ObjectEditWidget::connectEvents()
{
    connect(ui_shortNameEdit, SIGNAL(editingFinished()), this, SLOT(setShortName()));
    connect(ui_nameEdit, SIGNAL(editingFinished()), this, SLOT(setName()));
    connect(ui_commentEdit, SIGNAL(editingFinished()), this, SLOT(setComment()));
    connect(ui_lockShortNameButton, &QToolButton::clicked, this, &ObjectEditWidget::unlockShortName);
}
