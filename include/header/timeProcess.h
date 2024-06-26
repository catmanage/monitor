#ifndef TIMEPROCESS_H
#define TIMEPROCESS_H

#include "maindef.h"
#include "record.h"


void maintainProce(char *processName);
int initTotalRecord(ProcessFocusTime* processFocusTime);
int initDateRecord(ProcessFocusTime* processFocusTime);
void createDateRecord(ProcessFocusTime* processFocusTime);
int findpnum(char* processName);
void save(ProcessFocusTime* processFocusTime);

#endif // TIMEPROCESS_H
