#ifndef c_expr_h
#define c_expr_h

#include "../parser/parser.h"

ASTnode *binexpr(int n);
void fatal(char *s);
void fatals(char *s1, char *s2);
void fatald(char *s, int d);
void fatalc(char *s, int c);

#endif 
