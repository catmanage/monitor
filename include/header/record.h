#ifndef RECORD_H
#define RECORD_H


#include "maindef.h"


void getCurrentDate(char* buffer, size_t size);
void formatTime(clock_t time, char* buffer, size_t size);
void initializeFiles();
void updateFocusTime(ProcessFocusTime* processFocusTime, int f);

#endif // RECORD_H
