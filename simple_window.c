#ifndef UNICODE
#define UNICODE
#endif


#include <stdio.h>
#include <string.h>
#include <windows.h>

#define WINDOW_HEIGHT 50
#define WINDOW_WIDTH 300
#define TEXT_BOX_HEIGHT WINDOW_HEIGHT -15
#define TEXT_BOX_WIDTH WINDOW_WIDTH -15

#define HOTKEY_IDENT 1234

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) ;

const wchar_t CLASS_NAME[]  = L"myClass";
HWND text_box;


HWND createButton(HINSTANCE hInstance, HWND hwnd) {
    HWND hwndButton = CreateWindow(
    L"EDIT",  // Predefined class; Unicode assumed
    L"",      // Button text
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_CENTER  ,  // Styles
    0,         // x position
    0,         // y position
    TEXT_BOX_WIDTH,        // Button width
    TEXT_BOX_HEIGHT,        // Button height
    hwnd,     // Parent window
    NULL,       // No menu.
    hInstance,
    NULL);      // Pointer not needed.

    return hwndButton;
}

HWND CreateWin(HINSTANCE hInstance) {
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"",    // Window text
        0,            // Window style

        // Size and position
        100, 100, WINDOW_WIDTH, WINDOW_HEIGHT,

        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
    return hwnd;
}
void CenterItem(HWND item,HWND bounding) {
    /*RECT rc;
    RECT bounding_rect;

    GetClientRect(item, &rc);

    GetClientRect(bounding, &bounding_rect);
    printf("item height: %d item width: %d\n",rc.bottom, rc.right );
    printf("bounding height: %d bounding width: %d\n",bounding_rect.bottom, bounding_rect.right);

    int x = (int)( (bounding_rect.right) - (rc.right - rc.left)) / 2;
    int y = (int)((bounding_rect.bottom) - (rc.bottom - rc.top)) / 2;
    //MoveWindow(item, x, y, rc.right , rc.bottom , 1);*/
    if (!item || !bounding) return;

    RECT rcBounding, rcItem;
    GetWindowRect(bounding, &rcBounding);
    GetWindowRect(item, &rcItem);

    int itemWidth = rcItem.right - rcItem.left;
    int itemHeight = rcItem.bottom - rcItem.top;

    int boundingWidth = rcBounding.right - rcBounding.left;
    int boundingHeight = rcBounding.bottom - rcBounding.top;
    printf("item height: %d item width: %d\n",itemHeight,itemWidth );
    printf("bounding height: %d bounding width: %d\n",boundingHeight,boundingWidth);
    int newX = (boundingWidth - itemWidth) / 2;
    int newY = (boundingHeight - itemHeight) / 2;

    // Convert screen coordinates to parent-relative coordinates
    POINT pt = { newX, newY };
    ScreenToClient(bounding, &pt);
    MoveWindow(item, newX, newY, itemWidth , itemHeight , 1);
    // Move the item to the center
    //SetWindowPos(item, NULL, pt.x, pt.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

}

void SetStyleWindow(HWND window,int style) {
    SetWindowLong(window, GWL_STYLE, style);
}
void HideWindow(HWND window) {
    //SetWindowLong(window, GWL_STYLE, WS_VIS);
    ShowWindow(window, SW_HIDE);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {



    WNDCLASS wc = {0};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWin(hInstance);
    if (hwnd == NULL)
    {
        printf("failed");
        return -1;
    }

    CenterItem(hwnd, GetDesktopWindow());

    RegisterHotKey(hwnd,HOTKEY_IDENT,MOD_ALT , 0x4D );

    SetStyleWindow(hwnd, WS_POPUP);
    text_box = createButton(hInstance, hwnd);

    CenterItem(text_box,hwnd);

    SetFocus(text_box);

    //https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
    ShowWindow(hwnd, SW_NORMAL);


    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        if (msg.message == WM_KEYDOWN && (msg.wParam == VK_ESCAPE || msg.wParam == VK_RETURN    )) {
            msg.hwnd = hwnd; // this is because some of the messages may for other classes
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }



    return 0;
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch(uMsg) {
        case WM_KEYDOWN: {
            puts("Key Down");
            switch (wParam) {
                case VK_RETURN:
                case VK_ESCAPE:
                    SetWindowText(text_box, L"");

                    HideWindow(hwnd);
                    return 0;
                default: ;

            }
            break;
        }


        case WM_HOTKEY: {
            if (wParam == HOTKEY_IDENT) {
                printf("hot key pressed:\n");
                ShowWindow(hwnd,SW_SHOW);
                BringWindowToTop(hwnd);
                SetFocus(text_box);

            }
            break;
        }
        /*case WM_ACTIVATEAPP: {
            printf("out of focus\n");
            HideWindow(hwnd);
        }*/

        case WM_SIZE:
        {

            int width = LOWORD(lParam);  // Macro to get the low-order word.
            int height = HIWORD(lParam); // Macro to get the high-order word.

            //return 0;
            break;
        }
        case WM_PAINT:
        {
            printf("paint\n");
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.
            HBRUSH brush = CreateSolidBrush(RGB(55, 55, 55));
            FillRect(hdc, &ps.rcPaint, (HBRUSH) brush);

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_CREATE: {

            HRGN hRgn = CreateRoundRectRgn(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 5, 5); // Rounded region
            SetWindowRgn(hwnd, hRgn, TRUE);

            break;
        }
        case WM_CLOSE:
        {
            if (MessageBox(hwnd, L"Really quit?", L"DONT QUIT MY APP", MB_OKCANCEL ) == IDOK)
            {
                DestroyWindow(hwnd);

            }
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }



    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

