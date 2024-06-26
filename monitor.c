#include "includes.h"

void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
     // 获取当前活动窗口的进程名
    char currentProcessName[MAX_PATH_LENGTH];
    getActiveWindowProcessName(currentProcessName);
    
    //给窗口添加时间
    maintainProce(currentProcessName);
    

    // 更新窗口内容显示
    InvalidateRect(hwndMain, NULL, TRUE); // 标记窗口无效区域以便重绘
    UpdateWindow(hwndMain); // 强制窗口更新显示
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    initializeFiles(); // 初始化记录文件

    // 获取屏幕宽高
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 计算窗口初始大小
    int windowWidth = screenWidth / 2;
    int windowHeight = screenHeight / 2;

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc; // 指定窗口过程函数
    wc.hInstance = hInstance;
    wc.lpszClassName = "FocusTimeWindowClass";

    RegisterClass(&wc); // 注册窗口类

    hwndMain = CreateWindow(wc.lpszClassName, "Focus Time Tracker", WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, 
                            NULL, NULL, hInstance, NULL); // 创建窗口

    if (hwndMain == NULL) {
        MessageBox(NULL, "Window creation failed.", "Error", MB_OK | MB_ICONERROR); // 窗口创建失败时显示错误提示框
        return 1;
    }

    ShowWindow(hwndMain, nCmdShow); // 显示窗口
    UpdateWindow(hwndMain);

    // 设置定时器
    UINT_PTR timerId = SetTimer(hwndMain, TIMER_ID, CHECK_INTERVAL, TimerProc);
    if (timerId == 0) {
        MessageBox(NULL, "Timer creation failed.", "Error", MB_OK | MB_ICONERROR); // 设置定时器失败时显示错误提示框
        return 1;
    }

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}