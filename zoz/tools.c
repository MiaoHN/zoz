#include "tools.h"

#include <stdio.h>
#include <string.h>

static void dump(const char* type, Token* token) {
  char* buf;
  strncpy(buf, token->start, token->length + 1);
  buf[token->length] = '\0';
  printf("%s:%d:%d\t%s\n", type, token->line, token->column, buf);
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
    DUMP(DOLLAR);
    DUMP(RIGHT_PAREN);
    DUMP(LEFT_BRACKET);
    DUMP(RIGHT_BRACKET);
    DUMP(LEFT_BRACE);
    DUMP(RIGHT_BRACE);
    DUMP(EQUAL);
    DUMP(EQUAL_EQUAL);
    DUMP(GREATER);
    DUMP(AMPERSAND_AND);
    DUMP(DOLLAR);
    DUMP(AT);
    DUMP(PERCENT);
    DUMP(HAT);
    DUMP(AMPERSAND);
    DUMP(PARALLEL);
    DUMP(VERTICAL_BAR);
    DUMP(POUND);
    DUMP(QUEST);
    DUMP(GREATER_EQUAL);
    DUMP(LESS);
    DUMP(TLIDE);
    DUMP(LESS_EQUAL);
    DUMP(DOT);
    DUMP(BANG);
    DUMP(BANG_EQUAL);
    DUMP(SEMICOLON);
    DUMP(COLON);
    DUMP(COMMA);
    DUMP(SLASH);
    DUMP(STAR);
    DUMP(NUMBER);
    DUMP(IF);
    DUMP(FUNC);
    DUMP(RETURN);
    DUMP(PRINT);
    DUMP(FOR);
    DUMP(AND);
    DUMP(OR);
    DUMP(FALSE);
    DUMP(CLASS);
    DUMP(TRUE);
    DUMP(IMPORT);
    DUMP(ELSE);
    DUMP(WHILE);
    DUMP(IDENTIFIER);
#undef DUMP
  }
}