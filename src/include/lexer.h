#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

typedef enum EToken {
    IDEN,
    LPAREN,
    RPAREN,
    COLON,
    SEMI,
    ASSIGNMENT,
    RETURN_TYPE_OPERATOR, // ::
    BLOCK_TERMINATOR, // ;;

    // error, some type like in rust
    BANG,
    OPTIONAL,

    INT,
    STRING,
    FLOAT,

    // math operators
    DIVIDE,
    PLUS,
    MINUS,
    MULTIPLY,

    ILLEGAL,
    END,
} typeof_token;

typedef struct SToken {
    typeof_token type;
    char *value;
    // unsigned short offset;
} token_t;

typedef struct SLexer {
    const char *input;
    char ch;
    size_t length;
    size_t offset;
} lexer_t;

lexer_t *init_lexer(char *input);

token_t *make_token(typeof_token type, char *value);

token_t *read_token(lexer_t *lexer);

void print_token(token_t *token);

void read_next(lexer_t *lexer);

#endif

