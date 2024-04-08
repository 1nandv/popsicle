#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/lexer.h"
#include "include/io.h"

lexer_t *init_lexer(char *path) {
    char *input = readfile(path);
    lexer_t *lexer = malloc(sizeof(lexer_t));

    if (lexer == NULL) {
        fprintf(stderr, "fatal: not enough memory available\n");
        return NULL;
    }

    lexer->input = input;
    lexer->length = strlen(input);
    lexer->offset = 0;
    lexer->ch = input[lexer->offset];

    return lexer;
}

token_t *make_token(typeof_token type, char *value) {
    token_t *token = malloc(sizeof(token_t));

    if (token == NULL) {
        fprintf(stderr, "fatal: not enough memory available\n");
        return NULL;
    }

    token->type = type;
    token->value = value;

    return token;
}

void movechar(lexer_t *lexer) {
    if (lexer->offset < lexer->length && lexer->ch != '\0') {
        lexer->ch = lexer->input[++lexer->offset];
    }
}

void skip_whitespace(lexer_t* lexer) {
    while (lexer->ch == ' ' || lexer->ch == '\t' || lexer->ch == '\r' || lexer->ch == '\n') {
        movechar(lexer);
    }
}

short isint(char ch) {
    if (ch >= 30 && ch <= 39) return 1;
    return 0;
}

const char *stringify_token_type(typeof_token type) {
    switch (type) {
        case IDEN: return "IDEN";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case COLON: return "COLON";
        case SEMI: return "SEMI";
        case ASSIGNMENT: return "ASSIGNMENT";
        case RETURN_TYPE_OPERATOR: return "RETURN_TYPE_OPERATOR";
        case BLOCK_TERMINATOR: return "BLOCK_TERMINATOR";
        case BANG: return "BANG";
        case OPTIONAL: return "OPTIONAL";
        case INT: return "INT";
        case STRING: return "STRING";
        case FLOAT: return "FLOAT";
        case DIVIDE: return "DIVIDE";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case MULTIPLY: return "MULTIPLY";
        case ILLEGAL: return "ILLEGAL";
        case END: return "END";
    }

    return "UNDEFINED";
}

void print_token(token_t *token) {
    #ifdef DEBUG
        const char *type = stringify_token_type(token->type);
        char *template = "<type=%s, int_type=%d, value=%s>\n";
        printf(template, type, token->type, (char*) token->value);
    #endif 
}

void read_next(lexer_t *lexer) {
    token_t *token;

    while(lexer->offset < lexer->length) {
        while (isspace(lexer->ch)) movechar(lexer);

        token = read_token(lexer);

        if (token->type == END) {
            free(token);
            return;
        }

        print_token(token);

        movechar(lexer);
    }
}

token_t *read_iden(lexer_t *lexer) {
    size_t start = lexer->offset;

    do { movechar(lexer); } while (!isspace(lexer->ch));

    size_t length = lexer->length - start;
    char *string = malloc(length);
    strncpy(string, lexer->input+start, lexer->offset - start);
    string[length] = '\0';

    return make_token(IDEN, string);
}

token_t *read_number(lexer_t *lexer) {
    size_t start = lexer->offset;

    do { movechar(lexer); } while (isdigit(lexer->ch));

    size_t length = lexer->length - start;
    char *string = malloc(length);
    strncpy(string, lexer->input+start, lexer->offset - start);
    string[length] = '\0';

    return make_token(INT, string);
}


token_t *read_string(lexer_t *lexer) {
    size_t start = lexer->offset + 1;

    do { movechar(lexer); } while (lexer->ch != '"');

    size_t length = lexer->length - start;
    char *string = malloc(length);
    strncpy(string, lexer->input+start, lexer->offset - start);
    string[length] = '\0';

    return make_token(STRING, string);
}

token_t *read_token(lexer_t *lexer) {
    token_t *token;

    if (lexer->offset >= lexer->length) {
        return make_token(END, NULL);
    } 

    switch (lexer->ch) {
        case '(':
            token = make_token(LPAREN, NULL);
            break;

        case ')':
            token = make_token(RPAREN, NULL);
            break;

        case '+':
            token = make_token(PLUS, NULL);
            break;

        case '-':
            token = make_token(MINUS, NULL);
            break;

        case '=':
            token = make_token(ASSIGNMENT, NULL);
            break;

        case '*':
            token = make_token(MULTIPLY, NULL);
            break;

        case '/':
            token = make_token(DIVIDE, NULL);
            break;

        case ':':
            if (lexer->input[lexer->offset + 1] == ':') {
                token = make_token(RETURN_TYPE_OPERATOR, NULL);
                movechar(lexer);
            } else {
                token = make_token(COLON, NULL);
            }
            break;


        case ';':
            if (lexer->input[lexer->offset + 1] == ';') {
                movechar(lexer);
                token = make_token(BLOCK_TERMINATOR, NULL);
            } else {
                token = make_token(SEMI, NULL);
            }
            break;

        case '\0':
        case EOF:
            token = make_token(END, NULL);
            break;

        default:
            if (lexer->ch == '"') token = read_string(lexer);
            else if (isalpha(lexer->ch)) token = read_iden(lexer);
            else if (isdigit(lexer->ch)) token = read_number(lexer);
            else token = make_token(ILLEGAL, &lexer->ch);
            break;
    }

    return token;
}



