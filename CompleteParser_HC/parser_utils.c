#include <stdio.h>
#include "parser_utils.h"

static int tabs = -1;
#define INDENT tabs++;
#define UNINDENT --tabs;
#define TABS for(int i = 0; i < tabs; i++) fprintf(stream, "\t");
#define PRINT_STATEMENT(X) fprintf(stream, "[%s statement]\n", #X);
#define PRINT_OPERATOR(X) fprintf(stream, "[Operator %s]\n", #X);

static ParseTree *newStatementTree(StmtKind kind){
    ParseTree *tree = newTree();
    tree->nodeKind = StmtK;
    tree->kind.stmt = kind;
    return tree;
}

static ParseTree *newExpressionTree(ExprKind kind){
    ParseTree *tree = newTree();
    tree->nodeKind = ExprK;
    tree->kind.expr = kind;
    return tree;
}

static ParseTree *newDeclarationTree(DeclKind kind){
    ParseTree *tree = newTree();
    tree->nodeKind = DeclK;
    tree->kind.decl = kind;
    return tree;
}

ParseTree *appendSibling(ParseTree *left, ParseTree *right){
    if (left == NULL) {
        return right;
    } else {
        ParseTree *ptr = left;
        while (ptr->sibling != NULL) {
            ptr = ptr->sibling;
        }
        ptr->sibling = right;
        return left;
    }
}

ParseTree *newVarDeclaration(ExprType type, char *name){
    ParseTree *tree = newDeclarationTree(VarD);
    tree->attr.name = name;
    tree->type = type;
    return tree;
}

ParseTree *newArrayDeclaration(ExprType type, char *name, int len){
    ParseTree *tree = newDeclarationTree(ArrD);
    tree->attr.name = name;
    tree->array_value = len;
    tree->type = type;
    return tree;
}

ParseTree *newFunDeclaration(ExprType type, char *name, ParseTree *params, ParseTree *body){
    ParseTree *tree = newDeclarationTree(FunD);
    tree->child[0] = params;
    tree->child[1] = body;
    tree->attr.name = name;
    tree->type = type;
    return tree;
}

ParseTree *newParameter(ExprType type, char *name, bool isArray){
    ParseTree *tree;
    if (isArray)
        tree = newArrayDeclaration(type, name, 0);
    else
        tree = newVarDeclaration(type, name);
    tree->isParam = true;
    return tree;
}

ParseTree *newCompoundStatement(ParseTree *declarations, ParseTree *statements){
    ParseTree *tree = newStatementTree(CompoundS);
    tree->child[0] = declarations;
    tree->child[1] = statements;
    return tree;
}

ParseTree *newIfStatement(ParseTree *condition, ParseTree *ifbody, ParseTree *elsebody){
    ParseTree *tree = newStatementTree(IfS);
    tree->child[0] = condition;
    tree->child[1] = ifbody;
    tree->child[2] = elsebody;
    return tree;
}

ParseTree *newWhileStatement(ParseTree *condition, ParseTree *body){
    ParseTree *tree = newStatementTree(WhileS);
    tree->child[0] = condition;
    tree->child[1] = body;
    return tree;
}

ParseTree *newReturnStatement(ParseTree *returnValue){
    ParseTree *tree = newStatementTree(ReturnS);
    tree->child[0] = returnValue;
    return tree;
}

ParseTree *newAssignExpression(ParseTree *var, ParseTree *value){
    ParseTree *tree = newExpressionTree(AssignE);
    tree->child[0] = var;
    tree->child[1] = value;
    return tree;
}

ParseTree *newVarExpression(char *name, ParseTree *index){
    ParseTree *tree = newExpressionTree(IdE);
    tree->child[0] = index;
    tree->attr.name = name;
    return tree;
}

ParseTree *newLogicalExpression(ParseTree *left, LogOp op, ParseTree *right){
    ParseTree *tree = newExpressionTree(LogE);
    tree->child[0] = left;
    tree->child[1] = right;
    tree->attr.logOp = op;
    return tree;
}

ParseTree *newBooleanConstant(bool val){
    ParseTree *tree = newExpressionTree(ConstE);
    tree->attr.val = (int) val;
    tree->type = Boolean;
    return tree;
}

ParseTree *newRelativeExpression(ParseTree *left, RelOp op, ParseTree *right){
    ParseTree *tree = newExpressionTree(RelE);
    tree->child[0] = left;
    tree->child[1] = right;
    tree->attr.relOp = op;
    return tree;
}

