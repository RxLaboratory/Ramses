#include "objectupdateblocker.h"

ObjectUpdateBlocker::ObjectUpdateBlocker(RamAbstractObject *o)
{
    if (o)
    {
        m_wasVirtual = o->isSaveSuspended();
        o->suspendSave(true);
        m_obj = o;
    }
}

ObjectUpdateBlocker::~ObjectUpdateBlocker()
{
    if (m_obj) m_obj->suspendSave(m_wasVirtual);
}
