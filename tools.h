#ifndef TOOLS_H
#define TOOLS_H


#include "maindef.h"

void getActiveWindowProcessName(char* processName);


void replaceLine(int lineNumber,const char *newLine,FILE *file);

void fillWithSpaces(char *resource, size_t length);

int findLine(FILE *file, const char *aim);

void removeSpacesAndNewlines(char *aim);

#endif // TOOLS_H
