#include <string.h>

#include "../common.h"
#include "../utils/utils.h"
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

static int scanident(int c, char *buf, int lim) {
  int i = 0;

  while (isalpha(c) || isdigit(c) || '_' == c) {
    if (lim - 1 == i) {
      printf("Identifier too long on line %d\n", Line);
      exit(1);
    } else if (i < lim - 1) {
      buf[i++] = c;
    }
    c = next();
  }

  putback(c);
  buf[i] = '\0';
  return (i);
}

static int keyword(char *s) {
  switch (*s) {
    case 'p':
      if (!strcmp(s, "print"))
        return (TOKEN_PRINT);
      break;
    case 'i':
      if (!strcmp(s, "int"))
        return (TOKEN_INT);
      break;
  }
  return (0);
}

int scan(Token *t) {
  int c, tokentype;

  c = skip();

  switch (c) {
    case EOF:
      t->type = TOKEN_EOF;
      return 0;
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
    case ';':
      t->type = TOKEN_SEMICOLON;
      break;
    case '=':
      if ((c = next()) == '=') {
        t->type = TOKEN_EQ;
      } else {
        putback(c);
        t->type = TOKEN_ASSIGN;
      }
      break;
    case '!':
      if ((c = next()) == '=') {
        t->type = TOKEN_NE;
      } else {
        fatalc("Unrecognised character", c);
      }
      break;
    case '<':
      if ((c = next()) == '=') {
        t->type = TOKEN_LE;
      } else {
        putback(c);
        t->type = TOKEN_LT;
      }
      break;
    case '>':
      if ((c = next()) == '=') {
        t->type = TOKEN_GE;
      } else {
        putback(c);
        t->type = TOKEN_GT;
      }
      break;
    default: {
      if (isdigit(c)) {
        t->intvalue = scanint(c);
        t->type = TOKEN_INTLIT;
        break;
      } else if (isalpha(c) || '_' == c) {
        scanident(c, Text, TEXTLEN);

        if ((tokentype = keyword(Text))) {
          t->type = tokentype;
          break;
        }

        t->type = TOKEN_IDENTIFIER;
        break;
      }
    }

    printf("Unrecognised character %c on line %d\n", c, Line);
    exit(1);
  }

  return 1;
}
