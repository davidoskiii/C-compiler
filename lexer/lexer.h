#ifndef c_lexer_h
#define c_lexer_h


typedef enum {
  TOKEN_EOF,
  TOKEN_MINUS, TOKEN_PLUS,
  TOKEN_SLASH, TOKEN_STAR,

  // One or two character tokens.
  TOKEN_BANG_EQUAL, TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER, TOKEN_LESS,
  TOKEN_LESS_EQUAL, TOKEN_GREATER_EQUAL,

  // Literals.
  TOKEN_INTVAL, TOKEN_SEMICOLON, TOKEN_EQUAL, TOKEN_IDENTIFIER, 

  // Keywords
  TOKEN_INT, TOKEN_PRINT,

  // Others
  TOKEN_ERROR
} TokenType;

typedef struct {
  TokenType type;
  const char* start;
  int length;
  int line;
  int intvalue;
} Token;

void initLexer(const char* source);
Token scanToken();

#endif 
