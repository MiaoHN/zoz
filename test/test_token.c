#include "test.h"
#include "token.h"

TEST(Number, Integer) {
  char* numbers[] = {"1", "+1", "-1", "000",
                     "111111111111111111111111111111111111111111111111111111111"
                     "1111111111111111111111111111111111111111111"};
  char  source[] =
      "1 +1 -1 000 "
      "111111111111111111111111111111111111111111111111111111111111111111111111"
      "1111111111111111111111111111";

  TokenList list = tokenizer(source);
  EXPECT_TRUE(list.len == sizeof(numbers) / sizeof(char*));

  for (int i = 0; i < list.len; ++i) {
    Token token = list.buffer[i];
    char  str[token.length + 1];
    token_get_str(str, &token);
    EXPECT_TRUE(list.buffer[0].type == TOKEN_NUMBER);
    EXPECT_STREQ(numbers[i], str);
  }
}

TEST(Number, Decimal) {
  char* numbers[] = {"0.1", ".1", "1.", "11111.11111"};
  char  source[]  = "0.1 .1 1. 11111.11111";

  TokenList list = tokenizer(source);
  EXPECT_TRUE(list.len == sizeof(numbers) / sizeof(char*));

  for (int i = 0; i < list.len; ++i) {
    Token token = list.buffer[i];
    char  str[token.length + 1];
    token_get_str(str, &token);
    EXPECT_TRUE(list.buffer[0].type == TOKEN_NUMBER);
    EXPECT_STREQ(numbers[i], str);
  }
}

TEST(Number, Sentific) {
  char* numbers[] = {"1e1",   "1e+1",  "1e-1", "+1e+1",
                     "-1e-1", "1.1e1", ".1e1", "-.1e1"};
  char  source[]  = "1e1 1e+1 1e-1 +1e+1 -1e-1 1.1e1 .1e1 -.1e1";

  TokenList list = tokenizer(source);
  EXPECT_TRUE(list.len == sizeof(numbers) / sizeof(char*));

  for (int i = 0; i < list.len; ++i) {
    Token token = list.buffer[i];
    char  str[token.length + 1];
    token_get_str(str, &token);
    EXPECT_TRUE(list.buffer[0].type == TOKEN_NUMBER);
    EXPECT_STREQ(numbers[i], str);
  }
}

TEST(String, Common) {
  char* strings[] = {"Hello", "World", "!"};
  char  source[]  = "\"Hello\" \"World\" \"!\"";

  TokenList list = tokenizer(source);
  EXPECT_TRUE(list.len == sizeof(strings) / sizeof(char*));

  for (int i = 0; i < list.len; ++i) {
    Token token = list.buffer[i];
    char  str[token.length + 1];
    token_get_str(str, &token);
    EXPECT_TRUE(list.buffer[0].type == TOKEN_STRING);
    EXPECT_STREQ(strings[i], str);
  }
}

TEST(String, EscapeSequence) {
  char* strings[] = {"\n", "\\n", "Hello World!\n"};
  char  source[]  = "\"\n\" \"\\n\" \"Hello World!\n\"";

  TokenList list = tokenizer(source);
  EXPECT_TRUE(list.len == sizeof(strings) / sizeof(char*));

  for (int i = 0; i < list.len; ++i) {
    Token token = list.buffer[i];
    char  str[token.length + 1];
    token_get_str(str, &token);
    EXPECT_TRUE(list.buffer[0].type == TOKEN_STRING);
    EXPECT_STREQ(strings[i], str);
  }
}

TEST(Character, Single) {
  TokenType tokens[] = {TOKEN_DOT, TOKEN_BANG, TOKEN_COMMA, TOKEN_SEMICOLON,
                        TOKEN_EQUAL};
  char      source[] = ".!,;=";

  TokenList list = tokenizer(source);
  EXPECT_TRUE(list.len == sizeof(tokens) / sizeof(TokenType));

  for (int i = 0; i < list.len; ++i) {
    EXPECT_TRUE(list.buffer[i].type == tokens[i]);
  }
}

TEST(Character, Multi) {
  TokenType tokens[] = {TOKEN_EQUAL_EQUAL, TOKEN_BANG_EQUAL};
  char      source[] = "==!=";

  TokenList list = tokenizer(source);
  EXPECT_TRUE(list.len == sizeof(tokens) / sizeof(TokenType));

  for (int i = 0; i < list.len; ++i) {
    EXPECT_TRUE(list.buffer[i].type == tokens[i]);
  }
}