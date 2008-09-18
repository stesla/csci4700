PROGNAME=c--

YACC=bison
LEX=flex
CC=gcc
CFLAGS=-g
LDFLAGS=

OBJS=input.o lexer_util.o main.o

${PROGNAME}: ${OBJS} parser.o lexer.o
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

lexer.c: lexer.l
	${LEX} -d -t $^ > $@

parser.c: parser.y
	${YACC} -t --defines=y.tab.h -o $@ $^

lexer.o: lexer.c y.tab.h parser.h
lexer_util.o: y.tab.h parser.h
main.o: parser.h
y.tab.h: parser.c

clean:
	rm *.o c-- parser.c lexer.c y.tab.h
