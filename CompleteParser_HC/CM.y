%code requires {
    #include "parseTree.h"
}

%union {
    int val;
    char *name;
    ParseTree *ptr;
    void *none;
}

%code provides {
    typedef yytoken_kind_t TokenType;
}

%code {
    #include <stdio.h>
    #include <stdbool.h>
    #include "lexer_interface.h"
    #include "parser_interface.h"
    #include "parser_utils.h"
    static int yylex();
    static void yyerror(const char *s);
    static ParseTree *topLevelTree;

    FILE *parse_file = NULL;
    bool traceParse = false;
}

%define parse.error detailed

/*
 * Reserved words
 */
%token <none> BOOL
%token <none> IF
%token <none> INT
%token <none> ELSE
%token <none> NOT
%token <none> RETURN
%token <none> TRUE
%token <none> FALSE
%token <none> VOID
%token <none> WHILE
/*
 * Special Symbols
 */
%token <none> PLUS
%token <none> MINUS
%token <none> TIMES
%token <none> DIV
%token <none> LE
%token <none> LT
%token <none> GE
%token <none> GT
%token <none> EQ
%token <none> NE
%token <none> ASSIGN
%token <none> AND
%token <none> OR
%token <none> SEMI
%token <none> COMMA
%token <none> LPAREN
%token <none> RPAREN
%token <none> LBRACK
%token <none> RBRACK
%token <none> LCBRACK
%token <none> RCBRACK
/*
 * Infinite tokens
 */
%token <name> ID
%token <val> NUM
/*
 * Nonterminal type declarations
 */
%type <none> program
%type <ptr> declarationList declaration varDeclaration funDeclaration params
%type <ptr> paramList param compoundStmt localDeclarations statementList
%type <ptr> statement expressionStmt selectionStmt iterationStmt returnStmt
%type <ptr> expression var orExpression andExpression notExpression
%type <ptr> logicalAtom relExpression addExpression mulExpression mathAtom
%type <ptr> call args argList

%type <val> typeSpecifier relop addop mulop
/*
 * Associativity declarations
 */
%nonassoc RPAREN
%nonassoc ELSE
%left OR
%left AND
%nonassoc LE LT GE GT EQ NE
%%
program:            declarationList { $$ = NULL; topLevelTree = $1; };

declarationList:    declarationList[left] declaration[right] {
                        $$ = appendSibling($left, $right);
                    }
                    | declaration { $$ = $1; };

declaration:        varDeclaration { $$ = $1; }
                    | funDeclaration { $$ = $1; };

varDeclaration:     typeSpecifier[type] ID[name] SEMI {
                        $$ = newVarDeclaration($type, $name);
                        $$->lineno = lineno;
                    }
                    | typeSpecifier[type] ID[name] LBRACK NUM[len] RBRACK SEMI {
                        $$ = newArrayDeclaration($type, $name, $len);
                        $$->lineno = lineno;
                    };

typeSpecifier:      INT { $$ = Integer; }
                    | BOOL { $$ = Boolean; }
                    | VOID { $$ = Void; };

funDeclaration:     typeSpecifier[type] ID[name] LPAREN params RPAREN compoundStmt[body] {
                        $$ = newFunDeclaration($type, $name, $params, $body);
                        $$->lineno = lineno;
                    };

params:             paramList { $$ = $1; }
                    | VOID { $$ = NULL; };

paramList:          paramList[left] COMMA param[right] {
                        $$ = appendSibling($left, $right);
                    }
                    | param { $$ = $1; };

param:              typeSpecifier[type] ID[name] {
                        $$ = newParameter($type, $name, false);
                        $$->lineno = lineno;
                    }
                    | typeSpecifier[type] ID[name] LBRACK RBRACK {
                        $$ = newParameter($type, $name, true);
                        $$->lineno = lineno;
                    };

compoundStmt:       LCBRACK localDeclarations[decls] statementList[stmts] RCBRACK {
                        $$ = newCompoundStatement($decls, $stmts);
                        $$->lineno = lineno;
                    };

localDeclarations:  localDeclarations[left] varDeclaration[right] {
                        $$ = appendSibling($left, $right);
                    }
                    | %empty { $$ = NULL; };

statementList:      statementList[left] statement[right] {
                        $$ = appendSibling($left, $right);
                    }
                    | %empty { $$ = NULL; };

statement:          expressionStmt { $$ = $1; }
                    | compoundStmt { $$ = $1; }
                    | selectionStmt { $$ = $1; }
                    | iterationStmt { $$ = $1; }
                    | returnStmt { $$ = $1; };

