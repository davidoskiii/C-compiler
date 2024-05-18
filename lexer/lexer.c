#include <string.h>

#include "../common.h"
#include "lexer.h"

typedef struct {
  const char* start;
  const char* current;
  int line;
} Lexer;

Lexer lexer;

void initLexer(const char* source) {
  lexer.start = source;
  lexer.current = source;
  lexer.line = 1;
}

static bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
          c == '_';
}

static bool isDigit(char c) {
  return c >= '0' && c <= '9';
}

static bool isAtEnd() {
  return *lexer.current == '\0';
}

static char advance() {
  lexer.current++;
  return lexer.current[-1];
}

static char peek() {
  return *lexer.current;
}

static char peekNext() {
  if (isAtEnd()) return '\0';
  return lexer.current[1];
}

static char peekPrevious() {
  return lexer.current[-1];
}

static bool match(char expected) {
  if (isAtEnd()) return false;
  if (*lexer.current != expected) return false;
  lexer.current++;
  return true;
}

static bool matchNext(char expected) {
  if (isAtEnd() || peekNext() != expected) return false;
  lexer.current += 2;
  return true;
}

static Token makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.start = lexer.start;
  token.length = (int)(lexer.current - lexer.start);
  token.line = lexer.line;
  token.intvalue = 0;
  return token;
}

static Token errorToken(const char* message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = lexer.line;
  return token;
}

static void skipWhitespace() {
  for (;;) {
    char c = peek();
    switch (c) {
      case ' ':
      case '\r':
      case '\t':
        advance();
        break;
      case '\n':
        lexer.line++;
        advance();
        break;
      case '/':
        if (peekNext() == '/') {
          // Single-line comment: skip until the end of the line.
          while (peek() != '\n' && !isAtEnd()) advance();
        } else if (peekNext() == '*') {
          // Multi-line comment: skip until the closing token '*/'.
          advance();
          advance();
          while (!(peek() == '*' && peekNext() == '/') && !isAtEnd()) {
            if (peek() == '\n') {
              lexer.line++;
            }
            advance();
          }

          advance();
          advance();
        } else {
          return;
        }
        break;
      default:
        return;
    }
  }
}


static TokenType checkKeyword(int start, int length,
    const char* rest, TokenType type) {
  if (lexer.current - lexer.start == start + length &&
      memcmp(lexer.start + start, rest, length) == 0) {
    return type;
  }

  return TOKEN_IDENTIFIER;
}

static TokenType identifierType() {
  switch (lexer.start[0]) {
    case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
    case 'i': return checkKeyword(1, 2, "nt", TOKEN_INT);
  }

  return TOKEN_IDENTIFIER;
}

static Token identifier() {
  while (isAlpha(peek()) || isDigit(peek())) advance();
  return makeToken(identifierType());
}

static Token number() {
  while (isDigit(peek())) advance();

  // Look for a fractional part.
  if (peek() == '.' && isDigit(peekNext())) {
    return errorToken("Short's need to be implemented");
  }

  return makeToken(TOKEN_INTVAL);
}

Token scanToken() {
  skipWhitespace();
  lexer.start = lexer.current;

  if (isAtEnd()) return makeToken(TOKEN_EOF);

  char c = advance();
  if (isAlpha(c)) return identifier();
  if (isDigit(c)) return number();

  switch (c) {
    case '(': return makeToken(TOKEN_LEFT_PAREN);
    case ')': return makeToken(TOKEN_RIGHT_PAREN);
    case '-': return makeToken(TOKEN_MINUS);
    case '+': return makeToken(TOKEN_PLUS);
    case '/': return makeToken(TOKEN_SLASH);
    case '*': return makeToken(TOKEN_STAR); 

    case '!':
      if (match('=')) return makeToken(TOKEN_BANG_EQUAL);
      else return errorToken("Need to implement !");
    case '=':
      return makeToken(
          match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<':
      return makeToken(
          match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
      return makeToken(
          match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
  }

  return errorToken("Unexpected character.");
}

