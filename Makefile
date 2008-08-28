# Template grammar for 4700 Fall 2005

YACC=bison
LEX=flex
CC=g++
LN=g++
COPT=-g
LOPT=-g
DIFF=diff

parse :	parse.o scanner.o
	$(CC) $(LOPT) -o parse parse.o scanner.o
	rm parse.o scanner.o

# Bison has this annoying habit of sending yydebug to stderr
# so I change that to go to stdout, which I like better.
# Bison puts output in the non-traditional place; move it.
parse.o :	parse.y parse.h
	$(YACC) -d -t parse.y
	sed "s/stderr/stdout/" parse.tab.c >parse.cpp ; rm parse.tab.c
	mv parse.tab.h y.tab.h
	$(CC) $(COPT) -c parse.cpp

scanner.o :	scanner.lex y.tab.h parse.h
	$(LEX) scanner.lex
	mv lex.yy.c scanner.cpp
	$(CC) $(COPT) -c scanner.cpp

clean :	
	rm parse.cpp scanner.cpp *.o parse parse.exe *~ \#*\# y.tab.h ram.image