expressionStmt:     expression SEMI { $$ = $1; }
                    | SEMI { $$ = NULL; };

selectionStmt:      IF LPAREN expression[cond] RPAREN statement {
                        $$ = newIfStatement($cond, $statement, NULL);
                        $$->lineno = lineno;
                    }
                    | IF LPAREN expression[cond] RPAREN statement[ifbody] ELSE statement [elsebody] {
                        $$ = newIfStatement($cond, $ifbody, $elsebody);
                        $$->lineno = lineno;
                    };

iterationStmt:      WHILE LPAREN expression[cond] RPAREN statement {
                        $$ = newWhileStatement($cond, $statement);
                        $$->lineno = lineno;
                    };

returnStmt:         RETURN expression SEMI {
                        $$ = newReturnStatement($expression);
                        $$->lineno = lineno;
                    }
                    | RETURN SEMI {
                        $$ = newReturnStatement(NULL);
                        $$->lineno = lineno;
                    };

expression:         var ASSIGN expression[value] {
                        $$ = newAssignExpression($var, $value);
                        $$->lineno = lineno;
                    }
                    | orExpression { $$ = $1; };

var:                ID {
                        $$ = newVarExpression($ID, NULL);
                        $$->lineno = lineno;
                    }
                    | ID LBRACK expression RBRACK {
                        $$ = newVarExpression($ID, $expression);
                        $$->lineno = lineno;
                    }

orExpression:       orExpression[left] OR andExpression[right] {
                        $$ = newLogicalExpression($left, OP_OR, $right);
                        $$->lineno = lineno;
                    }
                    | andExpression { $$ = $1; };

andExpression:      andExpression[left] AND notExpression[right] {
                        $$ = newLogicalExpression($left, OP_AND, $right);
                        $$->lineno = lineno;
                    }
                    | notExpression { $$ = $1; }

notExpression:      NOT notExpression {
                        $$ = newLogicalExpression(NULL, OP_NOT, $2);
                        $$->lineno = lineno;
                    }
                    | logicalAtom { $$ = $1; };

logicalAtom:        relExpression { $$ = $1; }
                    | TRUE {
                        $$ = newBooleanConstant(true);
                        $$->lineno = lineno;
                    }
                    | FALSE {
                        $$ = newBooleanConstant(false);
                        $$->lineno = lineno;
                    };

relExpression:      addExpression[left] relop addExpression[right] {
                        $$ = newRelativeExpression($left, $relop, $right);
                        $$->lineno = lineno;
                    }
                    | addExpression { $$ = $1; }

relop:              LE { $$ = OP_LE; }
                    | LT { $$ = OP_LT; }
                    | GT { $$ = OP_GT; }
                    | GE { $$ = OP_GE; }
                    | EQ { $$ = OP_EQ; }
                    | NE { $$ = OP_NE; };

addExpression:      addExpression[left] addop mulExpression[right] {
                        $$ = newAdditiveExpression($left, $addop, $right);
                        $$->lineno = lineno;
                    }
                    | mulExpression { $$ = $1; };

addop:              PLUS { $$ = OP_ADD; }
                    | MINUS { $$ = OP_SUB; };

mulExpression:      mulExpression[left] mulop mathAtom[right] {
                        $$ = newMultiplicativeExpression($left, $mulop, $right);
                        $$->lineno = lineno;
                    }
                    | mathAtom { $$ = $1; };

mulop:              TIMES { $$ = OP_MUL; }
                    | DIV { $$ = OP_DIV; };

mathAtom:           LPAREN expression RPAREN { $$ = $2; }
                    | var { $$ = $1; }
                    | call { $$ = $1; }
                    | NUM {
                        $$ = newIntegerConstant($NUM);
                        $$->lineno = lineno;
                    };

call:               ID LPAREN args RPAREN {
                        $$ = newCallExpression($ID, $args);
                        $$->lineno = lineno;
                    };

args:               argList { $$ = $1; }
                    | %empty { $$ = NULL; };

argList:            argList[left] COMMA expression[right] {
                        $$ = appendSibling($left, $right);
                    }
                    | expression { $$ = $1; };
%%
static int yylex() {
    return getToken();
}

void yyerror(const char *s){
    fprintf(stderr, "Syntax error at line %d: %s\n", lineno, s);
    printCurrentToken(stderr);
}

ParseTree *parse(){
    yyparse();
    if (traceParse){
        printTree(parse_file, topLevelTree);
    }
    return topLevelTree;
}
