#ifndef c_sym_h
#define c_sym_h

typedef struct {
  char *name;
} SymbleTable;

int findglob(char *s);
int addglob(char *name);

#endif 
