#ifndef IO_H
#define IO_H

#include <stdio.h>

FILE *openfile(char *path);
char *readfile(FILE *fp);

#endif

