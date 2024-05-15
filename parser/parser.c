#include <stdlib.h>
#include "parser.h"

ASTnode *mkastnode(int op, ASTnode *left, ASTnode *right, int intvalue) {
  ASTnode *n;

  n = (ASTnode *) malloc(sizeof(ASTnode));
  if (n == NULL) {
    fprintf(stderr, "Unable to malloc in mkastnode()\n");
    exit(1);
  }

  n->op = op;
  n->left = left;
  n->right = right;
  n->intvalue = intvalue;
  return n;
}

ASTnode *mkastleaf(int op, int intvalue) {
  return mkastnode(op, NULL, NULL, intvalue);
}

ASTnode *mkastunary(int op, ASTnode *left, int intvalue) {
  return mkastnode(op, left, NULL, intvalue);
}
