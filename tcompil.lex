%{
/* tpc-2018.lex */
#include "tcompil.tab.h"
int lineno = 1;
%}

%option noinput nounput noyywrap
%x COMMENT

%%
[ \t\r]+              ;
"/*"                    { BEGIN COMMENT;}
&&                      { return AND; }
"||"                    { return OR; }
"*"|"/"|%               { yylval.divstar=yytext[0]; return DIVSTAR; }
"+"|-                   { yylval.addsub=yytext[0]; return ADDSUB; }
"<"|"<="|">"|>=         { strcpy(yylval.comp, yytext); return ORDER; }
==|!=                   { strcpy(yylval.comp, yytext); return EQ; }
entier                  { strcpy(yylval.type, yytext); return TYPE; }
caractere               { strcpy(yylval.type, yytext); return TYPE; }
void                    { return VOID; }
const                   { return CONST; }
if                      { return IF;}
while                   { return WHILE; }
return                  { return RETURN; }
else					{ return ELSE; }
reade					{ return READE; }
readc					{ return READC; }
print					{ return PRINT; }
[a-zA-Z][a-zA-Z0-9_]{0,62} { strcpy(yylval.ident, yytext); return IDENT; }
[0-9]+                  { sscanf(yytext, "%d", &(yylval.num));
                               return NUM; }
'\\?.'                  { if (strlen(yytext)==3)
                            yylval.caractere=yytext[1];
                          else switch(yytext[2]) {
                            case 'n': yylval.caractere='\n'; break;
                            case 't': yylval.caractere='\t'; break;
                            case '\'': yylval.caractere='\''; break;
                            }
                          return CARACTERE; }
.                       { return yytext[0];}
<COMMENT>"*/"      		{ BEGIN INITIAL;}
<COMMENT>.    			;
<COMMENT>[\n]			{lineno++;}
[\n] 					{lineno++;}
%%

