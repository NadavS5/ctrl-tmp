//
// Created by Nadav Salem on 06-Feb-25.
//
#include <windows.h>
#include <stdio.h>

HHOOK _k_hook;

LRESULT __stdcall key_callback(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
    //a key was pressed
    if (wParam == WM_KEYDOWN && nCode == HC_ACTION)
    {

        switch (key->vkCode)
        {
            case 0x54:
                puts("T pressed");
            break;
            case 0x4D:
                puts("M pressed");
            break;
            case 0x50:
                puts("P pressed");
            break;
            case VK_CONTROL:
                puts("Control pressed");
            break;
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


int main()
{
    _k_hook = SetWindowsHookEx(WH_KEYBOARD_LL, key_callback, NULL, 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    if (_k_hook)
        UnhookWindowsHookEx(_k_hook);
    return TRUE;
}