#include "ast.h"

#include <stddef.h>
#include <stdlib.h>

#define PRE_ALLOC_CHILDS_COUNT 3

static AstNode* create_ast_node(OpCode operation, AstNode* parent) {
  AstNode* node   = (AstNode*)malloc(sizeof(AstNode));
  node->operation = operation;
  node->parent    = parent;
  node->len       = 0;
  node->childs = (AstNode**)malloc(sizeof(AstNode*) * PRE_ALLOC_CHILDS_COUNT);
  node->alloc  = PRE_ALLOC_CHILDS_COUNT;
  return node;
}

static void destory_ast_node(AstNode* node) {
  // TODO
}

static void ast_node_add_child(AstNode* node, AstNode* child) {
  if (node->len == node->alloc) {
    // expansion
    int       new_size = node->alloc * 2;
    AstNode** new_buffer =
        (AstNode**)malloc(sizeof(AstNode*) * PRE_ALLOC_CHILDS_COUNT);
    for (int i = 0; i < node->alloc; ++i) {
      new_buffer[i] = node->childs[i];
    }
    free(node->childs);
    node->childs = new_buffer;
    node->alloc  = new_size;
  }
  node->childs[node->len++] = child;
}

typedef struct Parser {
  TokenList* tokens;

  int current;

  AstNode* node;

} Parser;

static void init_parser(Parser* parser, TokenList* tokens, Ast* root) {
  parser->tokens  = tokens;
  parser->node    = root;
  parser->current = 0;
}

static bool is_at_end(Parser* parser) {
  return parser->current >= parser->tokens->len;
}

static Token* peek(Parser* parser) {
  return parser->tokens->buffer + parser->current;
}

static Token* advance(Parser* parser) {
  parser->current++;
  return parser->tokens->buffer + parser->current - 1;
}

static void parse_expression(Parser* parser) {
  Token* token = advance(parser);
  switch (token->type) {
    case TOKEN_EOF: {
      break;
    }
    default:
      break;
  }
}

bool create_ast(Ast* ast, TokenList* tokens) {
  Parser parser;
  ast = create_ast_node(OP_NOP, NULL);
  init_parser(&parser, tokens, ast);

  while (!is_at_end(&parser)) {
    parse_expression(&parser);
  }

  return false;
}

void destory_ast(Ast* ast) { destory_ast_node(ast); }