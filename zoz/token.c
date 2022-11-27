#include "token.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_LIST_INITIAL_LENGTH 8

/**
 * @brief expand tokenlist buffer's size one time larger than before
 *
 * @param list
 */
static void token_list_expansion(TokenList* list) {
  int old_size = list->alloc;
  int new_size = 2 * old_size;

  Token* new_buffer = (Token*)malloc(sizeof(Token) * new_size);
  memcpy(new_buffer, list->buffer, list->alloc * sizeof(Token));
  free(list->buffer);
  list->buffer = new_buffer;

  list->alloc = new_size;
}

void token_get_str(char* str, Token* token) {
  str[token->length] = '\0';
  memcpy(str, token->start, token->length);
}

void init_token_list(TokenList* list) {
  list->alloc  = TOKEN_LIST_INITIAL_LENGTH;
  list->buffer = (Token*)malloc(sizeof(Token) * TOKEN_LIST_INITIAL_LENGTH);
  list->len    = 0;
}

void free_token_list(TokenList* list) {
  if (list->buffer != NULL) {
    free(list->buffer);
  }
}

void token_list_add_token(TokenList* list, Token token) {
  if (list->len >= list->alloc) {
    token_list_expansion(list);
  }
  list->buffer[list->len++] = token;
}

typedef struct Scanner {
  const char* start;
  const char* current;

  int line;
  int column;
} Scanner;

static bool is_at_end(Scanner* scanner) { return *scanner->current == '\0'; }

static char advance(Scanner* scanner) {
  scanner->current++;
  return scanner->current[-1];
}

static char peek(Scanner* scanner) { return *scanner->current; }

static char peek_next(Scanner* scanner) {
  if (is_at_end(scanner)) return '\0';
  return scanner->current[1];
}

static char peek_next_next(Scanner* scanner) {
  if (peek_next(scanner) == '\0') return '\0';
  return scanner->current[2];
}

static bool match(Scanner* scanner, char expected) {
  if (is_at_end(scanner)) return false;
  if (*scanner->current != expected) return false;
  scanner->current++;
  return true;
}

static Scanner init_scanner(const char* source) {
  Scanner scanner;
  scanner.start   = source;
  scanner.current = source;
  scanner.line    = 1;
  scanner.column  = 1;

  return scanner;
}

static Token error_token(Scanner* scanner, const char* message) {
  Token token;
  token.line   = scanner->line;
  token.column = scanner->column;
  token.type   = TOKEN_ERROR;
  token.start  = message;
  token.length = (int)strlen(message);
  return token;
}

static Token create_token(Scanner* scanner, TokenType type) {
  Token token;
  token.line   = scanner->line;
  token.column = scanner->column;
  token.type   = type;
  token.start  = scanner->start;
  token.length = (int)(scanner->current - scanner->start);
  return token;
}

static Token create_token_string(Scanner* scanner) {
  while (!is_at_end(scanner) && peek(scanner) != '"') {
    if (peek(scanner) == '\\') {
      advance(scanner);
    }
    if (peek(scanner) == '\n') {
      scanner->line++;
    }
    advance(scanner);
  }

  if (is_at_end(scanner)) {
    return error_token(scanner, "Unterminated string");
  }

  // skip first "
  scanner->start++;
  Token token = create_token(scanner, TOKEN_STRING);
  // skip last "
  advance(scanner);
  return token;
}

static bool is_exp_start(Scanner* scanner) {
  if (peek(scanner) != 'e') return false;
  if (peek_next(scanner) == '+' || peek_next(scanner) == '-') {
    if (isdigit(peek_next_next(scanner))) return true;
  }
  return isdigit(peek_next(scanner));
}

/**
 * @brief create number token
 * @details
 *
 * @param scanner
 * @return Token
 */
static Token create_token_number(Scanner* scanner) {
  while (isdigit(peek(scanner))) advance(scanner);
  if (peek(scanner) == '.') {
    advance(scanner);
    while (isdigit(peek(scanner))) advance(scanner);
  }
  if (is_exp_start(scanner)) {
    // digest 'e'
    advance(scanner);
    if (peek(scanner) == '-' || peek(scanner) == '+') advance(scanner);
    if (!isdigit(peek(scanner))) {
      return error_token(scanner, "Unexpected end of number");
    }
    while (isdigit(peek(scanner))) advance(scanner);
  }

  return create_token(scanner, TOKEN_NUMBER);
}

