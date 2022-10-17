#ifndef OBJECTUPDATEBLOCKER_H
#define OBJECTUPDATEBLOCKER_H

#include "ramabstractobject.h"

class ObjectUpdateBlocker
{
public:
    ObjectUpdateBlocker(RamAbstractObject *o);
    ~ObjectUpdateBlocker();
private:
    RamAbstractObject *m_obj = nullptr;
    bool m_wasVirtual = false;
};

#endif // OBJECTUPDATEBLOCKER_H
