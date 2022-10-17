#include "ramobjectcombobox.h"
#include "ramfilterlistproxymodel.h"
#include "ramobjectdelegate.h"

RamObjectComboBox::RamObjectComboBox(QWidget *parent):
    QComboBox(parent)
{
    m_objects = new RamObjectSortFilterProxyModel("", this);
    setSortMode(RamObject::Order);
    m_objects->setSourceModel( RamObjectModel::emptyModel() );
    this->setModel(m_objects);
    setupUi();
    //setObjectModel(nullptr);
    connectEvents();
}

void RamObjectComboBox::setObjectModel(RamObjectModel *model, QString filterListName)
{
    RamFilterListProxyModel *proxyModel = new RamFilterListProxyModel(filterListName, this);
    if (model) proxyModel->setSourceModel(model);
    else proxyModel->setSourceModel( RamObjectModel::emptyModel() );
    this->setModel(proxyModel);
}

void RamObjectComboBox::setSortMode(RamAbstractObject::DataRole mode)
{
    m_objects->setSortRole(mode);
}

void RamObjectComboBox::setObjectModel(RamObjectModel *model)
{
    if (model)
    {
        m_objects->setSourceModel(model);
        m_objects->sort(0);
    }
    else
    {
        m_objects->setSourceModel( RamObjectModel::emptyModel() );
    }
}

void RamObjectComboBox::setupUi()
{
    RamObjectDelegate *delegate = new RamObjectDelegate(this);
    delegate->setComboBoxMode(true);
    this->setItemDelegate(delegate);
}

void RamObjectComboBox::connectEvents()
{
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(currentObjectIndexChanged(int)));
    connect(this, SIGNAL(activated(int)), this, SLOT(objectIndexActivated(int)));
}

RamObject *RamObjectComboBox::currentObject()
{
    int i = this->currentIndex();
    return objectAt(i);
}

QString RamObjectComboBox::currentUuid()
{
    int i = this->currentIndex();
    return uuidAt(i);
}

void RamObjectComboBox::setObject(QString uuid)
{
    /*if (uuid == "")
    {
        setCurrentIndex(-1);
        return;
    }*/

    for(int i = 0; i < this->count(); i++)
    {
        QString ou = uuidAt(i);
        if (uuid == ou)
        {
            this->setCurrentIndex(i);
            return;
        }
    }
    this->setCurrentIndex(-1);
}

void RamObjectComboBox::setObject(RamObject *obj)
{
    if (!obj) setObject("");
    else setObject(obj->uuid());
}

RamObject *RamObjectComboBox::objectAt(int i)
{
    QModelIndex ind = modelIndex(i);
    QString uuid = ind.data(RamObject::UUID).toString();
    if (uuid == "") return nullptr;

    int tI = ind.data(RamObject::Type).toInt();
    RamObject::ObjectType t = static_cast<RamObject::ObjectType>( tI );
    return RamObject::get(uuid, t);

}

QString RamObjectComboBox::uuidAt(int i)
{
    return modelIndex(i).data(RamObject::UUID).toString();
}

QModelIndex RamObjectComboBox::modelIndex(int i)
{
    return model()->index(i, 0);
}

void RamObjectComboBox::beginReset()
{
    m_resetting = true;
    m_resettingUuid = currentUuid();
}

void RamObjectComboBox::endReset()
{
    m_resetting = false;
    setObject(m_resettingUuid);
}

void RamObjectComboBox::showPopup()
{
    // Update size
    // get the minimum width that fits the largest item.
    int width = this->minimumSizeHint().width();
    // set the view to that width + icon and margins.
    this->view()->setMinimumWidth(width + 40);

    QComboBox::showPopup();
    emit popupShown();
}

void RamObjectComboBox::hidePopup()
{
    QComboBox::hidePopup();
    emit popupHidden();
}

void RamObjectComboBox::currentObjectIndexChanged(int i)
{
    Q_UNUSED(i)

#ifdef DUMP_OBJECT_DEBUG
    dumpObjectInfo();
#endif

    if (!m_resetting)
    {
        emit currentObjectChanged( currentObject() );
        emit currentUuidChanged( currentUuid() );
    }
}

void RamObjectComboBox::objectIndexActivated(int i)
{
    Q_UNUSED(i)

#ifdef DUMP_OBJECT_DEBUG
    dumpObjectInfo();
#endif

    emit objectActivated( currentObject() );
    emit uuidActivated( currentUuid() );
}
