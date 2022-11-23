#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef WIO_H
#define WIO_H

char* WIO_LoadSource(char* file);
void WIO_WriteFile(char* file, char* contents);

#endif