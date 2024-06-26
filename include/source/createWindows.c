#include "../header/createWindows.h"
#include "../header/maindef.h"
#include "../header/record.h"


void displayDatabaseInfo(HDC hdc) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;
    char currentDate[20];
    getCurrentDate(currentDate, sizeof(currentDate));

    // 打开数据库
    rc = sqlite3_open(DATABASENAME, &db);
    if (rc) {
        TextOut(hdc, 20, 40, "Cannot open database.", 21);
        return;
    }

    // 显示当前日期
    TextOut(hdc, 20, 20, currentDate, strlen(currentDate));

    // 查询数据
    const char* sql =
        "SELECT pn.processName, SUM(r.time) AS totalTime "
        "FROM " RECORDSTABLENAME " r "
        "JOIN " NAMETABLENAME " pn ON r.processId = pn.id "
        "WHERE r.date = ? "
        "GROUP BY r.processId "
        "ORDER BY totalTime DESC";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        TextOut(hdc, 20, 60, "Failed to prepare statement.", 26);
        return;
    }

    sqlite3_bind_text(stmt, 1, currentDate, -1, SQLITE_STATIC);

    int yPos = 60;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* processName = sqlite3_column_text(stmt, 0);
        int totalTime = sqlite3_column_int(stmt, 1);

        char buffer[100];
        snprintf(buffer, sizeof(buffer), "%s: %d", processName, totalTime);
        TextOut(hdc, 20, yPos, buffer, strlen(buffer));
        yPos += 20;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            // 设置定时器
            SetTimer(hwnd, TIMER_ID, CHECK_INTERVAL, NULL);
            break;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                // 显示数据库信息
                displayDatabaseInfo(hdc);

                EndPaint(hwnd, &ps);
            }
            break;
        case WM_SIZE:
            // 窗口大小改变时重新绘制
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        case WM_DESTROY:
            KillTimer(hwnd, TIMER_ID);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}