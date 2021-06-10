#ifndef _PARSE_TREE_H
# define _PARSE_TREE_H
# include <stdlib.h>
# include "parseTree.h"

ParseTree *newTree(void){
    ParseTree *tree = (ParseTree *) malloc(sizeof(ParseTree));
    for (int i = 0; i < MAXCHILDREN; i++){
        tree->child[i] = NULL;
    }
    tree->sibling = NULL;
    tree->nodeKind = StmtK;
    tree->kind.stmt = ExprS;
    tree->attr.val = 0;
    tree->array_value = 0;
    tree->isParam = false;
    tree->type = Void;
    return tree;
}

void freeTree(ParseTree *tree){
    if (tree == NULL) return;
    switch(tree->nodeKind){
        case DeclK:
        free(tree->attr.name);
        break;
        case ExprK:
        switch(tree->kind.expr){
            case CallE:
            case IdE:
            free(tree->attr.name);
            default:
            break;
        }
        break;
        default:
        break;
    }
    for(int i = 0; i < MAXCHILDREN; i++){
        freeTree(tree->child[i]);
    }
    freeTree(tree->sibling);
    free(tree);
}

#endif
