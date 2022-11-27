#ifndef __ZOZ_TOKEN_H__
#define __ZOZ_TOKEN_H__

/**
 * @brief String, number, operators and keywords
 *
 */
typedef enum TokenType {
  TOKEN_LEFT_PAREN,     // `(`
  TOKEN_RIGHT_PAREN,    // `)`
  TOKEN_RIGHT_BRACKET,  // `[`
  TOKEN_LEFT_BRACKET,   // `]`
  TOKEN_LEFT_BRACE,     // `{`
  TOKEN_RIGHT_BRACE,    // `}`
  TOKEN_COMMA,          // `,`
  TOKEN_DOT,            // `.`
  TOKEN_DOLLAR,         // `$`
  TOKEN_AT,             // `@`
  TOKEN_PERCENT,        // `%`
  TOKEN_HAT,            // `^`
  TOKEN_MINUS,          // `-`
  TOKEN_PLUS,           // `+`
  TOKEN_SEMICOLON,      // `;`
  TOKEN_SLASH,          // `/`
  TOKEN_STAR,           // `*`
  TOKEN_BANG,           // `!`
  TOKEN_VERTICAL_BAR,   // `|`
  TOKEN_PARALLEL,       // `||`
  TOKEN_POUND,          // `#`
  TOKEN_TLIDE,          // `~`
  TOKEN_COLON,          // `:`
  TOKEN_QUEST,          // `?`
  TOKEN_AMPERSAND,      // `&`
  TOKEN_AMPERSAND_AND,  // `&&`
  TOKEN_BANG_EQUAL,     // `!=`
  TOKEN_EQUAL,          // `=`
  TOKEN_EQUAL_EQUAL,    // `==`
  TOKEN_GREATER,        // `>`
  TOKEN_GREATER_EQUAL,  // `>=`
  TOKEN_LESS,           // `<`
  TOKEN_LESS_EQUAL,     // `<=`
  TOKEN_IMPORT,         // `import`
  TOKEN_FUNC,           // `func`
  TOKEN_RETURN,         // `return`
  TOKEN_WHILE,          // `while`
  TOKEN_FOR,            // `for`
  TOKEN_IF,             // `if`
  TOKEN_ELSE,           // `else`
  TOKEN_IDENTIFIER,     // identifier
  TOKEN_STRING,         // string
  TOKEN_NUMBER,         // number
  TOKEN_AND,            // `and`
  TOKEN_OR,             // 'or'
  TOKEN_CLASS,          // `class`
  TOKEN_TRUE,           // `true`
  TOKEN_FALSE,          // `false`
  TOKEN_PRINT,          // `print` NOTICE: for debug convenience
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