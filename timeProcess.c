#include "timeProcess.h"
#include "maindef.h"
#include "record.h"
#include "tools.h"




// 更新聚焦时间
void maintainProce(char *processName) {
    ProcessFocusTime* processFocusTime = NULL; // 初始化为 NULL

    // 查找进程号
    int pnum = findpnum(processName);

    if (pnum == -1) {
        // 如果没找到就去文件里找
        processFocusTime = (ProcessFocusTime*)malloc(sizeof(ProcessFocusTime));
        
        // 处理内存分配失败的情况
        if (processFocusTime == NULL) {
            MessageBox(NULL, "Memory allocation failed.", "Error", MB_OK | MB_ICONERROR);
            return;
        }


        strcpy(processFocusTime->processName, processName);

        // 初始化processFocusTime总时长
        if (initTotalRecord(processFocusTime) != 1) {
            free(processFocusTime); // 如果查找失败，释放内存
            return;
        }

        // 初始化processFocusTime日期时长
        if(initDateRecord(processFocusTime) != 1){
            free(processFocusTime); // 如果查找失败，释放内存
            return;
        }
        

        // 添加新进程记录到数组
        pnum = numProcesses;
        focusTimes[numProcesses++] = *processFocusTime;
    } else {
        processFocusTime = &focusTimes[pnum];
    }

    // 聚焦时间增加 1s
    processFocusTime->totalTime += 1;
    processFocusTime->dateTime += 1;

    save(processFocusTime); // 保存更新后的数据
}

// 查找文件里面的记录:如果找到了就初始化processFocusTime，如果没找到就创建新的记录,一般会返回1
int initTotalRecord(ProcessFocusTime* processFocusTime) {
    
    FILE* totalFile = fopen(TOTAL_RECORD_FILE, "r+");
    
    if (totalFile == NULL) {
        MessageBox(NULL, "Error opening total record file.", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

   
    int found = 0;

    while (fgets(line, sizeof(line), totalFile)) {
        char filename[LINELENGTH];
        int value;

        if (sscanf(line, "%[^:]:%d", filename, &value) == 2) {
            // 找到了总记录，初始化processFocusTime
            if (strcmp(filename, processFocusTime->processName) == 0) {
                // 初始化processFocusTime总时长
                processFocusTime->totalTime = value;
                found = 1;
                break;
            }
        }
    }

    // 如果没找到，创建新的记录
    if (!found) {
        // 初始化processFocusTime
        processFocusTime->totalTime = 0;
        processFocusTime->dateTime = 0;

    
        strcpy(line,processFocusTime->processName);
        strcat(line,":0");
        // 填充 processName 到一定长度
        fillWithSpaces(line,sizeof(line));
        // 写入新总记录
        fprintf(totalFile,line);
    }
    fclose(totalFile);
    return 1;
}


// 初始化processFocusTime日期时长
int initDateRecord(ProcessFocusTime* processFocusTime) {
    FILE* dateFile = fopen(TEMP_RECORD_FILE, "r+");
    if (dateFile == NULL) {
        MessageBox(NULL, "Error opening focus record file.", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    int found = 0;

    while (fgets(line, sizeof(line), dateFile)) {
        char filename[LINELENGTH];
        int value;

        if (sscanf(line, "%[^:]:%d", filename, &value) == 2) {
            // 找到了日期记录，初始化processFocusTime
            if (strcmp(filename, processFocusTime->processName) == 0) {
                processFocusTime->dateTime = value;
                found = 1;
                break;
            }
        }
    }

    // 如果没找到，创建新的记录
    if (!found) {
        processFocusTime->dateTime = 0;


        strcpy(line,processFocusTime->processName);
        strcat(line,":0");
        // 填充 processName 到一定长度
        fillWithSpaces(line,sizeof(line));
        // 写入新总记录
        fprintf(dateFile,line); // 写入新日期记录

    }

    fclose(dateFile);
    return 1;
}

// 创建新的日期记录
void createDateRecord(ProcessFocusTime* processFocusTime) {
    FILE* dateFile = fopen(TEMP_RECORD_FILE, "a");
    if (dateFile == NULL) {
        MessageBox(NULL, "Error opening focus record file.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    strcpy(line,processFocusTime->processName);
    strcat(line,":0");
    // 填充 processName 到一定长度
    fillWithSpaces(line,sizeof(line));
    // 写入新总记录
    fprintf(dateFile,line); // 写入新日期记录
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
void save(ProcessFocusTime* processFocusTime) {
    updateFocusTime(processFocusTime, 1); // 更新总聚焦时间记录
    updateFocusTime(processFocusTime, 2); // 更新每日聚焦时间记录
}
