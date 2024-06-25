#include "createWindows.h"
#include "maindef.h"


// 窗口过程函数


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            // 创建 GUI 元素
            break;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                char buffer[100];
                int yPos = 20;

                // 显示总聚焦时间记录
                FILE* totalFile = fopen(TOTAL_RECORD_FILE, "r");
                if (totalFile == NULL) {
                    MessageBox(NULL, "Error opening total record file.", "Error", MB_OK | MB_ICONERROR); // 打开总记录文件失败时显示错误提示框
                    EndPaint(hwnd, &ps);
                    break;
                }

                while (fgets(buffer, sizeof(buffer), totalFile)) {
                    TextOut(hdc, 20, yPos, buffer, strlen(buffer)); // 在窗口上绘制记录内容
                    yPos += 20;
                }

                fclose(totalFile);

                EndPaint(hwnd, &ps);
            }
            break;
        case WM_DESTROY:
            // 清理工作
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}