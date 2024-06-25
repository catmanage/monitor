#ifndef MAINDEF_H
#define MAINDEF_H

#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <processthreadsapi.h>

#define TIMER_ID 1
#define CHECK_INTERVAL 5000 // 检查间隔（毫秒）
#define MAX_PATH_LENGTH 260
#define MAX_PROCESSES 100

//#define FOCUS_RECORD_FILE "record/daily_focus_records.txt"//保存文件
#define TOTAL_RECORD_FILE "records\\total_records.txt"
#define TEMP_RECORD_FILE "records\\temp_records.txt"

//每一行的长度
#define LINELENGTH 50





// 全局变量的声明
typedef struct {
    char processName[MAX_PATH_LENGTH];
    int totalTime;
    int dateTime;
} ProcessFocusTime;

 ProcessFocusTime focusTimes[MAX_PROCESSES]; // 存储每个进程的聚焦时间
 int numProcesses; // 进程计数器
 clock_t focusStartTime; // 记录当前聚焦开始时间

 HWND hwndMain; // 主窗口句柄
//每行容器
 char line[LINELENGTH];

#endif // MAINDEF_H
