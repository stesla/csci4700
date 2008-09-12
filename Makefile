PROGNAME=c--

YACC=bison
LEX=flex
CC=g++
CFLAGS=-g
LDFLAGS=

${PROGNAME}: ${OBJS} parser.o lexer.o
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

lexer.c: lexer.l
	${LEX} -d -t $^ > $@

parser.c: parser.y
	${YACC} -t --defines=y.tab.h -o $@ $^

lexer.o: lexer.c y.tab.h parser.h
y.tab.h: parser.c

clean:
	rm *.o c-- parser.c lexer.c y.tab.h
