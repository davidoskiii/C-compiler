#include "compiler.h"
#include "../sym/sym.h"

void genpreamble()        { cgpreamble(); }
void genpostamble()       { cgpostamble(); }
void genfreeregs()        { freeall_registers(); }
void genprintint(int reg) { cgprintint(reg); }

int genAST(ASTnode *n) {
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
      return (cgloadint(n->v.intvalue));
    case AST_IDENT:
      return (cgloadglob(globals.gsym[n->v.id].name));
//    case AST_LVIDENT:
//      return (cgstorglob(reg, globals.gsym[n->v.id].name));
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
    case AST_NEGATE:
      return (cgnegate(leftreg));
    default:
      fprintf(stderr, "Unknown AST operator %d\n", n->op);
      exit(1);
  }
}
