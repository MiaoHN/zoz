#ifndef __ZOZ_AST_H__
#define __ZOZ_AST_H__

#include <stdbool.h>

#include "opcode.h"
#include "token.h"

typedef struct AstNode {
  OpCode operation;
  void*  data;

  struct AstNode* parent;

  struct AstNode** childs;

  int len;    // number of childs
  int alloc;  // pre-alloc childs count
} AstNode;

/**
 * @brief Abstract Syntax Tree
 *
 */
typedef struct AstNode Ast;

/**
 * @brief Create an ast from given tokens
 *
 * @param tokens
 * @return Ast*
 */
bool create_ast(Ast* ast, TokenList* tokens);

void destory_ast(Ast* ast);

#endif  // __ZOZ_AST_H__