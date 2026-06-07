#include <windows.h>
#include "CPU.h"
#include "Renderer.h"

CPU cpu;
Renderer renderer;
bool running = false;

bool isInside(int x, int y, int boxX, int boxY, int boxW, int boxH) {
    return x >= boxX && x <= boxX + boxW && y >= boxY && y <= boxY + boxH;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        SetTimer(hwnd, 1, 30, NULL);
        return 0;

    case WM_TIMER:
        renderer.updateAnimation(running);
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;

    case WM_ERASEBKGND:
        return 1;

    case WM_LBUTTONDOWN: {
        int mouseX = LOWORD(lParam);
        int mouseY = HIWORD(lParam);

        if (isInside(mouseX, mouseY, 930, 755, 75, 35)) {
            running = !running;
        }
        else if (isInside(mouseX, mouseY, 1020, 755, 75, 35)) {
            if (!running) {
                cpu.step();
                renderer.resetAnimation();
            }
        }
        else if (isInside(mouseX, mouseY, 1110, 755, 85, 35)) {
            cpu.reset();
            cpu.loadDefaultProgram();
            renderer.resetAnimation();
            running = false;
        }

        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }

    case WM_KEYDOWN:
        if (wParam == 'R') {
            running = !running;
        }
        else if (wParam == VK_SPACE) {
            if (!running) {
                cpu.step();
                renderer.resetAnimation();
            }
        }
        else if (wParam == VK_BACK) {
            cpu.reset();
            cpu.loadDefaultProgram();
            renderer.resetAnimation();
            running = false;
        }
        else if (wParam == VK_ESCAPE) {
            DestroyWindow(hwnd);
        }

        InvalidateRect(hwnd, NULL, FALSE);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT paintStruct;
        HDC hdc = BeginPaint(hwnd, &paintStruct);

        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        HDC memoryDC = CreateCompatibleDC(hdc);
        HBITMAP memoryBitmap = CreateCompatibleBitmap(
            hdc,
            clientRect.right - clientRect.left,
            clientRect.bottom - clientRect.top
        );

        HBITMAP oldBitmap = (HBITMAP)SelectObject(memoryDC, memoryBitmap);

        renderer.drawAll(memoryDC, running);

        BitBlt(
            hdc,
            0,
            0,
            clientRect.right - clientRect.left,
            clientRect.bottom - clientRect.top,
            memoryDC,
            0,
            0,
            SRCCOPY
        );

        SelectObject(memoryDC, oldBitmap);
        DeleteObject(memoryBitmap);
        DeleteDC(memoryDC);

        EndPaint(hwnd, &paintStruct);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcA(hwnd, message, wParam, lParam);
}

int main() {
    cpu.loadDefaultProgram();
    renderer.setCPU(&cpu);

    HINSTANCE instance = GetModuleHandleA(NULL);

    WNDCLASSA windowClass = {};
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = "CPUProjectWindow";
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClassA(&windowClass);

    HWND window = CreateWindowA(
        "CPUProjectWindow",
        "CPU Instruction Execution Visual Simulator",
        WS_OVERLAPPEDWINDOW,
        80,
        40,
        1250,
        850,
        NULL,
        NULL,
        instance,
        NULL
    );

    if (window == NULL) {
        return 1;
    }

    renderer.setWindow(window);

    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);

    MSG message = {};

    while (GetMessageA(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return 0;
}
