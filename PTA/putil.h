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
        void InstallForegroundHookCb(std::function<void(bool)> fgcb);
        void InstallMouseHookCb(std::function<bool(WPARAM wParam, LPARAM lParam)> cb);
        void InstallKeyboardHookCb(std::function<bool(WPARAM wParam, LPARAM lParam)> cb);
    }
}