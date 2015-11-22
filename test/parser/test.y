%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kvf-parser.h"
int yylex(void);
void yyerror(char *);
extern FILE* yyin;
typedef char* str;
kvfParser yyKVF;

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
| DEV STRING {
	printf("Device: %s\n", $2);
	int min_len = strlen($2);
	if (min_len > NVMKV_KVF_MAX_DEV_NAME_LEN + 1){
		min_len = NVMKV_KVF_MAX_DEV_NAME_LEN + 1;
	}
	memcpy(yyKVF.devPath, &($2[1]), min_len-2);}
| PNUM INTEGER {printf("Pool Num: %d\n", atoi($2)); yyKVF.poolNum = atoi($2);}
| CSIZE INTEGER {printf("Cache Size: %d\n", atoi($2)); yyKVF.cacheSize = atoi($2);}
;

%%

void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}

int kvf_parser(const char* file, kvfParser* kp){
	if (kp == NULL){
		return -1;
	}
	if ((yyin = fopen(file, "r")) == NULL){
		printf("File %s not found!\n", file);
		return -1;
	}
	yyparse();
	memcpy(kp, &yyKVF, sizeof(kvfParser));
	fclose(yyin);
	return 0;
}

