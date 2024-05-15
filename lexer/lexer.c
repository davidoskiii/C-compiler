#include <string.h>

#include "../common.h"
#include "../data.h"
#include "lexer.h"

static int chrpos(char *s, int c) {
  char *p;

  p = strchr(s, c);
  return (p ? p - s : -1);
}

static int next(void) {
  int c;

  if (Putback) {
    c = Putback;
    Putback = 0;
    return c;
  }

  c = fgetc(Infile);
  if ('\n' == c)
    Line++;
  return c;
}

static void putback(int c) {
  Putback = c;
}

static int skip(void) {
  int c;

  c = next();
  while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c) {
    c = next();
  }
  return (c);
}

static int scanint(int c) {
  int k, val = 0;

  while ((k = chrpos("0123456789", c)) >= 0) {
    val = val * 10 + k;
    c = next();
  }

  putback(c);
  return val;
}

int scan(Token *t) {
  int c;

  c = skip();

  switch (c) {
    case EOF: return 0;
    case '+':
      t->type = TOKEN_PLUS;
      break;
    case '-':
      t->type = TOKEN_MINUS;
      break;
    case '*':
      t->type = TOKEN_STAR;
      break;
    case '/':
      t->type = TOKEN_SLASH;
      break;
    default: {
      if (isdigit(c)) {
        t->intvalue = scanint(c);
        t->type = TOKEN_INTLIT;
        break;
      }
    }

    printf("Unrecognised character %c on line %d\n", c, Line);
    exit(1);
  }

  return 1;
}
