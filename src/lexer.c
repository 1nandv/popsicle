#include "include/lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lexer_t *init_lexer(char* input) {
    lexer_t *lexer = malloc(sizeof(lexer_t));

    lexer->input = input;
    lexer->length = sizeof(input);
    lexer->offset = 0;
    lexer->ch = input[0];

    return lexer;
}

token_t* make_token(Token type, char* value) {
    token_t* token = malloc(sizeof(token_t));

    token->type = type;
    token->lexeme = value;

    return token;
}

void skip_whitespace(lexer_t *lexer) {
  if (isspace(lexer->ch)) {
    lexer->ch = lexer->input[lexer->offset++];
  }
}

char peek(lexer_t *lexer) {
    if (lexer->offset >= lexer->length) {
        return '\0';
    }

    return lexer->input[lexer->offset + 1];
}

void next_offset(lexer_t *lexer) {
    lexer->ch = lexer->input[lexer->offset++];     
}

token_t* read_iden(lexer_t* lexer) {
    size_t start = lexer->offset;

    while (isalpha(lexer->ch)) {
        next_offset(lexer);
    }
        
    char* lexeme = malloc((lexer->length - start) + 1);
    strncpy(lexeme, lexer->input+start, lexer->offset); 

    lexeme[sizeof(lexeme)] = '\0';

    next_offset(lexer);
    
    return make_token(IDEN, lexeme);
}

token_t* next(lexer_t *lexer) {
    skip_whitespace(lexer);

    switch (lexer->ch) {
        case '(':
            return make_token(LPAREN, NULL);
            break;

        case ')':
            return make_token(RPAREN, NULL);
            break;

        case '!':
            return make_token(BANG, NULL); 
            break;

        case '?':
            return make_token(OPTIONAL, NULL); 
            break;

        case '+':
            return make_token(PLUS, NULL); 
            break;

        case '-':
            return make_token(MINUS, NULL); 
            break;

        case '=':
            return make_token(ASSIGNMENT, NULL); 
            break;

        case '/':
            return make_token(DIVIDE, NULL); 
            break;

        case '*':
            return make_token(MULTIPLY, NULL); 
            break;

        case ':':
            if (peek(lexer) == ':') {
                lexer->ch = lexer->input[lexer->offset++];
                return make_token(RETURN_TYPE_OPERATOR, NULL); 
                break;
            }

            return make_token(COLON, NULL);
            break;

        case ';':
            if (peek(lexer) == ';') {
                lexer->ch = lexer->input[lexer->offset++];
                return make_token(BLOCK_TERMINATOR, NULL); 
                break;
            }

            return make_token(SEMI, NULL);
            break;

        case '\0':
            return make_token(END, NULL);
    }

}

