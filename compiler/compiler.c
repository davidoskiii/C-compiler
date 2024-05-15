#include "compiler.h"
#include "../assembly/assembly.h"

static char *ASTop[] = { "+", "-", "*", "/" };

int interpretAST(struct ASTnode *n) {
  int leftval, rightval;

  if (n->left) leftval = interpretAST(n->left);
  if (n->right) rightval = interpretAST(n->right);

  switch (n->op) {
    case AST_ADD:
      return leftval + rightval;
    case AST_SUBTRACT:
      return leftval - rightval;
    case AST_MULTIPLY:
      return leftval * rightval;
    case AST_DIVIDE:
      return leftval / rightval;
    case AST_INTLIT:
      return n->intvalue;
    default:
      fprintf(stderr, "Unknown AST operator %d\n", n->op);
      exit(1);
  }
}

static int genAST(ASTnode *n) {
  int leftreg, rightreg;

  if (n->left)
    leftreg = genAST(n->left);
  if (n->right)
    rightreg = genAST(n->right);

  switch (n->op) {
    case AST_ADD:
      return (cgadd(leftreg,rightreg));
    case AST_SUBTRACT:
      return (cgsub(leftreg,rightreg));
    case AST_MULTIPLY:
      return (cgmul(leftreg,rightreg));
    case AST_DIVIDE:
      return (cgdiv(leftreg,rightreg));
    case AST_INTLIT:
      return (cgload(n->intvalue));
    default:
      fprintf(stderr, "Unknown AST operator %d\n", n->op);
      exit(1);
  }
}

void generatecode(ASTnode *n) {
  int reg;

  cgpreamble();
  reg= genAST(n);
  cgprintint(reg);
  cgpostamble();
}
