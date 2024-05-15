#include "compiler.h"
#include "../sym/sym.h"

static char *ASTop[] = { "+", "-", "*", "/" };


void genpreamble()        { cgpreamble(); }
void genpostamble()       { cgpostamble(); }
void genfreeregs()        { freeall_registers(); }
void genprintint(int reg) { cgprintint(reg); }

int genAST(struct ASTnode *n, int reg) {
  int leftreg, rightreg;

  if (n->left)
    leftreg = genAST(n->left, -1);
  if (n->right)
    rightreg = genAST(n->right, leftreg);

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
      return (cgloadint(n->v.intvalue));
    case AST_IDENT:
      return (cgloadglob(Gsym[n->v.id].name));
    case AST_LVIDENT:
      return (cgstorglob(reg, Gsym[n->v.id].name));
    case AST_ASSIGN:
      return (rightreg);
    case AST_EQ:
      return (cgequal(leftreg, rightreg));
    case AST_NE:
      return (cgnotequal(leftreg, rightreg));
    case AST_LT:
      return (cglessthan(leftreg, rightreg));
    case AST_GT:
      return (cggreaterthan(leftreg, rightreg));
    case AST_LE:
      return (cglessequal(leftreg, rightreg));
    case AST_GE:
      return (cggreaterequal(leftreg, rightreg));
    default:
      fprintf(stderr, "Unknown AST operator %d\n", n->op);
      exit(1);
  }
}
