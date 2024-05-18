#ifndef c_node_h
#define c_node_h

#include "../common.h"

typedef struct ASTnode ASTnode;

enum {
  AST_ADD, AST_SUBTRACT, AST_MULTIPLY, AST_DIVIDE, AST_INTLIT,
  AST_IDENT, AST_LVIDENT, AST_ASSIGN, AST_EQ, AST_NE, AST_LT, AST_GT,
  AST_LE, AST_GE, AST_NEGATE,
};

struct ASTnode {
  int op;
  ASTnode *left;
  ASTnode *right;
  union {
    int intvalue;
    int id;
  } v;
};

ASTnode *mkastnode(int op, ASTnode *left, ASTnode *right, int intvalue);
ASTnode *mkastleaf(int op, int intvalue);
ASTnode *mkastunary(int op, ASTnode *left, int intvalue);

#endif 
