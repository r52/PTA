#include "putil.h"

#include <Windows.h>

#include <string>

#include <QDebug>

namespace
{
    HWINEVENTHOOK             g_ForegroundHook   = nullptr;
    std::function<void(bool)> g_ForegroundHookCb = nullptr;
}

namespace pta
{
    bool IsPoEForeground()
    {
        static const std::wstring s_poeCls = L"POEWindowClass";

        HWND hwnd = GetForegroundWindow();

        if (nullptr == hwnd)
        {
            return false;
        }

        wchar_t cls[512];
        GetClassName(hwnd, cls, std::size(cls));

        if (s_poeCls != cls)
            return false;

        return true;
    }

    INPUT CreateInput(WORD vk, bool isDown)
    {
        INPUT input          = {};
        input.type           = INPUT_KEYBOARD;
        input.ki.wVk         = vk;
        input.ki.wScan       = 0;
        input.ki.dwFlags     = (isDown ? 0 : KEYEVENTF_KEYUP);
        input.ki.time        = 0;
        input.ki.dwExtraInfo = 0;
        return input;
    }

    namespace hook
    {
        VOID CALLBACK
        ForegroundHookCallback(HWINEVENTHOOK hWinEventHook, DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
        {
            if (g_ForegroundHookCb)
            {
                g_ForegroundHookCb(IsPoEForeground());
            }
        }

        void InitializeHooks()
        {
            g_ForegroundHook = SetWinEventHook(
                EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, ForegroundHookCallback, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

            if (!g_ForegroundHook)
            {
                qWarning() << "Failed to set foreground event hook. Macros may not work.";
            }
        }

        void ShutdownHooks()
        {
            if (g_ForegroundHook)
            {
                UnhookWinEvent(g_ForegroundHook);
            }
        }

        void SetForegroundHookCb(std::function<void(bool)> fgcb)
        {
            if (fgcb)
            {
                g_ForegroundHookCb = fgcb;
            }
        }
    }
}
