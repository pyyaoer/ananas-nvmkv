%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(char *);
extern FILE* yyin;
typedef char* str;
#define YYSTYPE str
%}
%token COMMENT DEV PNUM CSIZE EQUAL INTEGER STRING

%%

parser:
parser sentence
| sentence
|
;
sentence:
COMMENT
| DEV EQUAL STRING {printf("Device: %s\n", $3);}
| PNUM EQUAL INTEGER {printf("Pool Num: %d\n", atoi($3));}
| CSIZE EQUAL INTEGER {printf("Cache Size: %d\n", atoi($3));}
;

%%

void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}

int main(int argc, char* argv[]) {
	yyin = fopen(argv[1],"r");
	yyparse();
	fclose(yyin);
	return 0;
}
