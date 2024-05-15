#include "../lexer/lexer.h"
#include "../common.h"
#include "expr.h"

ASTnode *primary(void) {
  ASTnode *n;

  switch (Token_.type) {
    case TOKEN_INTLIT:
      n = mkastleaf(AST_INTLIT, Token_.intvalue);
      scan(&Token_);
      return (n);
    default:
      fprintf(stderr, "Syntax error on line %d\n", Line);
      exit(1);
  }
}


int arithop(int tok) {
  switch (tok) {
    case TOKEN_PLUS:
      return (AST_ADD);
    case TOKEN_MINUS:
      return (AST_SUBTRACT);
    case TOKEN_STAR:
      return (AST_MULTIPLY);
    case TOKEN_SLASH:
      return (AST_DIVIDE);
    case TOKEN_EOF:
    default:
      fprintf(stderr, "Unknown token in arithop() on line %d: %d\n", Line, tok);
      exit(1);
  }
}

ASTnode *binexpr(void) {
  ASTnode *n, *left, *right;
  int nodetype;

  left = primary();

  if (Token_.type == TOKEN_EOF) return (left);

  nodetype = arithop(Token_.type);

  scan(&Token_);

  right = binexpr();

  n = mkastnode(nodetype, left, right, 0);
  return n;
}
