#ifndef __ZOZ_OPCODE_H__
#define __ZOZ_OPCODE_H__

typedef enum OpCode {
  OP_RETURN,
  OP_NUMBER,
  OP_WHILE,
  OP_IF,
  OP_ELSE,
  OP_ADD,
  OP_MINUS,
  OP_NOP,
} OpCode;

#endif  // __ZOZ_OPCODE_H__