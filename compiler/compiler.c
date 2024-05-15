#include "compiler.h"

static char *ASTop[] = { "+", "-", "*", "/" };

int interpretAST(struct ASTnode *n) {
  int leftval, rightval;

  if (n->left) leftval = interpretAST(n->left);
  if (n->right) rightval = interpretAST(n->right);

  if (n->op == AST_INTLIT)
    printf("int %d\n", n->intvalue);
  else
    printf("%d %s %d\n", leftval, ASTop[n->op], rightval);

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
