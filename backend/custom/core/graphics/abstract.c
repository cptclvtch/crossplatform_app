#ifdef USE_VK
#include "vk_abstract.c"
#endif

#ifdef CUSTOM_BOILERPLATE
#ifdef _WIN32
HWND create_window(HINSTANCE instance, int window_show)
{
    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"window class";
    
    WNDCLASS wc = {};

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = instance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L""APP_NAME"",                  // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        instance,   // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, window_show);

    return hwnd;
}
#endif
#endif