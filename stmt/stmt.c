#include "../common.h"
#include "../utils/utils.h"
#include "../sym/sym.h"
#include "stmt.h"

void var_declaration(void) {
  match(TOKEN_INT, "int");
  ident();
  addglob(Text);
  genglobsym(Text);
  semi();
}

void print_statement(void) {
  ASTnode *tree;
  int reg;

  match(TOKEN_PRINT, "print");

  tree = binexpr(0);
  reg = genAST(tree, -1);
  genprintint(reg);
  genfreeregs();

  semi();
}

void assignment_statement(void) {
  ASTnode *left, *right, *tree;
  int id;

  ident();

  if ((id = findglob(Text)) == -1) {
    fatals("Undeclared variable", Text);
  }
  right = mkastleaf(AST_LVIDENT, id);

  match(TOKEN_EQUALS, "=");

  left = binexpr(0);

  tree = mkastnode(AST_ASSIGN, left, right, 0);

  genAST(tree, -1);
  genfreeregs();

  semi();
}

void statements(void) {
  while (1) {
    switch (Token_.type) {
    case TOKEN_PRINT:
      print_statement();
      break;
    case TOKEN_INT:
      var_declaration();
      break;
    case TOKEN_IDENTIFIER:
      assignment_statement();
      break;
    case TOKEN_EOF:
      return;
    default:
      fatald("Syntax error, token", Token_.type);
    }
  }
}
