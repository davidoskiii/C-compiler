#include <stdio.h>
#define extern_
  #include "common.h"
#undef extern_

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "node/node.h"
#include "compiler/compiler.h"

Globals globals;

static void init() {
  globals.line = 1;
  globals.putback = '\n';
}

static void usage(char *prog) {
  fprintf(stderr, "Usage: %s [path]\n", prog);
  exit(1);
}

char* readFile(const char* path) {
  FILE* file = fopen(path, "rb");
  if (file == NULL) {
    fprintf(stderr, "Could not open file \"%s\".\n", path);
    fprintf(stderr, "Usage: luminique [path]\n");
    exit(74);
  }

  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  char* buffer = (char*)malloc(fileSize + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
    exit(74);
  }

  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
  if (bytesRead < fileSize) {
    fprintf(stderr, "Could not read file \"%s\".\n", path);
    exit(74);
  }

  buffer[bytesRead] = '\0';

  fclose(file);
  return buffer;
}

int interpretAST(struct ASTnode *n) {
  int leftval, rightval;

  if (n->left) leftval = interpretAST(n->left);
  if (n->right) rightval = interpretAST(n->right);

  switch (n->op) {
    case AST_ADD:
      return leftval + rightval;
    case AST_SUBTRACT:
      return leftval - rightval;
    case AST_MULTIPLY:
      return leftval * rightval;
    case AST_DIVIDE:
      return leftval / rightval;
    case AST_INTLIT:
      return n->v.intvalue;
    default:
      fprintf(stderr, "Unknown AST operator %d\n", n->op);
      exit(1);
  }
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

  initLexer(readFile(argv[1]));
  parser.hadError = false;
  parser.panicMode = false;
  
  advance();
  advance();

  n = parse();
  int reg;

  cgpreamble();
  reg = genAST(n);
  cgprintint(reg);
  cgpostamble();

  fclose(globals.outfile);

  if (system("cc -o a.out out.s") != 0) {
    fprintf(stderr, "Error compiling assembly file\n");
    exit(1);
  }

  exit(0);
}
