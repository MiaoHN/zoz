#ifndef __ZOZ_TOKEN_H__
#define __ZOZ_TOKEN_H__

typedef enum TokenType {
  TOKEN_LEFT_PAREN,     // (
  TOKEN_RIGHT_PAREN,    // )
  TOKEN_LEFT_BRACE,     // {
  TOKEN_RIGHT_BRACE,    // }
  TOKEN_COMMA,          // ,
  TOKEN_DOT,            // .
  TOKEN_MINUS,          // -
  TOKEN_PLUS,           // +
  TOKEN_SEMICOLON,      // ;
  TOKEN_SLASH,          // /
  TOKEN_STAR,           // *
  TOKEN_BANG,           // !
  TOKEN_BANG_EQUAL,     // !=
  TOKEN_EQUAL,          // =
  TOKEN_EQUAL_EQUAL,    // ==
  TOKEN_GREATER,        // >
  TOKEN_GREATER_EQUAL,  // >=
  TOKEN_LESS,           // <
  TOKEN_LESS_EQUAL,     // <=
  TOKEN_IDENTIFIER,
  TOKEN_STRING,
  TOKEN_NUMBER,
  TOKEN_AND,
  TOKEN_CLASS,
  TOKEN_ELSE,
  TOKEN_FALSE,
  TOKEN_FOR,
  TOKEN_FUN,
  TOKEN_IF,
  TOKEN_NIL,
  TOKEN_OR,
  TOKEN_PRINT,
  TOKEN_RETURN,
  TOKEN_SUPER,
  TOKEN_THIS,
  TOKEN_TRUE,
  TOKEN_VAR,
  TOKEN_WHILE,
  TOKEN_FUNCTION,
  TOKEN_EOF,
  TOKEN_ERROR,
} TokenType;

typedef struct Token {
  TokenType   type;
  const char* start;

  int length;

  int line;
  int column;
} Token;

void token_get_str(char* str, Token* token);

typedef struct TokenList {
  Token* buffer;

  int len;
  int alloc;
} TokenList;

void init_token_list(TokenList* list);

void free_token_list(TokenList* list);

void token_list_add_token(TokenList* list, Token token);

/**
 * @brief tokenizer source code
 * @attention use free_token_list() to free memory!!!
 *
 * @param source
 * @return TokenList
 */
TokenList tokenizer(const char* source);

#endif  // __ZOZ_TOKEN_H__