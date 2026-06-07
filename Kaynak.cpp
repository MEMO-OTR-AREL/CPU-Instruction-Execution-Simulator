#include <windows.h>
#include "CPU.h"
#include "Renderer.h"

CPU cpu;
Renderer renderer;
bool running = false;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        SetTimer(hwnd, 1, 30, NULL);
        return 0;

    case WM_TIMER:
        renderer.updateAnimation(running);
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;

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
        renderer.drawAll(hdc, running);
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