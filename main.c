#define extern_
  #include "common.h"
#undef extern_

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "compiler/compiler.h"

static void init() {
  Line = 1;
  Putback = '\n';
}

static void usage(char *prog) {
  fprintf(stderr, "Usage: %s [path]\n", prog);
  exit(1);
}

int main(int argc, char *argv[]) {
  ASTnode *n;

  if (argc != 2) usage(argv[0]);

  init();

  if ((Infile = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Unable to open %s\n", argv[1]);
    exit(1);
  }

  scan(&Token_);
  n = binexpr();
  printf("%d\n", interpretAST(n));
  exit(0);
}
