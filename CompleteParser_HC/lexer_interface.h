#ifndef _LEXER_INTERFACE_H
# define _LEXER_INTERFACE_H
# include <stdio.h>
# include <stdbool.h>

#include "CM.tab.h"

extern FILE *scan_file;
extern bool traceScan;
extern int lineno;

void setSourceFile(FILE *source);
TokenType getToken(void);
void printCurrentToken(FILE *stream);

#endif
