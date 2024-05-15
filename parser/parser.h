#ifndef c_parser_h
#define c_parser_h

#include "../common.h"

typedef struct ASTnode ASTnode;

enum {
  AST_ADD, AST_SUBTRACT, AST_MULTIPLY, AST_DIVIDE, AST_INTLIT
};

struct ASTnode {
  int op;
  ASTnode *left;
  ASTnode *right;
  int intvalue;
};

ASTnode *mkastnode(int op, ASTnode *left, ASTnode *right, int intvalue);
ASTnode *mkastleaf(int op, int intvalue);
ASTnode *mkastunary(int op, ASTnode *left, int intvalue);
ASTnode *binexpr(void);

#endif 
