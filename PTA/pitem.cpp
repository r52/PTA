#include "pitem.h"

void PItem::setType(QString type)
{
    type   = type.remove("Superior ");
    m_type = type;
}
