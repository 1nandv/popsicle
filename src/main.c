#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/io.h"
#include "include/lexer.h"

int main(int argc, char** argv) {
    ++argv, --argc; // skip program file name
    
    if (argc == 0) {
        printf("usage: popsicle [options] [command]\n");
        return 1; 
    }

    FILE *fp = openfile(argv[0]);
    lexer_t *lexer = init_lexer(fp);

    read_next(lexer); 

    free(lexer->input);
    free(lexer);

    if (fclose(fp)) {
        fprintf(stderr, "fatal: unable to close file `%s`\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;

}

