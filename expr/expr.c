#include "../lexer/lexer.h"
#include "../common.h"
#include "../sym/sym.h"
#include "expr.h"

static ASTnode *primary(void) {
  ASTnode *n;
  int id;

  switch (Token_.type) {
    case TOKEN_INTLIT: {
      n = mkastleaf(AST_INTLIT, Token_.intvalue);
      break;
    }
    case TOKEN_IDENTIFIER: {
      id = findglob(Text);
      if (id == -1) fatals("Unknown variable", Text);

      n = mkastleaf(AST_IDENT, id);
      break;
    }
    default: fatald("Syntax error, token", Token_.type);
  }

  scan(&Token_);
  return (n);
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
    case TOKEN_EQ:
      return AST_EQ;
    case TOKEN_NE:
      return AST_NE;
    case TOKEN_LT:
      return AST_LT;
    case TOKEN_GT:
      return AST_GT;
    case TOKEN_LE:
      return AST_LE;
    case TOKEN_GE:
      return AST_GE;
    default:
      fprintf(stderr, "Syntax error on line %d, token %d\n", Line, tokentype);
      exit(1);
  }
}

static int OpPrec[] = { 0, 10, 10, 20, 20, 30, 30, 40, 40, 40, 40, 0 };

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
  if (tokentype == TOKEN_SEMICOLON) return left;

  while (op_precedence(tokentype) > ptp) {
    scan(&Token_);

    right = binexpr(OpPrec[tokentype]);

    left = mkastnode(arithop(tokentype), left, right, 0);

    tokentype = Token_.type;
    if (tokentype == TOKEN_SEMICOLON) return left;
  }

  return left;
}
