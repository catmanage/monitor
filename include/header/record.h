#ifndef RECORD_H
#define RECORD_H


#include "maindef.h"


void getCurrentDate(char*date, size_t size);
void formatTime(clock_t time, char* buffer, size_t size);
void initializeFiles();
void updateFocusTime(ProcessFocusTime* processFocusTime, int f);
void add(ProcessFocusTime* processFocusTime);

#endif // RECORD_H
