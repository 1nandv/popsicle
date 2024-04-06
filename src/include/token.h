#ifndef TOKEN_H
#define TOKEN_H

typedef enum Token {
    IDEN,
    LPAREN,
    RPAREN,
    COLON,
    RETURN_TYPE_OPERATOR, // ::
    BLOCK_TERMINATOR, // ;;

    // error, some type like in rust
    BANG,
    OPTIONAL,
    
    // math operators
    DIVIDE_OPERATOR,
    PLUS,
    MINUS,
    MULTIPLY,
} token_t;

#endif

