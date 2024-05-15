#include "../lexer/lexer.h"
#include "../common.h"
#include "expr.h"

static ASTnode *primary(void) {
  ASTnode *n;

  switch (Token_.type) {
    case TOKEN_INTLIT:
      n = mkastleaf(AST_INTLIT, Token_.intvalue);
      scan(&Token_);
      return n;
    default:
      fprintf(stderr, "Syntax error on line %d, token %d\n", Line, Token_.type);
      exit(1);
  }
}


int arithop(int tokentype) {
  switch (tokentype) {
    case TOKEN_PLUS:
      return AST_ADD;
    case TOKEN_MINUS:
      return AST_SUBTRACT;
    case TOKEN_STAR:
      return AST_MULTIPLY;
    case TOKEN_SLASH:
      return AST_DIVIDE;
    default:
      fprintf(stderr, "Syntax error on line %d, token %d\n", Line, tokentype);
      exit(1);
  }
}

static int OpPrec[] = { 0, 10, 10, 20, 20, 0 };

static int op_precedence(int tokentype) {
  int prec = OpPrec[tokentype];
  if (prec == 0) {
    fprintf(stderr, "Syntax error on line %d, token %d\n", Line, tokentype);
    exit(1);
  }
  return prec;
}

ASTnode *binexpr(int ptp) {
  ASTnode *left, *right;
  int tokentype;

  left = primary();

  tokentype = Token_.type;
  if (tokentype == TOKEN_EOF) return left;

  while (op_precedence(tokentype) > ptp) {
    scan(&Token_);

    right = binexpr(OpPrec[tokentype]);

    left = mkastnode(arithop(tokentype), left, right, 0);

    tokentype = Token_.type;
    if (tokentype == TOKEN_EOF)
      return left;
  }

  return left;
}
