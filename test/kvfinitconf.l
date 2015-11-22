%{
#include <string.h>
#include "kvfinitconf.tab.h"
typedef char* str;
extern YYSTYPE yylval;
%}

char [^ \"\t\n]
num [0-9]
empty [ \t]
tail {empty}*=

%%

#([^\n]*)$ /* comment */ {return COMMENT;}
{empty} /* ignore whitespace */
[Dd][Ee][Vv]{tail} /* device name*/{return DEV;}
[Pp][Nn][Uu][Mm]{tail} /* pool num*/ {return PNUM;}
[Cc][Ss][Ii][Zz][Ee]{tail} /* cache size*/ {return CSIZE;}
{num}+ /* integer */ {yylval = strdup(yytext); return INTEGER;}
\"{char}+\" /* common string */ {yylval = strdup(yytext); return STRING;}

%%

int yywrap(void) {
return 1;
}