/**
 * @brief skip whitespace, tab .etc
 * @todo skip comment
 *
 * @param scanner
 */
static void skip_nonsense(Scanner* scanner) {
  while (true) {
    switch (*scanner->current) {
      case ' ':
      case '\r':
      case '\t': {
        scanner->current++;
        break;
      }
      case '/': {
        if (peek_next(scanner)) {
          while (peek(scanner) != '\n' && !is_at_end(scanner)) advance(scanner);
        } else {
          return;
        }
        break;
      }
      case '\n': {
        scanner->line++;
        scanner->column = 1;
        scanner->current++;
        break;
      }
      default:
        scanner->column += scanner->current - scanner->start;
        scanner->start = scanner->current;
        return;
    }
  }
}

static Token scan_token(Scanner* scanner) {
  skip_nonsense(scanner);

  if (is_at_end(scanner)) {
    return create_token(scanner, TOKEN_EOF);
  }

  switch (advance(scanner)) {
    case '(':
      return create_token(scanner, TOKEN_LEFT_PAREN);
    case ')':
      return create_token(scanner, TOKEN_RIGHT_PAREN);
    case '{':
      return create_token(scanner, TOKEN_LEFT_BRACE);
    case '}':
      return create_token(scanner, TOKEN_RIGHT_BRACE);
    case '.': {
      if (isdigit(peek(scanner))) {
        return create_token_number(scanner);
      } else {
        return create_token(scanner, TOKEN_DOT);
      }
    }
    case ';':
      return create_token(scanner, TOKEN_SEMICOLON);
    case ',':
      return create_token(scanner, TOKEN_COMMA);
    case '-': {
      if (isdigit(peek(scanner))) {
        return create_token_number(scanner);
      } else if (peek(scanner) == '.' && isdigit(peek_next(scanner))) {
        return create_token_number(scanner);
      } else {
        return create_token(scanner, TOKEN_MINUS);
      }
    }
    case '+': {
      if (isdigit(peek(scanner))) {
        return create_token_number(scanner);
      } else if (peek(scanner) == '.' && isdigit(peek_next(scanner))) {
        return create_token_number(scanner);
      } else {
        return create_token(scanner, TOKEN_PLUS);
      }
    }
    case '/':
      return create_token(scanner, TOKEN_SLASH);
    case '*':
      return create_token(scanner, TOKEN_STAR);
    case '!': {
      if (match(scanner, '=')) {
        return create_token(scanner, TOKEN_BANG_EQUAL);
      } else {
        return create_token(scanner, TOKEN_BANG);
      }
    }
    case '=': {
      if (match(scanner, '=')) {
        return create_token(scanner, TOKEN_EQUAL_EQUAL);
      } else {
        return create_token(scanner, TOKEN_EQUAL);
      }
    }
    case '<': {
      if (match(scanner, '=')) {
        return create_token(scanner, TOKEN_LESS_EQUAL);
      } else {
        return create_token(scanner, TOKEN_LESS);
      }
    }
    case '>': {
      if (match(scanner, '=')) {
        return create_token(scanner, TOKEN_GREATER_EQUAL);
      } else {
        return create_token(scanner, TOKEN_GREATER);
      }
    }
    case '"':
      return create_token_string(scanner);
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return create_token_number(scanner);
    default:
      return error_token(scanner, "Unexpected charactor");
  }
}

TokenList tokenizer(const char* source) {
  TokenList token_list;

  init_token_list(&token_list);

  Scanner scanner = init_scanner(source);

  while (*scanner.current != '\0') {
    Token token = scan_token(&scanner);
    token_list_add_token(&token_list, token);
    if (token.type == TOKEN_ERROR) {
      printf("%d:%d %s\n", token.line, token.column, token.start);
      // TODO
      break;
    }
  }

  return token_list;
}