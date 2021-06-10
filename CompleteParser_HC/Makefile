CC = gcc
LIBS = -lfl
CFLAGS = -Wall -g
NAME = analyzer

HARD_SRCS = lexer_utils.c main.c parser_utils.c parseTree.c symbolTable.c analyzer.c
GEN_SRCS = CM.tab.c lex.yy.c
SRCS = $(sort $(HARD_SRCS) $(GEN_SRCS))

HARD_HDRS = globals.h lexer_interface.h lexer_utils.h parser_interface.h parser_utils.h parseTree.h symbolTable.h analyzer_interface.h
GEN_HDRS = CM.tab.h
HDRS = $(sort $(HARD_HDRS) $(GEN_HDRS))

OBJS = ${SRCS:.c=.o}

$(NAME): $(OBJS) Makefile
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

%.o: %.c Makefile $(HDRS)
	$(CC) $(CFLAGS) -c $<

lex.yy.c: CM.flex
	flex CM.flex

CM.tab.c CM.tab.h &: CM.y
	bison -d CM.y

.PHONY: clean objs srcs

clean:
	rm -f $(NAME) $(OBJS) $(GEN_SRCS) $(GEN_HDRS)

objs:
	echo $(OBJS)

srcs:
	echo $(SRCS)
