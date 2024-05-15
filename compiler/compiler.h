#ifndef c_compiler_h
#define c_compiler_h

#include "../parser/parser.h"

int interpretAST(ASTnode *n);
void generatecode(ASTnode *n);

#endif 
