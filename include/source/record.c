#include "../header/record.h"
#include "../header/maindef.h"
#include "../header/tools.h"



// 获取当前日期字符串
void getCurrentDate(char* buffer, size_t size) {
    time_t now = time(0);
    struct tm tstruct;
    localtime_s(&tstruct, &now);
    strftime(buffer, size, "%Y_%m_%d", &tstruct); // 格式化日期为年-月-日
}

// 格式化时间为小时、分钟、秒
void formatTime(clock_t time, char* buffer, size_t size) {
    snprintf(buffer, size, "%d", time); // 格式化为 HH小时MM分SS秒
}

void initializeFiles() {
    FILE* focusFile = fopen(TEMP_RECORD_FILE, "a+");
    FILE* totalFile = fopen(TOTAL_RECORD_FILE, "a+");
    if (focusFile == NULL || totalFile == NULL) {
        perror("Error opening file");
        MessageBox(NULL, "Error opening file.", "Error", MB_OK | MB_ICONERROR);
        exit(1);
    }
    fclose(focusFile);
    fclose(totalFile);
}



void updateFocusTime(ProcessFocusTime* processFocusTime,int f) {
    FILE* file=NULL;
    int time=0;

    if(f==1){
        file = fopen(TOTAL_RECORD_FILE, "r+");
        time=processFocusTime->totalTime;
    }else{
        file = fopen(TEMP_RECORD_FILE, "r+");
        time=processFocusTime->dateTime;
        
    }

    if (file == NULL) {
        MessageBox(NULL, "Error opening total record file.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    //找对应行数
    int lines=findLine(file,processFocusTime->processName);

    if (lines!=-1) {
        strcpy(line,processFocusTime->processName);
        strcat(line,":");

        char temp[LINELENGTH]; // 假设足够大，可以容纳转换后的整数字符串
        sprintf(temp, "%d", time); // 将整数 num 格式化为字符串
        strcat(line,temp);

        fillWithSpaces(line,sizeof(line));
        replaceLine(lines, line, file);

    } else {
        // Didn't find the process name, add new record at the end of the file
        fseek(file, 0, SEEK_END); // Move to the end of the file
        fprintf(file, "%s:%d\n", processFocusTime->processName, time);
    }

    fclose(file);
}