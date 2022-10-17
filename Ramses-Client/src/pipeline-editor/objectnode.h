#ifndef OBJECTNODE_H
#define OBJECTNODE_H

#include <QDockWidget>

#include "duqf-nodeview/duqfnode.h"

#include "ramobject.h"

class ObjectNode : public DuQFNode
{
public:
    ObjectNode(RamObject *obj);
    RamObject *ramObject() const;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private slots:
    virtual void objectChanged();
    void objectRemoved();

private:
    RamObject *m_object;

    bool m_mousePress;
};

#endif // STEPNODE_H
