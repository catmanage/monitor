#include "../header/dailyUpdate.h"
#include "../header/maindef.h"
#include "../header/record.h"
#include "../header/tools.h"




void updateTemp() {
    char currentDate[LINELENGTH];
    getCurrentDate(currentDate, sizeof(currentDate));
    
    FILE* tempdailyfile = fopen(TEMP_RECORD_FILE, "r");
    if (tempdailyfile == NULL) {
        MessageBox(NULL, "Error opening temp daily focus record file.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    char tempDate[LINELENGTH];
    fgets(tempDate, sizeof(tempDate), tempdailyfile);
    fclose(tempdailyfile);

    


    //格式化
    fillWithSpaces(currentDate, sizeof(currentDate));
    fillWithSpaces(tempDate, sizeof(tempDate));

    //debug------------------------------------
    printf("currentDate: %s\n", currentDate);
    printf("tempDate: %s\n", tempDate);
    printf("strcmp: %d\n", strcmp(tempDate, currentDate));
    //-----------------------------------------
    //如果不是当天的
    if (strcmp(tempDate, currentDate) != 0) {
        
        // 重命名 tempdailyfile 为 {currentDate}.txt
        char newTempDailyFileName[LINELENGTH];
        snprintf(newTempDailyFileName, sizeof(newTempDailyFileName), "records\\%s.txt", tempDate);
        removeSpacesAndNewlines(newTempDailyFileName);
        printf(newTempDailyFileName);

        //todo  bug

        if (rename(TEMP_RECORD_FILE, newTempDailyFileName) != 0) {
            char errorMessage[256]; // 存储错误消息的缓冲区

            // 使用 strerror 获取错误描述字符串，并构建完整的错误消息
            sprintf(errorMessage, "Error renaming %s to %s: %s", TEMP_RECORD_FILE, newTempDailyFileName, strerror(errno));
            printf(errorMessage);
            // 使用 MessageBox 函数显示错误消息框
            MessageBox(NULL, errorMessage, "Error", MB_OK | MB_ICONERROR);
            // MessageBox(NULL, "Error renaming temp daily focus record file.", "Error", MB_OK | MB_ICONERROR);
            return;
        }
    
        

        // 创建新的 tempdailyfile 文件
        tempdailyfile = fopen(TEMP_RECORD_FILE, "w");
        if (tempdailyfile == NULL) {
            MessageBox(NULL, "Error creating new temp daily focus record file.", "Error", MB_OK | MB_ICONERROR);
            return;
        }

        // 写入当前日期
        fprintf(tempdailyfile, "%s", currentDate);
        fclose(tempdailyfile);
    }

}
