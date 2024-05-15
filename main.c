#define extern_
  #include "common.h"
#undef extern_

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "stmt/stmt.h"
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

  if (argc != 2)
    usage(argv[0]);

  init();

  if ((Infile = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Unable to open %s\n", argv[1]);
    exit(1);
  }

  if ((Outfile = fopen("out.s", "w")) == NULL) {
    fprintf(stderr, "Unable to create out.s\n");
    exit(1);
  }

  scan(&Token_);
  genpreamble();
  statements();
  genpostamble();

  fclose(Outfile);

  if (system("cc -o a.out out.s") != 0) {
    fprintf(stderr, "Error compiling assembly file\n");
    exit(1);
  }

  exit(0);
}
