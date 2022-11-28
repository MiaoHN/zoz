#ifndef __ZOZ_COMPILER_H__
#define __ZOZ_COMPILER_H__

#include "ast.h"

#define COMPILE_STATUS_SUCCESS 1
#define COMPILE_STATUS_ERROR   0

/**
 * @brief Compiler is a state machine
 *
 */
typedef struct CompileResult {
  Ast* ast;
  int  status;
} CompileResult;

/**
 * @brief Compile source to tokens
 *
 * @param source
 * @return CompileResult*
 */
CompileResult compile(const char* source);

#endif  // __ZOZ_COMPILER_H__