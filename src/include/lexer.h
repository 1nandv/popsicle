#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

typedef enum TokenType {
    IDEN,
    LPAREN,
    RPAREN,
    COLON,
    SEMI,
    COMMA,

    ASSIGNMENT,
    RETURN_TYPE_OPERATOR, // ::
    BLOCK_TERMINATOR, // ;;

    // error, some type like in rust
    BANG,
    SOME,

    // types
    INT,
    STRING,
    FLOAT,

    // math operators
    DIVIDE,
    PLUS,
    MINUS,
    MULTIPLY,

    // comparison
    LOGICAL_OR,
    LOGICAL_AND,
    NOT_EQUAL,
    EQUAL,

    // pipe operator like in elixir
    PIPE_OPERATOR,

    ILLEGAL,
    END,
} typeof_token;

typedef struct SToken {
    typeof_token type;
    char *value;
    // unsigned short offset;
} token_t;

typedef struct SLexer {
    char *input;
    char ch;
    size_t length;
    size_t offset;
} lexer_t;

lexer_t *init_lexer(FILE *fp);

token_t *make_token(typeof_token type, char *value);

token_t *read_token(lexer_t *lexer);

void print_token(token_t *token);

void read_next(lexer_t *lexer);

#endif

