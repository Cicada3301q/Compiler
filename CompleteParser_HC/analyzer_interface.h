#ifndef _ANALYZER_INTERFACE_H
# define _ANALYZER_INTERFACE_H

# include <stdio.h>
# include <stdbool.h>
# include "parseTree.h"
# include "symbolTable.h"

extern FILE *analyzer_file;
extern bool traceAnalysis;

void buildSymbolTable(ParseTree *tree);
void typeCheck(ParseTree *tree);

#endif
