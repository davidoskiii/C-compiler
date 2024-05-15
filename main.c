#define extern_
  #include "data.h"
#undef extern_

#include "common.h"
#include "lexer/lexer.h"

static void init() {
  Line = 1;
  Putback = '\n';
}

static void usage(char *prog) {
  fprintf(stderr, "Usage: %s infile\n", prog);
  exit(1);
}

char *tokstr[] = { "+", "-", "*", "/", "intlit" };

static void scanfile() {
  Token T;

  while (scan(&T)) {
    printf("Token %s", tokstr[T.type]);
    if (T.type == TOKEN_INTLIT)
      printf(", value %d", T.intvalue);
    printf("\n");
  }
}


int main(int argc, char *argv[]) {
  if (argc != 2)
    usage(argv[0]);

  init();

  if ((Infile = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Unable to open %s\n", argv[1]);
    exit(1);
  }

  scanfile();
  return 0;
}
