#ifndef __ZOZ_COMPILER_H__
#define __ZOZ_COMPILER_H__

#define COMPILE_STATUS_SUCCESS 0
#define COMPILE_STATUS_ERROR   1

/**
 * @brief Compiler is a state machine
 *
 */
typedef struct CompileResult {
  int status;
} CompileResult;

/**
 * @brief Compile source to tokens
 *
 * @param source
 * @return CompileResult*
 */
CompileResult compile(const char* source);

#endif  // __ZOZ_COMPILER_H__