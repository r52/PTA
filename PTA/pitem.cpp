#include "pitem.h"

void PItem::setType(std::string type)
{
    std::string erase = "Superior ";
    size_t      pos   = type.find(erase);

    if (pos != std::string::npos)
    {
        // If found then erase it from string
        type.erase(pos, erase.length());
    }

    m_type = type;
}
