#ifndef _PARSER_UTILS_H
# define _PARSER_UTILS_H
# include <stdio.h>
# include <stdbool.h>
# include "parseTree.h"

ParseTree *appendSibling(ParseTree *left, ParseTree *right);
ParseTree *newVarDeclaration(ExprType type, char *name);
ParseTree *newArrayDeclaration(ExprType type, char *name, int len);
ParseTree *newFunDeclaration(ExprType type, char *name, ParseTree *params, ParseTree *body);
ParseTree *newParameter(ExprType type, char *name, bool isArray);
ParseTree *newExpressionStatement(ParseTree *expression);
ParseTree *newCompoundStatement(ParseTree *declarations, ParseTree *statements);
ParseTree *newIfStatement(ParseTree *condition, ParseTree *ifbody, ParseTree *elsebody);
ParseTree *newWhileStatement(ParseTree *condition, ParseTree *body);
ParseTree *newReturnStatement(ParseTree *returnValue);
ParseTree *newAssignExpression(ParseTree *var, ParseTree *value);
ParseTree *newVarExpression(char *name, ParseTree *index);
ParseTree *newLogicalExpression(ParseTree *left, LogOp op, ParseTree *right);
ParseTree *newBooleanConstant(bool val);
ParseTree *newRelativeExpression(ParseTree *left, RelOp op, ParseTree *right);
ParseTree *newAdditiveExpression(ParseTree *left, AddOp op, ParseTree *right);
ParseTree *newMultiplicativeExpression(ParseTree *left, MulOp op, ParseTree *right);
ParseTree *newIntegerConstant(int val);
ParseTree *newCallExpression(char *name, ParseTree *args);

void printTree(FILE *stream, ParseTree *tree);

#endif
