#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/lexer.h"
#include "include/io.h"

// private functions

static void movechar(lexer_t *lexer) {
    if (lexer->offset < lexer->length && lexer->ch != '\0') {
        lexer->ch = lexer->input[++lexer->offset];
    }
}

static void prevchar(lexer_t *lexer) {
    if (lexer->offset > 0) {
        lexer->ch = lexer->input[--lexer->offset];
    }
}

static short should_skip(char ch) {
    return ch == '\n' || ch == '\t' || ch == ' ' || ch == '\r';
}

static void skip_whitespace(lexer_t* lexer) {
    while (should_skip(lexer->ch), lexer->offset >= lexer->length) {
        movechar(lexer);
    }
}

// checks if character is apostrophe
static short isstring(char ch) {
    return ch == '"';
}

static short isiden(char ch) {
    return isalpha(ch) || ch == '_';
}

// for now only ascii digits, but will support hexadecimal values in future
static short isint(char ch) {
    return isdigit(ch);
}

static token_t *read_iden(lexer_t *lexer) {
    size_t start = lexer->offset;

    while (!should_skip(lexer->ch) && isalpha(lexer->ch)) 
        movechar(lexer);

    size_t length = (lexer->length - start) + 1;
    char *string = malloc(length);
    strncpy(string, lexer->input+start, lexer->offset - start);
    string[length] = '\0';

    prevchar(lexer);

    return make_token(IDEN, string);
}

static token_t *read_int(lexer_t *lexer) {
    size_t start = lexer->offset;

    do { movechar(lexer); } while (isint(lexer->ch));

    size_t length = lexer->length - start;
    char *string = malloc(length);
    strncpy(string, lexer->input+start, lexer->offset - start);
    string[length] = '\0';

    // cast to int and then back to char*
    // probably inefficient but it just works
    int cast = atoi(string);
    sprintf(string, "%d", cast);

    return make_token(INT, string);
}


static token_t *read_string(lexer_t *lexer) {
    size_t start = lexer->offset + 1;

    do { movechar(lexer); } while (lexer->ch != '"');

    size_t length = lexer->length - start;
    char *string = malloc(length);
    strncpy(string, lexer->input+start, lexer->offset - start);
    string[length] = '\0';

    return make_token(STRING, string);
}

static const char *stringify_token_type(typeof_token type) {
    switch (type) {
        case IDEN: return "IDEN";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case COLON: return "COLON";
        case SEMI: return "SEMI";
        case COMMA: return "COMMA";
        case ASSIGNMENT: return "ASSIGNMENT";
        case RETURN_TYPE_OPERATOR: return "RETURN_TYPE_OPERATOR";
        case BLOCK_TERMINATOR: return "BLOCK_TERMINATOR";
        case BANG: return "BANG";
        case SOME: return "SOME";
        case INT: return "INT";
        case STRING: return "STRING";
        case FLOAT: return "FLOAT";
        case DIVIDE: return "DIVIDE";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case MULTIPLY: return "MULTIPLY";
        case ILLEGAL: return "ILLEGAL";
        case END: return "END";
        case LOGICAL_OR:  return "LOGICAL_OR";
        case LOGICAL_AND: return "LOGICAL_AND";
        case PIPE_OPERATOR: return "PIPE_OPERATOR";
        case NOT_EQUAL: return "NOT_EQUAL";
        case EQUAL: return "EQUAL";
    }

    return "UNDEFINED";
}

static char peek(lexer_t *lexer) {
    return lexer->input[lexer->offset + 1];
}

// public functions

void read_next(lexer_t *lexer) {
    token_t *token;

    while(lexer->offset < lexer->length) {
        token = read_token(lexer);

        print_token(token);

        if (token->type == END) {
            if (token->value) free(token->value);
            return free(token);
        }

        movechar(lexer);

        if (token->value) free(token->value);

        free(token);
    }
}

token_t *read_token(lexer_t *lexer) {
    token_t *token;

    skip_whitespace(lexer);

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

        case '!':
            if (peek(lexer) == '=') {
                token = make_token(NOT_EQUAL, NULL);
                movechar(lexer);
            } else token = make_token(BANG, NULL);
            break;

        case '=':
            if (peek(lexer) == '=') {
                token = make_token(EQUAL, NULL);
                movechar(lexer);
            } else token = make_token(ASSIGNMENT, NULL);
            break;

        case '*':
            token = make_token(MULTIPLY, NULL);
            break;

        case '/':
            token = make_token(DIVIDE, NULL);
            break;

        case '?':
            token = make_token(SOME, NULL);
            break;

        case ',':
            token = make_token(COMMA, NULL);
            break;

        case '&':
            if (peek(lexer) == '&') { 
                token = make_token(LOGICAL_OR, NULL);
                movechar(lexer);
            }

            break;

        case '|':
            if (peek(lexer) == '|') { 
                token = make_token(LOGICAL_OR, NULL);
                movechar(lexer);
            } else if (peek(lexer) == '>') {
                token = make_token(PIPE_OPERATOR, NULL);
                movechar(lexer);
            } 

            break;

        case ':':
            if (peek(lexer) == ':') {
                token = make_token(RETURN_TYPE_OPERATOR, NULL);
                movechar(lexer);
            } else token = make_token(COLON, NULL);

            break;


        case ';':
            if (peek(lexer) == ';') {
                token = make_token(BLOCK_TERMINATOR, NULL);
                movechar(lexer);
            } else token = make_token(SEMI, NULL);

            break;

        case '\0':
        case EOF:
            token = make_token(END, NULL);
            break;

        default:
            if (isstring(lexer->ch)) token = read_string(lexer);
            else if (isiden(lexer->ch)) token = read_iden(lexer);
            else if (isint(lexer->ch)) token = read_int(lexer);
            else token = make_token(ILLEGAL, NULL); // todo: assign illegal character

            break;
    }

    return token;
}

void print_token(token_t *token) {
#ifdef DEBUG
    const char *type = stringify_token_type(token->type);
    char *template = "[ type=%s, int_type=%d, value=%s ]\n";
    printf(template, type, token->type, (char*) token->value);
#endif 
}


lexer_t *init_lexer(FILE *fp) {
    char *input = readfile(fp);
    lexer_t *lexer = malloc(sizeof(lexer_t));

    if (lexer == NULL) {
        fprintf(stderr, "fatal: not enough memory available\n");
        return NULL;
    }

    lexer->input = input;
    lexer->length = strlen(input) + 1;
    lexer->offset = 0;
    lexer->ch = input[0];

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

