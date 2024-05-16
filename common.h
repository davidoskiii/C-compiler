#ifndef c_common_h
#define c_common_h

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer/lexer.h"
#include "sym/sym.h"

#define TEXTLEN 512
#define NSYMBOLS 1024

typedef struct {
  int line;
  int	putback;
  FILE *infile;
  FILE *outfile;
  Token token;
  char text[TEXTLEN + 1];
  SymbleTable gsym[NSYMBOLS];
} Globals;

extern Globals globals;

#endif 
