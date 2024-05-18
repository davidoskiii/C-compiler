#include "../parser/parser.h"
#include "../common.h"
#include "../sym/sym.h"
#include <stdio.h>
#include "expr.h"

static ASTnode *primary(void) {
  ASTnode *n;
  int id;

  switch (parser.current.type) {
    case TOKEN_INTVAL: {
      printf("intvalue: %d\n", parser.current.intvalue);
      n = mkastleaf(AST_INTLIT, parser.current.intvalue);
      break;
    }
    case TOKEN_IDENTIFIER: {
      id = findglob(globals.text);
      if (id == -1) errorAtCurrent("Unknown variable");

      n = mkastleaf(AST_IDENT, id);
      break;
    }
    default: errorAtCurrent("Syntax error");
  }
  advance();

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
    case TOKEN_EQUAL_EQUAL:
      return AST_EQ;
    case TOKEN_BANG_EQUAL:
      return AST_NE;
    case TOKEN_LESS:
      return AST_LT;
    case TOKEN_GREATER:
      return AST_GT;
    case TOKEN_LESS_EQUAL:
      return AST_LE;
    case TOKEN_GREATER_EQUAL:
      return AST_GE;
    default:
      fprintf(stderr, "Syntax error on line %d, token %d\n", parser.current.line, tokentype);
      exit(1);
  }
}

static int OpPrec[] = { 0, 10, 10, 20, 20, 30, 30, 40, 40, 40, 40, 0 };

static int op_precedence(int tokentype) {
  int prec = OpPrec[tokentype];
  if (prec == 0) {
    fprintf(stderr, "Syntax error on line %d, token %d\n", parser.current.line, tokentype);
    exit(1);
  }
  return prec;
}

struct ASTnode *binexpr(int ptp) {
  struct ASTnode *left, *right;
  int tokentype;

  left = primary();

  tokentype = parser.current.type;
  if (tokentype == TOKEN_SEMICOLON) return (left);

  while (op_precedence(tokentype) > ptp) {
    advance();

    right = binexpr(OpPrec[tokentype]);

    left = mkastnode(arithop(tokentype), left, right, 0);

    tokentype = parser.current.type;
    if (tokentype == TOKEN_SEMICOLON) return (left);
  }

  return (left);
}
