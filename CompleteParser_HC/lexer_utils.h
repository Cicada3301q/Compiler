#ifndef _LEXER_UTILS_H
# define _LEXER_UTILS_H

#include "lexer_interface.h"

void printTokenInfo(FILE *stream, TokenType token, const char *tokenString);
void lexerError(const char *errorString);

#endif
