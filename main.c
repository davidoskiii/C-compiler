#define extern_
  #include "common.h"
#undef extern_

#include "lexer/lexer.h"
#include "node/node.h"
#include "stmt/stmt.h"
#include "compiler/compiler.h"

Globals globals;

static void init() {
  globals.line = 1;
  globals.line = '\n';
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

  if ((globals.infile = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Unable to open %s\n", argv[1]);
    exit(1);
  }

  if ((globals.outfile = fopen("out.s", "w")) == NULL) {
    fprintf(stderr, "Unable to create out.s\n");
    exit(1);
  }

  scan(&globals.token);
  genpreamble();
  statements();
  genpostamble();

  fclose(globals.outfile);

  if (system("cc -o a.out out.s") != 0) {
    fprintf(stderr, "Error compiling assembly file\n");
    exit(1);
  }

  exit(0);
}
