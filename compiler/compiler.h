#ifndef c_compiler_h
#define c_compiler_h

#include "../parser/parser.h"
#include "../assembly/assembly.h"

int genAST(struct ASTnode *n, int reg);

void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);
void genglobsym(char *s);

#endif 