ParseTree *newAdditiveExpression(ParseTree *left, AddOp op, ParseTree *right){
    ParseTree *tree = newExpressionTree(AddE);
    tree->child[0] = left;
    tree->child[1] = right;
    tree->attr.addOp = op;
    return tree;
}

ParseTree *newMultiplicativeExpression(ParseTree *left, MulOp op, ParseTree *right){
    ParseTree *tree = newExpressionTree(MulE);
    tree->child[0] = left;
    tree->child[1] = right;
    tree->attr.mulOp = op;
    return tree;
}

ParseTree *newIntegerConstant(int val){
    ParseTree *tree = newExpressionTree(ConstE);
    tree->attr.val = val;
    tree->type = Integer;
    return tree;
}

ParseTree *newCallExpression(char *name, ParseTree *args){
    ParseTree *tree = newExpressionTree(CallE);
    tree->child[0] = args;
    tree->attr.name = name;
    return tree;
}

static const char *typeString(ExprType type){
    switch(type){
        case Void:
        return "Void";
        case Integer:
        return "Integer";
        case Boolean:
        return "Boolean";
    }
    return "";
}

void printTree(FILE *stream, ParseTree *tree){
    INDENT;
    while(tree != NULL){
        TABS;
        switch(tree->nodeKind){
            case StmtK:
            switch(tree->kind.stmt){
                case ExprS:
                PRINT_STATEMENT(Expression); break;
                case IfS:
                PRINT_STATEMENT(If); break;
                case WhileS:
                PRINT_STATEMENT(While); break;
                case ReturnS:
                PRINT_STATEMENT(Return); break;
                case CompoundS:
                PRINT_STATEMENT(Compound); break;
            }
            break;
            case ExprK:
            switch(tree->kind.expr){
                case LogE:
                switch(tree->attr.logOp){
                    case OP_AND:
                    PRINT_OPERATOR(&&); break;
                    case OP_OR:
                    PRINT_OPERATOR(||); break;
                    case OP_NOT:
                    PRINT_OPERATOR(not); break;
                }
                break;
                case RelE:
                switch(tree->attr.relOp){
                    case OP_LE:
                    PRINT_OPERATOR(<=); break;
                    case OP_LT:
                    PRINT_OPERATOR(<); break;
                    case OP_GT:
                    PRINT_OPERATOR(>); break;
                    case OP_GE:
                    PRINT_OPERATOR(>=); break;
                    case OP_EQ:
                    PRINT_OPERATOR(==); break;
                    case OP_NE:
                    PRINT_OPERATOR(!=); break;
                }
                break;
                case AddE:
                switch(tree->attr.addOp){
                    case OP_ADD:
                    PRINT_OPERATOR(+); break;
                    case OP_SUB:
                    PRINT_OPERATOR(-); break;
                }
                break;
                case MulE:
                switch(tree->attr.mulOp){
                    case OP_MUL:
                    PRINT_OPERATOR(*); break;
                    case OP_DIV:
                    PRINT_OPERATOR(/); break;
                }
                break;
                case ConstE:
                fprintf(stream, "[%s constant \"%d\"]\n", typeString(tree->type), tree->attr.val);
                break;
                case IdE:
                fprintf(stream, "[Identifier \"%s\"]\n", tree->attr.name); break;
                case AssignE:
                fprintf(stream, "[Assignment]\n"); break;
                case CallE:
                fprintf(stream, "[Function call \"%s\"]\n", tree->attr.name); break;
            }
            break;
            case DeclK:
            switch(tree->kind.decl){
                case VarD:
                if (tree->isParam)
                    fprintf(stream, "[Parameter declaration \"%s\" of type \"%s\"]\n", tree->attr.name, typeString(tree->type));
                else
                    fprintf(stream, "[Variable declaration \"%s\" of type \"%s\"]\n", tree->attr.name, typeString(tree->type));
                break;
                case ArrD:
                if (tree->isParam)
                    fprintf(stream, "[Array parameter declaration \"%s\" of type \"%s\"]\n", tree->attr.name, typeString(tree->type));
                else
                    fprintf(stream, "[Array declaration \"%s\" of type \"%s\" and length \"%d\"]\n", tree->attr.name, typeString(tree->type), tree->array_value);
                break;
                case FunD:
                fprintf(stream, "[Function declaration \"%s()\" of return type \"%s\"]\n", tree->attr.name, typeString(tree->type));
                break;
            }
            break;
        }
        for(int i = 0; i < MAXCHILDREN; i++){
            printTree(stream, tree->child[i]);
        }
        tree = tree->sibling;
    }
    UNINDENT;
}
