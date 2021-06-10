%{
    #include <string.h>
    #include "globals.h"
    #include "lexer_interface.h"
    #include "lexer_utils.h"
    #define NOVAL yylval.none = NULL

    FILE *scan_file = NULL;
    bool traceScan = false;
    int lineno = 0;

    static bool firstTime = true;

    static TokenType currentToken;
%}

idPattern [a-zA-Z]+
numPattern [0-9]+

%x COMMENT
%%
<INITIAL>{
          /*
          * Reserved words
          */
    "bool"          { NOVAL; return BOOL; }
    "if"            { NOVAL; return IF; }
    "int"           { NOVAL; return INT; }
    "else"          { NOVAL; return ELSE; }
    "not"           { NOVAL; return NOT; }
    "return"        { NOVAL; return RETURN; }
    "true"          { NOVAL; return TRUE; }
    "false"         { NOVAL; return FALSE; }
    "void"          { NOVAL; return VOID; }
    "while"         { NOVAL; return WHILE; }

        /*
         * Special symbols
         */
    "+"             { NOVAL; return PLUS; }
    "-"             { NOVAL; return MINUS; }
    "*"             { NOVAL; return TIMES; }
    "/"             { NOVAL; return DIV; }
    "<"             { NOVAL; return LT; }
    "<="            { NOVAL; return LE; }
    ">"             { NOVAL; return GT; }
    ">="            { NOVAL; return GE; }
    "=="            { NOVAL; return EQ; }
    "!="            { NOVAL; return NE; }
    "="             { NOVAL; return ASSIGN; }
    "&&"            { NOVAL; return AND; }
    "||"            { NOVAL; return OR; }
    ";"             { NOVAL; return SEMI; }
    ","             { NOVAL; return COMMA; }
    "("             { NOVAL; return LPAREN; }
    ")"             { NOVAL; return RPAREN; }
    "["             { NOVAL; return LBRACK; }
    "]"             { NOVAL; return RBRACK; }
    "{"             { NOVAL; return LCBRACK; }
    "}"             { NOVAL; return RCBRACK; }
    "/*"            { BEGIN(COMMENT); }
    "*/"            { NOVAL; lexerError("Unmatched */"); return YYerror; }

        /*
         * Other tokens
         */
    {idPattern}     { yylval.name = strdup(yytext); return ID; }
    {numPattern}    { yylval.val = atoi(yytext); return NUM; }
    [ \t\r]         { /* Consume whitespace */ }
    .               { NOVAL; lexerError(yytext); return YYUNDEF; }
    <<EOF>>         { NOVAL; return YYEOF; }
}
<COMMENT>{
    "*/"        { BEGIN(INITIAL); }
    .           { /* Consume all characters that aren't a newline */ }
    <<EOF>>     { NOVAL; lexerError("EOF in comment"); return YYEOF; }
}

<COMMENT,INITIAL>\n    { lineno++; }
%%
void setSourceFile(FILE *source){
    yyin = source;
}

static void printToken(TokenType token, char *tokenString){
    fprintf(scan_file, "%d: ", lineno);
    printTokenInfo(scan_file, token, tokenString);
}

TokenType getToken(){
    if (firstTime){
        lineno = 1;
        if (traceScan) {
            fprintf(scan_file, "C- COMPILATION: %s\n", input_filename);
        }
        firstTime = false;
    }
    currentToken = yylex();
    if (traceScan) {
        printToken(currentToken, yytext);
    }
    return currentToken;
}

void printCurrentToken(FILE *stream){
    fprintf(stream, "Current token: ");
    printTokenInfo(stream, currentToken, yytext);
}
