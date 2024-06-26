#include "../header/record.h"
#include "../header/maindef.h"
#include "../header/tools.h"




// 获取当前日期字符串
void getCurrentDate(char* date,size_t size) {
    // 获取当前时间
    time_t now = time(0);
    struct tm tstruct;
    localtime_s(&tstruct, &now);

    // 格式化日期为 年_月_日
    strftime(date, size, "%Y_%m_%d", &tstruct);
}


// 格式化时间为小时、分钟、秒
void formatTime(clock_t time, char* buffer, size_t size) {
    snprintf(buffer, size, "%d", time); // 格式化为 HH小时MM分SS秒
}


void initializeFiles() {
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open(DATABASENAME, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return ;
    }

    const char* createNameTableSQL = 
        "CREATE TABLE IF NOT EXISTS " NAMETABLENAME " ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "processName TEXT NOT NULL);";

    const char* createRecordsTableSQL =
    "CREATE TABLE IF NOT EXISTS " RECORDSTABLENAME " ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "processId INTEGER NOT NULL,"
    "section INTEGER NOT NULL,"
    "time INTEGER NOT NULL,"
    "date TEXT NOT NULL,"
    "UNIQUE (processId, date, section)"
    ");";


    rc = sqlite3_exec(db, createNameTableSQL, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return ;
    }
    rc = sqlite3_exec(db, createRecordsTableSQL, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return ;
    }

    sqlite3_close(db);
    return ;
}