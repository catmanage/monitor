#include "../header/tools.h"
#include "../header/maindef.h"



// 获取当前窗口的进程名
void getActiveWindowProcessName(char* processName) {
    HWND hwnd = GetForegroundWindow();
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (hProcess != NULL) {
        char buffer[MAX_PATH];
        DWORD bufferSize = MAX_PATH;

        if (QueryFullProcessImageNameA(hProcess, 0, buffer, &bufferSize)) {
            // Extract process name from the full path
            const char* filename = strrchr(buffer, '\\');
            if (filename != NULL) {
                strncpy(processName, filename + 1, MAX_PATH - 1);
            } else {
                strncpy(processName, buffer, MAX_PATH - 1);
            }
            processName[MAX_PATH - 1] = '\0'; // Ensure null-termination
        } else {
            // Error handling if QueryFullProcessImageName fails
            printf("QueryFullProcessImageName failed: %d\n", GetLastError());
        }

        CloseHandle(hProcess);
    } else {
        // Error handling if OpenProcess fails
        printf("OpenProcess failed: %d\n", GetLastError());
    }
}


//寻找某一行，找到返回行数，失败返回-1
int findLine(FILE *file, const char *aim) {
    char line[LINELENGTH];
    int lineNumber = 1;

    // Rewind the file to the beginning
    rewind(file);

    // Read through the file line by line
    while (fgets(line, sizeof(line), file)) {
        char *colonPos = strchr(line, ':');
        if (colonPos) {
            // Extract the xxx part before the colon
            size_t len = colonPos - line;
            char key[256];
            strncpy(key, line, len);
            key[len] = '\0';

            // Check if the key matches the aim
            if (strcmp(key, aim) == 0) {
                return lineNumber;
            }
        }
        lineNumber++;
    }

    // If no match is found, return -1
    return -1;
}



void replaceLine(int lineNumber, const char *newLine, FILE *file) {
     char buffer[LINELENGTH];
     
    // 将文件指针移动到指定行的开头位置
    fseek(file, 0, SEEK_SET);
    
    // 跳过 lineNumber - 1 行
   
    for (int i = 0; i < lineNumber - 1; ++i) {
        if (fgets(buffer, sizeof(buffer), file) == NULL) {
            // 如果到达文件末尾或读取错误
            break;
        }
        printf("%s", buffer); // 打印跳过的行
    }

    // 保存当前位置
    long int pos = ftell(file);

    // 读取当前行内容
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fprintf(stderr, "Error reading line number %d.\n", lineNumber);
        return;
    }

    // 计算当前行长度
    size_t len = strlen(buffer);

    // 将文件指针移到当前行的开头
    fseek(file, pos, SEEK_SET);

    // 写入新的行内容
    fprintf(file, "%s", newLine);

    // 如果新行比旧行短，则需要在文件末尾写入空字符覆盖剩余内容
    if (strlen(newLine) < len) {
        fseek(file, strlen(newLine), SEEK_CUR);
        fprintf(file, "%*s", (int)(len - strlen(newLine)), "");
    }

    printf("Line %d replaced successfully.\n", lineNumber);
}

// 格式化字符串并以换行符结尾
void fillWithSpaces(char *resource, size_t length) {
    size_t len = strlen(resource);

    if (len >= length) {
        // 如果resource的长度大于或等于指定长度，不需要填充
        return;
    }

    // 填充空格到指定长度
    for (size_t i = len; i < length - 2; ++i) {
        resource[i] = ' ';
    }
    resource[length - 2] = '\n'; // 添加换行符
    resource[length-1] = '\0'; // 添加字符串结束符
}


//去除空格和\n
void removeSpacesAndNewlines(char *aim) {
    if (aim == NULL) {
        return; // 如果传入的指针为空，直接返回
    }

    int len = strlen(aim);
    int j = 0;

    for (int i = 0; i < len; i++) {
        if (aim[i] != ' ' && aim[i] != '\n') {
            aim[j++] = aim[i];
        }
    }

    aim[j] = '\0'; // 添加字符串结束符
}


