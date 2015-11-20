%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(char *);
extern FILE* yyin;
typedef char* str;
#define YYSTYPE str
%}
%token COMMENT DEV PNUM CSIZE INTEGER STRING

%%

parser:
sentence parser
|
;
sentence:
COMMENT
| DEV STRING {printf("Device: %s\n", $2);}
| PNUM INTEGER {printf("Pool Num: %d\n", atoi($2));}
| CSIZE INTEGER {printf("Cache Size: %d\n", atoi($2));}
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
