#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/lexer.h"

int main(int argc, char** argv) {
    if (argc <= 1) {
        fprintf(stderr, "usage: popsicle [options] [command]\n");
        return 1; 
    }

    lexer_t *lexer = init_lexer(argv[1]);
    read_next(lexer); 

    if (lexer != NULL) free(lexer);

    return 0;

}

