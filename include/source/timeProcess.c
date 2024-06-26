#include "../header/timeProcess.h"
#include "../header/maindef.h"
#include "../header/record.h"
#include "../header/tools.h"




// 更新聚焦时间
void maintainProce(char *processName) {
    //初始化processFocusTime
    ProcessFocusTime* processFocusTime = (ProcessFocusTime*)malloc(sizeof(ProcessFocusTime)); // 初始化为 NULL
    StringCbCopyA(processFocusTime->processName, sizeof(processFocusTime->processName), processName);
   
    // 获取当前时间
    time_t now = time(0);
    struct tm tstruct;
    localtime_s(&tstruct, &now);

    // 格式化日期为 年_月_日
    strftime(processFocusTime->date, sizeof(processFocusTime->date), "%Y_%m_%d", &tstruct);
    // 获取当前的小时
    // 根据当前小时设置 section
    processFocusTime->section = tstruct.tm_hour; 
    
    // 保存更新后的数据
    add(processFocusTime); 

    free(processFocusTime);
}


// 查找进程在记录中的索引
int findpnum(char* processName) {
    for (int i = 0; i < numProcesses; ++i) {
        if (strcmp(focusTimes[i].processName, processName) == 0) {
            return i;
        }
    }
    return -1;
}

// 保存更新的聚焦时间
void add(ProcessFocusTime* processFocusTime) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    // 打开数据库
    rc = sqlite3_open(DATABASENAME, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // 查询name表中对应processName的id
    const char* sqlSelectName = "SELECT id FROM " NAMETABLENAME " WHERE processName = ?;";
    rc = sqlite3_prepare_v2(db, sqlSelectName, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, processFocusTime->processName, -1, SQLITE_STATIC);

    int id = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    if (id == -1) {
        // 没有找到匹配的记录，需要插入新记录
        const char* sqlInsertName = "INSERT INTO " NAMETABLENAME " (processName) VALUES (?);";
        rc = sqlite3_prepare_v2(db, sqlInsertName, -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        sqlite3_bind_text(stmt, 1, processFocusTime->processName, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
        }

        // 获取刚插入的记录的id
        id = sqlite3_last_insert_rowid(db);

        sqlite3_finalize(stmt);
    }


    // 查询records表中对应的记录
    const char* sqlSelectRecord = "SELECT time FROM " RECORDSTABLENAME " WHERE processId = ? AND date = ? AND section = ?;";
    rc = sqlite3_prepare_v2(db, sqlSelectRecord, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, processFocusTime->date, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, processFocusTime->section);

   
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        processFocusTime->dateTime = sqlite3_column_int(stmt, 0);
        processFocusTime->dateTime += 1;
    } else {
        processFocusTime->dateTime = 1;
    }
    sqlite3_finalize(stmt);

    // 更新或插入records表中的记录
    const char* sqlUpsertRecord =
    "INSERT INTO " RECORDSTABLENAME " (processId, section, time, date) VALUES (?, ?, ?, ?) "
    "ON CONFLICT(processId, date, section) DO UPDATE SET time = excluded.time;";

    
    rc = sqlite3_prepare_v2(db, sqlUpsertRecord, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_int(stmt, 2, processFocusTime->section);
    sqlite3_bind_int(stmt, 3, processFocusTime->dateTime);
    sqlite3_bind_text(stmt, 4, processFocusTime->date, -1, SQLITE_STATIC);
    
   

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
