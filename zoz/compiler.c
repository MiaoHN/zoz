#include "compiler.h"

#include <stddef.h>
#include <stdio.h>

#include "token.h"
#include "tools.h"

CompileResult compile(const char* source) {
  CompileResult result;

  TokenList tokens = tokenizer(source);

  Token* last_token = &tokens.buffer[tokens.len - 1];
  if (last_token->type != TOKEN_ERROR) dump_tokens(&tokens);

  result.status = COMPILE_STATUS_ERROR;
  return result;
}