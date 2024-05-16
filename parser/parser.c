#include <stdio.h>

#include "parser.h"

Parser parser;

static void errorAt(Token* token, const char* message) {
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
    return;
  }

  errorAtPrevious(message);
}

bool check(TokenType type) {
  return parser.current.type == type;
}

bool checkNext(TokenType type) {
  return parser.next.type == type;
}

bool checkPrevious(TokenType type) {
  return parser.previous.type == type;
}

bool match(TokenType type) {
  if (!check(type)) return false;
  advance();
  return true;
}
