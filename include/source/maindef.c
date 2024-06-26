#include "../header/maindef.h"

ProcessFocusTime focusTimes[MAX_PROCESSES];
int numProcesses = 0;
clock_t focusStartTime = 0;
HWND hwndMain = NULL;
char line[LINELENGTH] = "";

