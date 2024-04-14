#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/io.h"

FILE *openfile(char *path) {
    FILE *fp = fopen(path, "r");

    if (fp == NULL) {
        fprintf(stderr, "fatal: unable to open file `%s`\n", path);
        exit(1);
    }

    return fp;
}

char *readfile(FILE *fp) {
    char *buffer;

    size_t bufsize = 4;
    size_t offset = 0;

    char ch;

    buffer = malloc(bufsize);

    if (buffer == NULL) {
        fprintf(stderr, "fatal: not enough memory available\n");
        exit(1);
    }

    while (ch = fgetc(fp), ch != EOF) {
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

    if (ch == EOF && offset == 0) {
        free(buffer);
        return NULL;
    }

    buffer[offset] = '\0';

    if (bufsize > offset) {
        char *newbuf = realloc(buffer, offset);
        if (newbuf != NULL) buffer = newbuf;
    }

    return buffer;
}

