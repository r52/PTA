#pragma once

#include <Windows.h>

namespace pta
{
    bool  IsPoEForeground();
    INPUT CreateInput(WORD vk, bool isDown);
}