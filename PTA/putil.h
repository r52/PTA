#pragma once

#include <functional>

#include <Windows.h>

namespace pta
{
    bool  IsPoEForeground();
    INPUT CreateInput(WORD vk, bool isDown);

    namespace hook
    {
        void InitializeHooks();
        void ShutdownHooks();
        void SetForegroundHookCb(std::function<void(bool)> fgcb);
    }
}