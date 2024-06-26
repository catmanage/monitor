#ifndef MAINDEF_H
#define MAINDEF_H


#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <processthreadsapi.h>
#include "sqlite3.h"
#include <tchar.h>
#include <strsafe.h>
#include <VersionHelpers.h>


#define TIMER_ID 1
#define CHECK_INTERVAL 5000 // 检查间隔（毫秒）
#define MAX_PATH_LENGTH 260
#define MAX_PROCESSES 100

//#define FOCUS_RECORD_FILE "record/daily_focus_records.txt"//保存文件
#define TOTAL_RECORD_FILE "records\\total_records.txt"
#define TEMP_RECORD_FILE "records\\temp_records.txt"


//每一行的长度
#define  NAMELENGTH 100

//数据库文件名
#define DATABASENAME "records.db"
#define NAMETABLENAME "processName"
#define RECORDSTABLENAME "records"

typedef struct {
    char processName[MAX_PATH_LENGTH];
    char date[NAMELENGTH];
    int section;
    int totalTime;
    int dateTime;
} ProcessFocusTime;

extern ProcessFocusTime focusTimes[MAX_PROCESSES];
extern int numProcesses;
extern clock_t focusStartTime;
extern HWND hwndMain;

#endif // MAINDEF_H
