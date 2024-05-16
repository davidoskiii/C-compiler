#include "../common.h"
#include "../sym/sym.h"
#include "../parser/parser.h"
#include "stmt.h"

void assignment_statement(int declaration) {
  ASTnode *left, *right, *tree;
  int id;

  if ((id = findglob(globals.text)) == -1) {
    errorAtCurrent("Undeclared variable");
  }
  right = mkastleaf(AST_LVIDENT, id);

  consume(TOKEN_EQUAL, "Expected '=' after identifier");

  left = binexpr(0);

  tree = mkastnode(AST_ASSIGN, left, right, 0);

  genAST(tree, -1);
  genfreeregs();

  consume(TOKEN_SEMICOLON, "Expected ';' after new value");
}

void var_declaration(void) {
  advance();
  consume(TOKEN_IDENTIFIER, "Expected identifier");
  addglob(globals.text);
  genglobsym(globals.text);

  if (check(TOKEN_EQUAL)) {
    assignment_statement(1);
  } else {
    consume(TOKEN_SEMICOLON, "Expected ';' after indentifier");
  }
}

void print_statement(void) {
  ASTnode *tree;
  int reg;

  advance();

  tree = binexpr(0);
  reg = genAST(tree, -1);
  genprintint(reg);
  genfreeregs();

  consume(TOKEN_SEMICOLON, "Expected ';' after printed value");
}

void statements(void) {
  while (true) {
    switch (globals.token.type) {
    case TOKEN_PRINT:
      print_statement();
      break;
    case TOKEN_INT:
      var_declaration();
      break;
    case TOKEN_IDENTIFIER:
      assignment_statement(0);
      break;
    case TOKEN_EOF:
      return;
    default:
      errorAtCurrent("Syntax error, token");
    }
  }
}
