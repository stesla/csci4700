PROGNAME=c--

YACC=../tools/bin/bison
LEX=../tools/bin/flex
CC=gcc
CFLAGS=-g
LDFLAGS=

OBJS = $(patsubst %.c,%.o,$(wildcard *.c)) parser.o lexer.o

${PROGNAME}: ${OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

lexer.c: lexer.lex
	${LEX} -d -t $^ > $@

parser.c: parser.y
	${YACC} -t --defines=y.tab.h -o $@ $^

## Utilities

clean:
	rm -rf *.o c-- lexer.c parser.c run-test y.tab.h

## File Dependencies

${AST_OBJS}: ast.h
lexer.o lexer_util.o: y.tab.h parser.h
main.o: parser.h
y.tab.h: parser.c
