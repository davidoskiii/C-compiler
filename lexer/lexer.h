#ifndef c_lexer_h
#define c_lexer_h

typedef enum {
  TOKEN_EOF,

  TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH, TOKEN_SEMICOLON,
  TOKEN_EQUALS,

  TOKEN_INTLIT, TOKEN_IDENTIFIER,

  TOKEN_PRINT, TOKEN_INT,
} TokenType;

typedef struct {
  TokenType type;
  int intvalue;
} Token;

int scan(Token *t);

#endif 
