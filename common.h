#ifndef c_common_h
#define c_common_h

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer/lexer.h"

#ifndef extern_
 #define extern_ extern
#endif

extern_ int Line;
extern_ int	Putback;
extern_ FILE *Infile;
extern_ Token Token_;

#endif 
