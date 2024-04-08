#include <stdio.h>
#include <stdlib.h>
#include "include/io.h"

char *readfile(char *path) {
    FILE *fp;
    char *buffer;

    size_t bufsize = 4;
    size_t offset = 0;

    char ch;

    fp = fopen(path, "r");

    if (fp == NULL) {
        fprintf(stderr, "fatal: unable to open file `%s`\n", path);
        exit(1);
    }

    buffer = malloc(bufsize);

    while ((ch = fgetc(fp)) != EOF) {
        if (offset >= bufsize - 1) {
            bufsize *= 2;

            char *newbuf = realloc(buffer, bufsize);

            if (newbuf == NULL) {
                fprintf(stderr, "fatal: not enough memory available to read file\n");
                free(buffer);
                exit(1);
            }

            buffer = newbuf;
        }

        buffer[offset++] = ch;
    }

    buffer[offset] = '\0';

    if (bufsize > offset) {
        char *newbuf = realloc(buffer, offset);

        if (newbuf != NULL) {
            buffer = newbuf;
            bufsize = offset;
        }
    }

    return buffer;
}

