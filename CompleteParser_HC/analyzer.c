#include "analyzer_interface.h"

FILE *analyzer_file;
bool traceAnalysis;

static void traverse (ParseTree *tree,
    void (*preProc)(ParseTree *),
    void (*postProc)(ParseTree *)
){
    if (tree != NULL){
        preProc(tree);
        for(int i = 0; i < MAXCHILDREN; i++){
            traverse(tree->child[i], preProc, postProc);
        }
        postProc(tree);
        traverse(tree->sibling, preProc, postProc);
    }
}

static void nullProc(ParseTree *tree){
    return;
}

static void insertNode(ParseTree *tree){
    /* You need to complete this function! */

}

void buildSymbolTable(ParseTree *tree){
    traverse(tree, insertNode, nullProc);
    if (traceAnalysis){
        fprintf(analyzer_file, "\nSymbol table:\n\n");
        printSymbolTable(analyzer_file);
    }
}

static void typeError(ParseTree *tree, char *message){
    fprintf(analyzer_file, "Type error att line %d: %s\n", tree->lineno, message);
}

static void checkNode(ParseTree *tree){
    /* You need to complete this function! */
}

void typeCheck(ParseTree *tree){
    traverse(tree, nullProc, checkNode);
}
