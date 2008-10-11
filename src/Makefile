PROGNAME=c--

YACC=../tools/bin/bison
LEX=../tools/bin/flex
CC=gcc
CFLAGS=-g
LDFLAGS=

AST_OBJS=ast_array.o ast_binary.o ast_block.o ast_call.o ast_constant.o \
	     ast_conditional.o ast_declare.o ast_for.o ast_formal.o ast_function.o \
	     ast_group.o ast_identifier.o ast_list.o ast_postfix.o ast_prefix.o \
	     ast_read.o ast_return.o ast_string_literal.o ast_while.o ast_write.o

OBJS=ir.o symbol.o util.o ast.o lexer_util.o parser.o lexer.o ${AST_OBJS}

## Main Program

${PROGNAME}: ${OBJS} main.o
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

lexer.c: lexer.lex
	${LEX} -d -t $^ > $@

parser.c: parser.y
	${YACC} -t --defines=y.tab.h -o $@ $^

## Tests

TEST_OBJS=

test: run-test
	./run-test

run-test: ${OBJS} ${TEST_OBJS} test.o
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS} -lcheck

## Utilities

clean:
	rm -rf *.o c-- lexer.c parser.c run-test y.tab.h

## File Dependencies

${AST_OBJS}: ast.h
lexer.o lexer_util.o: y.tab.h parser.h
main.o: parser.h
y.tab.h: parser.c
