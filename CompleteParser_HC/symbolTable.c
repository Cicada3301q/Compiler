#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "symbolTable.h"

#define CAST_MALLOC(TYPE) (TYPE *) malloc(sizeof(TYPE))

#define SHIFT 4
#define HT_SIZE 211

struct scopeList {
    ST_Bucket *hashTable[HT_SIZE];
};

static struct scopeList *ScopeList = NULL;
static int max_scope = 0;

static int hash(char *key){
    int temp = 0;
    for(int i = 0; key[i] != '\0'; i++){
        temp = ((temp << SHIFT) + key[i]) % HT_SIZE;
    }
    return temp;
}

void st_insert(char *name, int lineno, int loc, int scope, bool isParam){
    if (scope >= max_scope){
        size_t new_size = (scope > 2*max_scope) ? scope : 2*max_scope;
        new_size *= sizeof(struct scopeList);
        ScopeList = (struct scopeList *) realloc(ScopeList, new_size);
        if (ScopeList == NULL){
            fprintf(stderr, "Failed to allocate memory for symbol table\n");
            exit(1);
        }
    }
    int ind = hash(name);
    ST_Bucket *bucket = ScopeList[scope].hashTable[ind];
    while ((bucket != NULL) && (strcmp(name, bucket->name) != 0)){
        bucket = bucket->next;
    }
    if (bucket == NULL){
        bucket = CAST_MALLOC(ST_Bucket);
        bucket->name = name;
        bucket->scope = scope;
        bucket->isParam = isParam;
        bucket->referencing_lines = CAST_MALLOC(LineList);
        bucket->referencing_lines->lineno = lineno;
        bucket->referencing_lines->next = NULL;
        bucket->memory_location = loc;
        bucket->next = ScopeList[scope].hashTable[ind];
        ScopeList[scope].hashTable[ind] = bucket;
    } else {
        LineList *ptr = bucket->referencing_lines;
        while (ptr->next != NULL){
            ptr = ptr->next;
        }
        ptr->next = CAST_MALLOC(LineList);
        ptr->next->lineno = lineno;
        ptr->next->next = NULL;
    }
}

ST_Bucket *fun_lookup(char *name, int scope){
    int ind = hash(name);
    ST_Bucket *bucket = ScopeList[scope].hashTable[ind];
    while ((bucket != NULL) && (strcmp(name, bucket->name) != 0)){
        bucket = bucket->next;
    }
    return bucket;
}

ST_Bucket *var_lookup(char *name, int scope){
    int ind = hash(name);
    ST_Bucket *bucket = ScopeList[scope].hashTable[ind];
    while ((bucket != NULL) && (strcmp(name, bucket->name) != 0)){
        bucket = bucket->next;
    }
    return bucket;
}

int st_lookup(char *name, int scope){
    int ind = hash(name);
    ST_Bucket *bucket = ScopeList[scope].hashTable[ind];
    while ((bucket != NULL) && (strcmp(name, bucket->name) != 0)){
        bucket = bucket->next;
    }
    if (bucket == NULL){
        return -1;
    } else {
        return bucket->memory_location;
    }
}

void printSymbolTable(FILE *output){
    for(int scope = 0; scope < max_scope; scope++){
        fprintf(output, "Scope  Variable Name  Location  Line Numbers\n");
        fprintf(output, "-----  -------------  --------  ------------\n");
        for (int i = 0; i < HT_SIZE; i++){
            ST_Bucket *bucket = ScopeList[scope].hashTable[i];
            while (bucket != NULL){
                LineList *lineList = bucket->referencing_lines;
                fprintf(output, "%-5d  ", bucket->scope);
                fprintf(output, "%-13s  ", bucket->name);
                fprintf(output, "%-8d  ", bucket->memory_location);
                while (lineList != NULL){
                    fprintf(output, "%4d ", lineList->lineno);
                    lineList = lineList->next;
                }
                fprintf(output, "\n");
                bucket = bucket->next;
            }
        }
    }
    fprintf(output, "\n");
}
