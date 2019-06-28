#include "pitem.h"

void PItem::setType(std::string t)
{
    std::string erase = "Superior ";
    size_t      pos   = t.find(erase);

    if (pos != std::string::npos)
    {
        // If found then erase it from string
        t.erase(pos, erase.length());
    }

    type = t;
}
