#ifndef c_lexer_h
#define c_lexer_h

typedef enum {
  TOKEN_EOF,
  TOKEN_PLUS, TOKEN_MINUS,
  TOKEN_STAR, TOKEN_SLASH,
  TOKEN_EQ, TOKEN_NE,
  TOKEN_LT, TOKEN_GT, TOKEN_LE, TOKEN_GE,
  TOKEN_INTLIT, TOKEN_SEMICOLON, TOKEN_ASSIGN, TOKEN_IDENTIFIER,
  // Keywords
  TOKEN_PRINT, TOKEN_INT
} TokenType;

typedef struct {
  TokenType type;
  int intvalue;
} Token;

int scan(Token *t);

#endif 
