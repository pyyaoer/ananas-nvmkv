%{
#include <string.h>
#include "test.tab.h"
typedef char* str;
extern YYSTYPE yylval;
%}

char [^ \"\t\n]
num [0-9]

%%

#([^\n]*)$ /* comment */ {return COMMENT;}
[ \t] /* ignore whitespace */
[Dd][Ee][Vv] /* device name*/{return DEV;}
[Pp][Nn][Uu][Mm] /* pool num*/ {return PNUM;}
[Cc][Ss][Ii][Zz][Ee] /* cache size*/ {return CSIZE;}
= /* equal to */ {return EQUAL;}
{num}+ /* integer */ {yylval = strdup(yytext); return INTEGER;}
\"{char}+\" /* common string */ {yylval = strdup(yytext); return STRING;}

%%

int yywrap(void) {
return 1;
}
