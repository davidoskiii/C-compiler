#ifndef c_compiler_h
#define c_compiler_h

#include "../node/node.h"
#include "../assembly/assembly.h"

int genAST(ASTnode *n);

void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);
void genglobsym(char *s);

#endif 
