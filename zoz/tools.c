#include "tools.h"

#include <stdio.h>
#include <string.h>

static void dump(const char* type, Token* token) {
  char buf[token->length + 1];
  buf[token->length] = '\0';
  memcpy(buf, token->start, token->length);
  printf("%s:%d:%d %s\n", type, token->line, token->column, buf);
}

void dump_tokens(TokenList* tokens) {
  for (int i = 0; i < tokens->len; ++i) {
    Token token = tokens->buffer[i];
#define DUMP(TYPE)                  \
  if (token.type == TOKEN_##TYPE) { \
    dump(#TYPE, &token);            \
  }
    DUMP(STRING);
    DUMP(MINUS);
    DUMP(PLUS);
    DUMP(LEFT_PAREN);
    DUMP(RIGHT_PAREN);
    DUMP(LEFT_BRACE);
    DUMP(RIGHT_BRACE);
    DUMP(DOT);
    DUMP(SEMICOLON);
    DUMP(COMMA);
    DUMP(SLASH);
    DUMP(STAR);
    DUMP(NUMBER);
#undef DUMP
  }
}