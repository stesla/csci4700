PROGNAME=c--

YACC=bison
LEX=flex
CC=gcc
CFLAGS=-g
LDFLAGS=

AST_OBJS=ast_array.o ast_binary.o ast_block.o ast_call.o ast_constant.o \
	     ast_conditional.o ast_declare.o ast_for.o ast_formal.o ast_function.o \
	     ast_group.o ast_identifier.o ast_list.o ast_postfix.o ast_prefix.o \
	     ast_read.o ast_return.o ast_string_literal.o ast_while.o ast_write.o

OBJS=util.o ast.o lexer_util.o parser.o lexer.o ${AST_OBJS}

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
	rm *.o c-- run-test parser.c lexer.c y.tab.h

## File Dependencies

lexer.o: lexer.c y.tab.h parser.h
lexer_util.o: y.tab.h parser.h
main.o: parser.h
parser.o: parser.c
y.tab.h: parser.c

ast_array.o: ast.h
ast_binary.o: ast.h
ast_block.o: ast.h
ast_call.o: ast.h
ast_constant.o: ast.h
ast_conditional.o: ast.h
ast_declare.o: ast.h
ast_for.o: ast.h
ast_formal.o: ast.h
ast_function.o: ast.h
ast_group.o: ast.h
ast_identifier.o: ast.h
ast_list.o: ast.h
ast_postfix.o: ast.h
ast_prefix.o: ast.h
ast_read.o: ast.h
ast_return.o: ast.h
ast_string_literal.o: ast.h
ast_while.o: ast.h
ast_write.o: ast.h
