#include "putil.h"

#include <Windows.h>

#include <string>

#include <QDebug>

namespace
{
    HWINEVENTHOOK g_ForegroundHook = nullptr;
    HHOOK         g_MouseHook      = nullptr;
    HHOOK         g_KeyboardHook   = nullptr;

    std::function<void(bool)>                         g_ForegroundHookCb = nullptr;
    std::function<bool(WPARAM wParam, LPARAM lParam)> g_MouseHookCb      = nullptr;
    std::function<bool(WPARAM wParam, LPARAM lParam)> g_KeyboardHookCb   = nullptr;
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

        LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
        {
            if (nCode >= HC_ACTION && g_MouseHookCb && g_MouseHookCb(wParam, lParam))
            {
                // consume the input
                return -1;
            }

            return CallNextHookEx(g_MouseHook, nCode, wParam, lParam);
        }

        LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
        {
            if (nCode >= HC_ACTION && g_KeyboardHookCb && g_KeyboardHookCb(wParam, lParam))
            {
                // consume the input
                return -1;
            }

            return CallNextHookEx(g_KeyboardHook, nCode, wParam, lParam);
        }

        void InitializeHooks()
        {
#ifdef NDEBUG
            g_ForegroundHook = SetWinEventHook(
                EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, ForegroundHookCallback, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

            if (!g_ForegroundHook)
            {
                qWarning() << "Failed to set foreground event hook. Macros may not work.";
            }

            g_MouseHook = SetWindowsHookEx(WH_MOUSE_LL, &LowLevelMouseProc, GetModuleHandle(NULL), NULL);

            if (!g_MouseHook)
            {
                qWarning() << "Failed to set mouse event hook. Some functions may not work.";
            }

            g_KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, &LowLevelKeyboardProc, GetModuleHandle(NULL), NULL);

            if (!g_KeyboardHook)
            {
                qWarning() << "Failed to set keyboard event hook. Some functions may not work.";
            }
#endif
        }

        void ShutdownHooks()
        {
            if (g_ForegroundHook)
            {
                UnhookWinEvent(g_ForegroundHook);
            }

            if (g_MouseHook)
            {
                UnhookWindowsHookEx(g_MouseHook);
            }

            if (g_KeyboardHook)
            {
                UnhookWindowsHookEx(g_KeyboardHook);
            }
        }

        void InstallForegroundHookCb(std::function<void(bool)> fgcb)
        {
            if (fgcb)
            {
                g_ForegroundHookCb = fgcb;
            }
        }

        void InstallMouseHookCb(std::function<bool(WPARAM wParam, LPARAM lParam)> cb)
        {
            if (cb)
            {
                g_MouseHookCb = cb;
            }
        }

        void InstallKeyboardHookCb(std::function<bool(WPARAM wParam, LPARAM lParam)> cb)
        {
            if (cb)
            {
                g_KeyboardHookCb = cb;
            }
        }
    }
}
