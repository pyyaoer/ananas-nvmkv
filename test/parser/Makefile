CC=gcc
YACC=bison
LEX=flex

test: lex.yy.c test.tab.c
	$(CC) $^ -o $@

test.tab.c: test.y
	$(YACC) -d test.y

lex.yy.c: test.l
	$(LEX) test.l

clean:
	rm -f test.tab.* lex.yy.c test
