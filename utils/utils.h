#ifndef c_utils_h
#define c_utils_h

#include "../parser/parser.h"

void fatal(char *s);
void fatals(char *s1, char *s2);
void fatald(char *s, int d);
void fatalc(char *s, int c);

void match(int t, char *what);
int check(int t);
void semi(void);
void ident(void);

#endif 
