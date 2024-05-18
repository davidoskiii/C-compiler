#include <stdio.h>
#include <stdlib.h> // For exit()

#include "parser.h"

Parser parser;

typedef enum {
  PREC_NONE,
  PREC_ASSIGNMENT,
  PREC_TERM,
  PREC_FACTOR,
  PREC_EQUALITY,
  PREC_COMPARISON,
  PREC_UNARY
} Precedence;

void errorAt(Token* token, const char* message) {
  if (parser.panicMode) return;
  parser.panicMode = true;
  fprintf(stderr, "[line %d] Error", token->line);

  if (token->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (token->type == TOKEN_ERROR) {
    // Nothing.
  } else {
    fprintf(stderr, " at '%.*s'", token->length, token->start);
  }

  fprintf(stderr, ": %s\n", message);
  parser.hadError = true;
}

void error(const char* message) {
  errorAt(&parser.previous, message);
}

void errorAtCurrent(const char* message) {
  errorAt(&parser.current, message);
}

void errorAtPrevious(const char* message) {
  errorAt(&parser.previous, message);
}

void advance() {
  parser.previous = parser.current;
  parser.current = parser.next;

  for (;;) {
    parser.next = scanToken();
    if (parser.next.type != TOKEN_ERROR) break;

    errorAtCurrent(parser.next.start);
  }
}

void consume(TokenType type, const char* message) {
  if (parser.current.type == type) {
    advance();
  } else {
    errorAtPrevious(message);
  }
}

bool check(TokenType type) {
  return parser.current.type == type;
}

bool match(TokenType type) {
  if (!check(type)) return false;
  advance();
  return true;
}

int getPrecedence(TokenType type) {
  switch (type) {
    case TOKEN_EQUAL_EQUAL:
    case TOKEN_BANG_EQUAL:
      return PREC_EQUALITY;
    case TOKEN_LESS:
    case TOKEN_LESS_EQUAL:
    case TOKEN_GREATER:
    case TOKEN_GREATER_EQUAL:
      return PREC_COMPARISON;
    case TOKEN_PLUS:
    case TOKEN_MINUS:
      return PREC_TERM;
    case TOKEN_STAR:
    case TOKEN_SLASH:
      return PREC_FACTOR;
    default:
      return PREC_NONE;
  }
}

ASTnode* parseBinaryOpRHS(int exprPrec, ASTnode* lhs) {
  while (true) {
    int tokPrec = getPrecedence(parser.current.type);

    if (tokPrec < exprPrec || parser.current.type == TOKEN_EOF) {
      return lhs;
    }

    TokenType op = parser.current.type;
    advance();

    ASTnode* rhs = parseUnary();

    int nextPrec = getPrecedence(parser.current.type);
    if (tokPrec < nextPrec) {
      rhs = parseBinaryOpRHS(tokPrec + 1, rhs);
    }

    int astType;
    switch (op) {
      case TOKEN_PLUS:
        astType = AST_ADD;
        break;
      case TOKEN_MINUS:
        astType = AST_SUBTRACT;
        break;
      case TOKEN_STAR:
        astType = AST_MULTIPLY;
        break;
      case TOKEN_SLASH:
        astType = AST_DIVIDE;
        break;
      case TOKEN_EQUAL_EQUAL:
        astType = AST_EQ;
        break;
      case TOKEN_BANG_EQUAL:
        astType = AST_NE;
        break;
      case TOKEN_LESS:
        astType = AST_LT;
        break;
      case TOKEN_LESS_EQUAL:
        astType = AST_LE;
        break;
      case TOKEN_GREATER:
        astType = AST_GT;
        break;
      case TOKEN_GREATER_EQUAL:
        astType = AST_GE;
        break;
      default:
        astType = -1;
        break;
    }

    lhs = mkastnode(astType, lhs, rhs, 0);
  }
}

ASTnode* parseExpression() {
  ASTnode* left = parseUnary();
  return parseBinaryOpRHS(0, left);
}

ASTnode* integer() {
  int value = strtol(parser.previous.start, NULL, 10);
  return mkastleaf(AST_INTLIT, value);
}

ASTnode* parsePrimary() {
  if (match(TOKEN_INTVAL)) {
    return integer();
  } else {
    error("Expected expression.");
    exit(0);
  }
}

ASTnode* parseUnary() {
  if (match(TOKEN_MINUS)) {
    ASTnode* zeroNode = mkastleaf(AST_INTLIT, 0);
    ASTnode* right = parseUnary();
    return mkastnode(AST_SUBTRACT, zeroNode, right, 0);
  } else if (match(TOKEN_PLUS)) {
    advance();
    return parseUnary();
  }
  return parsePrimary();
}

ASTnode* parse() {
  ASTnode* expr = parseExpression();
  if (parser.current.type != TOKEN_EOF) {
    error("Expected end of file.");
    exit(EXIT_FAILURE);
  }
  return expr;
}
