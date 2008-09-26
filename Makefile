PROGNAME=c--

YACC=bison
LEX=flex
CC=gcc
CFLAGS=-g
LDFLAGS=

AST_OBJS=ast_array.o ast_binary.o ast_call.o ast_constant.o ast_declare.o \
         ast_formal.o ast_group.o ast_identifier.o ast_list.o ast_postfix.o \
         ast_prefix.o ast_return.o ast_string_literal.o

OBJS=util.o ast.o input.o lexer_util.o parser.o lexer.o ${AST_OBJS}

## Main Program

${PROGNAME}: ${OBJS} main.o
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

lexer.c: lexer.lex
	${LEX} -d -t $^ > $@

parser.c: parser.y
	${YACC} -t --defines=y.tab.h -o $@ $^

lexer.o: lexer.c y.tab.h parser.h
lexer_util.o: y.tab.h parser.h
main.o: parser.h
y.tab.h: parser.c

## Tests

TEST_OBJS=

test: run-test
	./run-test

run-test: ${OBJS} ${TEST_OBJS} test.o
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS} -lcheck

## Utilities

clean:
	rm *.o c-- run-test parser.c lexer.c y.tab.h
