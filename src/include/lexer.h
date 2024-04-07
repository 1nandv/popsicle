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

    END,
} Token;

typedef struct SToken {
    Token type;
    char* lexeme;
    // unsigned short offset;
} token_t;

typedef struct SLexer {
    const char* input;
    char ch;
    size_t length;
    size_t offset;
} lexer_t;

lexer_t *init_lexer(char* input);

token_t* make_token(Token type, char* value);

token_t* next(lexer_t* lexer);

void next_offset(lexer_t* lexer);

token_t* read_iden(lexer_t* lexer);

void skip_whitespace(lexer_t* lexer);

char peek(lexer_t *lexer);


#endif

