#ifndef _PARSER_INTERFACE_H
# define _PARSER_INTERFACE_H
# include <stdio.h>
# include <stdbool.h>
# include "parseTree.h"

extern FILE *parse_file;
extern bool traceParse;

ParseTree *parse();

#endif
