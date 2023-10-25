#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void SetUpDoubleBuffering(HWND hwnd);
void Draw(HWND hwnd);
void DrawChildWindow(HDC hdc, RECT* rect);

// Global variables for double buffering
HBITMAP hbmOffscreen = NULL;
HDC hdcOffscreen = NULL;

// Variables for dragging
bool isDragging = false;
POINT dragStartPos;
RECT childWindowRect = { 100, 100, 200, 200 };

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR pCmdLine, int nCmdShow) {
    // Register the window class.

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"WindowDragDemoWin32";

    RegisterClass(&wc);

    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,
        L"WindowDragDemoWin32",
        L"WindowDragDemoWin32",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        if (hdcOffscreen) {
            DeleteDC(hdcOffscreen);
            DeleteObject(hbmOffscreen);
        }
        PostQuitMessage(0);
        return 0;

    case WM_SIZE: {
        SetUpDoubleBuffering(hwnd);
        return 0;
    }

    case WM_PAINT: {
        Draw(hwnd);
        return 0;
    }

    case WM_LBUTTONDOWN: {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        if (PtInRect(&childWindowRect, pt)) {
            isDragging = true;
            dragStartPos = pt;
            SetCapture(hwnd);
        }
        return 0;
    }

    case WM_MOUSEMOVE: {
        if (isDragging) {
            POINT pt = { LOWORD(lParam), HIWORD(lParam) };
            int dx = pt.x - dragStartPos.x;
            int dy = pt.y - dragStartPos.y;

            // Constrain to window bounds
            RECT rcClient;
            GetClientRect(hwnd, &rcClient);
            int newLeft = max(rcClient.left, min(childWindowRect.left + dx, rcClient.right - (childWindowRect.right - childWindowRect.left)));
            int newTop = max(rcClient.top, min(childWindowRect.top + dy, rcClient.bottom - (childWindowRect.bottom - childWindowRect.top)));

            SetRect(&childWindowRect, newLeft, newTop, newLeft + (childWindowRect.right - childWindowRect.left), newTop + (childWindowRect.bottom - childWindowRect.top));

            dragStartPos = pt;

            // Redraw window
            InvalidateRect(hwnd, NULL, FALSE);
        }
        return 0;
    }

    case WM_LBUTTONUP: {
        isDragging = false;
        ReleaseCapture();
        return 0;
    }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void SetUpDoubleBuffering(HWND hwnd) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;

    if (hdcOffscreen) {
        // Delete old bitmap and DC
        DeleteObject(hbmOffscreen);
        DeleteDC(hdcOffscreen);
    }

    // Create new offscreen DC and bitmap
    HDC hdcWindow = GetDC(hwnd);
    hdcOffscreen = CreateCompatibleDC(hdcWindow);
    hbmOffscreen = CreateCompatibleBitmap(hdcWindow, width, height);
    SelectObject(hdcOffscreen, hbmOffscreen);

    ReleaseDC(hwnd, hdcWindow);
}

void Draw(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    // Drawing background
    FillRect(hdcOffscreen, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

    // Drawing the "child window"
    DrawChildWindow(hdcOffscreen, &childWindowRect);

    // Copy off-screen DC to screen
    BitBlt(hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, hdcOffscreen, 0, 0, SRCCOPY);

    EndPaint(hwnd, &ps);
}

void DrawChildWindow(HDC hdc, RECT* rect) {
    FillRect(hdc, rect, (HBRUSH)(COLOR_WINDOWFRAME + 1));
}
