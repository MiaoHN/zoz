#include "test.h"
#include "token.h"

TEST(Token, Number) {
  char* numbers[] = {"1",  ".1", "0",    "1.1",   "1.",
                     "+1", "-1", "-.32", "1.e+1", "-.1e-3"};

  TokenList list = tokenizer("1  .1  0  1.1  1.  +1  -1 -.32 1.e+1 -.1e-3");
  EXPECT_TRUE(list.len == sizeof(numbers) / sizeof(char*));

  for (int i = 0; i < list.len; ++i) {
    Token token = list.buffer[i];
    char  str[token.length + 1];
    token_get_str(str, &token);
    EXPECT_TRUE(list.buffer[0].type == TOKEN_NUMBER);
    EXPECT_STREQ(numbers[i], str);
  }
}