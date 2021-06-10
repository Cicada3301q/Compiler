#ifndef _SYMBOL_TABLE_H
# define _SYMBOL_TABLE_H

# include <stdio.h>
# include <stdbool.h>

typedef struct lineListItem {
    struct lineListItem *next;
    int lineno;
} LineList;

typedef struct symbolTableBucketItem {
    struct symbolTableBucketItem *next;
    char *name;
    int memory_location;
    int scope;
    bool isParam;
    int fun_start;
    LineList *referencing_lines;
} ST_Bucket;

void st_insert(char *name, int lineno, int loc, int scope, bool isParam);
int st_lookup(char *name, int scope);
ST_Bucket *fun_lookup(char *name, int scope);
ST_Bucket *var_lookup(char *name, int scope);

void printSymbolTable(FILE *output);

#endif
