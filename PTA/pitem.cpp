#include "pitem.h"

void PItem::setName(std::string n)
{
    std::string erase = "<<set:MS>><<set:M>><<set:S>>";
    size_t      pos   = n.find(erase);

    if (pos != std::string::npos)
    {
        // If found then erase it from string
        n.erase(pos, erase.length());
    }

    name = n;
}

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
