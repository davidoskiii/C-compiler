#ifndef c_parser_h
#define c_parser_h

#include <stdbool.h>
#include "../lexer/lexer.h"
#include "../node/node.h"

typedef struct {
  Token current;
  Token previous;
  Token next;
  bool hadError;
  bool panicMode;
} Parser;

extern Parser parser;

void error(const char* message);
void errorAtCurrent(const char* message);
void errorAtPrevious(const char* message);
void advance();
void consume(TokenType type, const char* message);
bool check(TokenType type);
bool checkNext(TokenType type);
bool checkPrevious(TokenType type);
bool match(TokenType type);

int getPrecedence(TokenType type);
ASTnode* parseBinaryOpRHS(int exprPrec, ASTnode* lhs);
ASTnode* parseExpression();
ASTnode* parseUnary();
ASTnode* parsePrimary();
ASTnode* parse();

#endif
