#include "pch.h"
#include "FrontPanelDolphin.h"

#include <roapi.h>
#include <iostream>

bool g_exit = false;

void ExitSample() noexcept
{
    g_exit = true;
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

static HWND CreateDolphinWindow(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEXW wc = {};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"DolphinClass";
    RegisterClassExW(&wc);

    HWND hWnd = CreateWindowExW(
    0,
    L"DolphinClass",
    L"Dolphin Sample",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    1920, 1080,
    nullptr,
    nullptr,
    hInstance,
    nullptr);

    ShowWindow(hWnd, nCmdShow);

    return hWnd;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    try
    {
        if (FAILED(Windows::Foundation::Initialize()))
        {
            throw std::exception("Failed to initialize WinRT");
        }

        SetThreadAffinityMask(GetCurrentThread(), 0x1);

        std::unique_ptr<Sample> sample = std::make_unique<Sample>();
        sample->Initialize(CreateDolphinWindow(hInstance, nCmdShow));

        MSG msg = {};
        while (!g_exit)
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                if (msg.message == WM_QUIT)
                    return 0;
            }
            else
            {
                sample->Tick();
            }
        }

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        MessageBoxA(nullptr, e.what(), "Fatal Error", MB_OK | MB_ICONERROR);
        return 1;
    }
}