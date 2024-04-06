#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "token.h"

struct SLexer {
    char* input;
    char ch;
    size_t length;
    size_t offset;
};

typedef struct SLexer lexer_t;

lexer_t *init_lexer(char *input);

char peek(lexer_t *lexer, int offset);

void skip_whitespace(lexer_t *lexer);

void next(lexer_t *lexer);

#endif

