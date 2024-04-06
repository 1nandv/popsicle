#include "include/lexer.h"
#include "include/token.h"
#include <stdlib.h>

lexer_t *init_lexer(char *input) {
  lexer_t *lexer = malloc(sizeof(lexer_t));
  lexer->input = input;
  lexer->length = sizeof(input);
  lexer->offset = 0;
  lexer->ch = input[0];

  return lexer;
}

void skip_whitespace(lexer_t *lexer) {
  if (lexer->ch == ' ' || lexer->ch == '\t' || lexer->ch == '\n' ||
      lexer->ch == '\r') {
    lexer->ch = lexer->input[lexer->offset++];
    return;
  }
}

char peek(lexer_t *lexer, int offset) {
    return lexer->input[offset++];
}

token_t* tok_next(lexer_t *lexer) {
    token_t* token = calloc(1, sizeof(token_t));
    return token;
}


