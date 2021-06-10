#include "lexer_utils.h"

void printTokenInfo(FILE *stream, TokenType token, const char *tokenString){
    switch(token){
        case BOOL:
        case IF:
        case INT:
        case ELSE:
        case NOT:
        case RETURN:
        case TRUE:
        case FALSE:
        case VOID:
        case WHILE:
            fprintf(stream, "reserved word: %s\n", tokenString);
            break;
        case PLUS:
        case MINUS:
        case TIMES:
        case DIV:
        case LE:
        case LT:
        case GE:
        case GT:
        case EQ:
        case NE:
        case ASSIGN:
        case AND:
        case OR:
        case SEMI:
        case COMMA:
        case LPAREN:
        case RPAREN:
        case LBRACK:
        case RBRACK:
        case LCBRACK:
        case RCBRACK:
            fprintf(stream, "special symbol: %s\n", tokenString);
            break;
        case ID:
            fprintf(stream, "ID, name = %s\n", tokenString);
            break;
        case NUM:
            fprintf(stream, "NUM, value = %s\n", tokenString);
            break;
        case YYEOF:
            fprintf(stream, "EOF\n");
            break;
        case YYEMPTY:
        case YYerror:
        case YYUNDEF:
            lexerError("Invalid token");
            break;
    }
}

void lexerError(const char *errorString){
    fprintf(scan_file, "%d: ERROR: %s\n", lineno, errorString);
}
