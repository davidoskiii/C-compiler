#ifndef c_common_h
#define c_common_h

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer/lexer.h"
#include "sym/sym.h"

#ifndef extern_
 #define extern_ extern
#endif

#define TEXTLEN 512
#define NSYMBOLS 1024

extern_ int Line;
extern_ int	Putback;
extern_ FILE *Infile;
extern_ FILE *Outfile;
extern_ Token Token_;
extern_ char Text[TEXTLEN + 1];
extern_ SymbleTable Gsym[NSYMBOLS];

#endif 
