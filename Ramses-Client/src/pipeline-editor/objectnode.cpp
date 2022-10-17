#include "objectnode.h"

ObjectNode::ObjectNode(RamObject *obj):
    DuQFNode(obj->shortName())
{
    m_object = obj;

    this->setTitleToolTip(m_object->name());
    this->setIcon(obj->iconName());

    connect(obj, &RamObject::dataChanged, this, &ObjectNode::objectChanged);
    connect(obj, &RamObject::removed, this, &ObjectNode::objectRemoved);

    this->setObjectName( "ObjectNode" );
}

RamObject *ObjectNode::ramObject() const
{
    return m_object;
}

void ObjectNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_object->edit( true );
    DuQFNode::mousePressEvent(event);
}

void ObjectNode::objectChanged()
{
    this->setTitle(m_object->shortName());
    this->setTitleToolTip(m_object->name());
}

void ObjectNode::objectRemoved()
{
    this->remove();
}

