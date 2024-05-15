#ifndef c_lexer_h
#define c_lexer_h

#include "../data.h"

typedef enum {
  TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH,
  TOKEN_INTLIT
} TokenType;

typedef struct {
  TokenType type;
  int intvalue;
} Token;

int scan(Token *t);

#endif 
