#include "putil.h"

#include <Windows.h>
#include <string>

bool pta::IsPoEForeground()
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

INPUT pta::CreateInput(WORD vk, bool isDown)
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
