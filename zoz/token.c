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

static TokenType pattern_match(Scanner* scanner, const char* pattern, int len,
                               TokenType type) {
  if (scanner->current - scanner->start == len) {
    if (!memcmp(scanner->start, pattern, len)) {
      return type;
    }
  }
  return TOKEN_IDENTIFIER;
}

/**
 * @brief get keyword type of scanner
 * @details if string in scanner is not a key word, return TOKEN_IDENTIFIER
 * @todo add all keywords
 *
 * @param scanner
 * @return TokenType
 */
static TokenType check_keyword(Scanner* scanner) {
  switch (scanner->start[0]) {
    case 'i': {
      switch (scanner->start[1]) {
        case 'f':
          return pattern_match(scanner, "if", 2, TOKEN_IF);
        case 'm':
          return pattern_match(scanner, "import", 6, TOKEN_IMPORT);
      }
      break;
    }
    case 'c':
      return pattern_match(scanner, "class", 5, TOKEN_CLASS);
    case 'w':
      return pattern_match(scanner, "while", 5, TOKEN_WHILE);
    case 'e':
      return pattern_match(scanner, "else", 4, TOKEN_ELSE);
    case 'a':
      return pattern_match(scanner, "and", 3, TOKEN_AND);
    case 'o':
      return pattern_match(scanner, "or", 2, TOKEN_OR);
    case 'f': {
      switch (scanner->start[1]) {
        case 'o':
          return pattern_match(scanner, "for", 3, TOKEN_FOR);
        case 'a':
          return pattern_match(scanner, "false", 5, TOKEN_FALSE);
        case 'u':
          return pattern_match(scanner, "func", 4, TOKEN_FUNC);
      }
      break;
    }
    case 't':
      return pattern_match(scanner, "true", 4, TOKEN_TRUE);
    case 'r':
      return pattern_match(scanner, "return", 6, TOKEN_RETURN);
    case 'p':
      return pattern_match(scanner, "print", 5, TOKEN_PRINT);
    default:
      break;
  }
  return TOKEN_IDENTIFIER;
}

static bool is_alpha(char c) { return isalpha(c) || c == '_'; }

/**
 * @brief create identifier or keywords
 * @details first check if the word is a keyword, if so, create corresponding
 * token, if not, create identifier token
 *
 * @param scanner
 * @return Token
 */
static Token create_token_identifier(Scanner* scanner) {
  while (isdigit(peek(scanner)) || is_alpha(peek(scanner))) {
    advance(scanner);
  }
  return create_token(scanner, check_keyword(scanner));
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
  if (isdigit(peek(scanner))) {
    return create_token_number(scanner);
  }
  if (is_alpha(peek(scanner))) {
    return create_token_identifier(scanner);
  }

  switch (advance(scanner)) {
    case '(':
      return create_token(scanner, TOKEN_LEFT_PAREN);
    case ')':
      return create_token(scanner, TOKEN_RIGHT_PAREN);
    case '[':
      return create_token(scanner, TOKEN_LEFT_BRACKET);
    case ']':
      return create_token(scanner, TOKEN_RIGHT_BRACKET);
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
      break;
    }
    case ';':
      return create_token(scanner, TOKEN_SEMICOLON);
    case ',':
      return create_token(scanner, TOKEN_COMMA);
    case '|': {
      if (match(scanner, '|')) {
        return create_token(scanner, TOKEN_PARALLEL);
      } else {
        return create_token(scanner, TOKEN_VERTICAL_BAR);
      }
      break;
    }
    case '$':
      return create_token(scanner, TOKEN_DOLLAR);
    case '@':
      return create_token(scanner, TOKEN_AT);
    case '%':
      return create_token(scanner, TOKEN_PERCENT);
    case '#':
      return create_token(scanner, TOKEN_POUND);
    case '~':
      return create_token(scanner, TOKEN_TLIDE);
    case ':':
      return create_token(scanner, TOKEN_COLON);
    case '^':
      return create_token(scanner, TOKEN_HAT);
    case '?':
      return create_token(scanner, TOKEN_QUEST);
    case '&': {
      if (match(scanner, '&')) {
        return create_token(scanner, TOKEN_AMPERSAND_AND);
      } else {
        return create_token(scanner, TOKEN_AMPERSAND);
      }
      break;
    }
    case '-': {
      if (isdigit(peek(scanner))) {
        return create_token_number(scanner);
      } else if (peek(scanner) == '.' && isdigit(peek_next(scanner))) {
        return create_token_number(scanner);
      } else {
        return create_token(scanner, TOKEN_MINUS);
      }
      break;
    }
    case '+': {
      if (isdigit(peek(scanner))) {
        return create_token_number(scanner);
      } else if (peek(scanner) == '.' && isdigit(peek_next(scanner))) {
        return create_token_number(scanner);
      } else {
        return create_token(scanner, TOKEN_PLUS);
      }
      break;
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
      break;
    }
    case '=': {
      if (match(scanner, '=')) {
        return create_token(scanner, TOKEN_EQUAL_EQUAL);
      } else {
        return create_token(scanner, TOKEN_EQUAL);
      }
      break;
    }
    case '<': {
      if (match(scanner, '=')) {
        return create_token(scanner, TOKEN_LESS_EQUAL);
      } else {
        return create_token(scanner, TOKEN_LESS);
      }
      break;
    }
    case '>': {
      if (match(scanner, '=')) {
        return create_token(scanner, TOKEN_GREATER_EQUAL);
      } else {
        return create_token(scanner, TOKEN_GREATER);
      }
      break;
    }
    case '"':
      return create_token_string(scanner);
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