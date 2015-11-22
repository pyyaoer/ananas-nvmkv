CC=gcc
YACC=bison
LEX=flex
CFLAGS= -I..

test: lex.yy.o test.tab.o
	$(CC) $^ -o $@	

lex.yy.o: test.tab.h

test.tab.o: test.tab.h ../kvf-parser.h

test.tab.c test.tab.h: test.y
	$(YACC) -d test.y

lex.yy.c: test.l
	$(LEX) test.l

clean:
	rm -f test.tab.* test.output lex.yy.* test
