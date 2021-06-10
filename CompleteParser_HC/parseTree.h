#ifndef _PARSETREE_H
# define _PARSETREE_H

# include <stdbool.h>

typedef enum {StmtK, ExprK, DeclK} NodeKind;
typedef enum {ExprS, IfS, WhileS, ReturnS, CompoundS} StmtKind;
typedef enum {LogE, RelE, AddE, MulE, ConstE, IdE, AssignE, CallE} ExprKind;
typedef enum {VarD, ArrD, FunD} DeclKind;
typedef enum {OP_AND, OP_OR, OP_NOT} LogOp;
typedef enum {OP_LE, OP_LT, OP_GT, OP_GE, OP_EQ, OP_NE} RelOp;
typedef enum {OP_ADD = '+', OP_SUB = '-'} AddOp;
typedef enum {OP_MUL = '*', OP_DIV = '/'} MulOp;

typedef enum {Void, Integer, Boolean} ExprType;

#define MAXCHILDREN 3

typedef struct treeNode {
    struct treeNode *child[MAXCHILDREN];
    struct treeNode *sibling;
    NodeKind nodeKind;
    union {StmtKind stmt; ExprKind expr; DeclKind decl;} kind;
    union {LogOp logOp; AddOp addOp; MulOp mulOp; RelOp relOp; int val; char *name;} attr;
    int array_value;
    bool isParam;
    ExprType type;
    int lineno;
} ParseTree;

ParseTree *newTree(void);
void freeTree(ParseTree *tree);

#endif
