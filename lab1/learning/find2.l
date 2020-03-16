%option yylineno
%{
#include "syntax.tab.h"
int chars = 0;
int words = 0;
int lines = 0;

%}
letter_ [A-Za-z_]
digit [0-9]
digits [0-9]+
OCT 0[0-7]+
OCTERROR 0[0-7]*[8-9]+[0-9]*
HEX 0[xX][0-9a-fA-F]+
DEC   0|([1-9][0-9]*)
FLOAT   {digits}\.{digits}|(\.{digits}|{digits}\.|{digits}\.{digits}|{digits})([eE][+-]?{digits})
FLOATERROR (\.{digits}|{digits}\.|{digits}\.{digits})([eE][+-]?)
ID {letter_}({letter_}|{digit})*
TYPE int|float
LP  "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"
LCOMMENT "/*"
COMMENT "//"
RELOP >|<|>=|<=|==|!=
WhiteSpace  [ \t\r\n]+
%%
{HEX} {yylval.type_int=yytext;return INT;}
{OCT} {yylval.type_int=yytext;return INT;}
{OCTERROR}    {printf("To be done OCTERROR\n");}
{DEC} {printf("DEC\n");yylval.type_int=atoi(yytext);return INT;}
{FLOAT} {printf("FLOAT\n");return FLOAT;}
{FLOATERROR}    {printf("To be done !FLOATERROR\n");}
{LCOMMENT} {printf("LCOMMENT\n");char a=input();char b=input();while(a!='*'||b!='/'){/*printf("%c,%c\n",a,b);*/a=b;b=input();}}
{COMMENT} {char a=input();while(a!='\n')a=input();printf("COMMENT\n");}
";" {printf("SEMI\n");return SEMI;}
"," {printf("comma\n");return COMMA; }
"=" {printf("assignop\n");return ASSIGNOP;}
{RELOP} {printf("RELOP\n");return RELOP;}
"+" {printf("plus\n");return PLUS;}
"-" {printf("minus\n");return MINUS;}
"*" {printf("star\n");return STAR;}
"/" {printf("div\n");return DIV;}
"&&"  {printf("and\n");return AND;}
"||"  {printf("or\n");return OR;}
"." {printf("dot\n");return DOT;}
"!" {printf("not\n");return NOT;}
{TYPE} {printf("type\n");return TYPE;}
{LP}  {printf("LP\n");return LP;}
{RP} {printf("RP\n");return RP;}
{LB} {printf("LB\n");return LB;}
{RB} {printf("RB\n");return RB;}
{LC} {printf("LC\n");return LC;}
{RC} {printf("RC\n");return RC;}
"struct" {printf("STRUCT\n");return STRUCT;}
"return"  {printf("return\n");return RETURN;}
"if"  {printf("if\n");return IF;}
"else"  {printf("else\n");return ELSE;}
"while" {printf("while\n");return WHILE;}
{ID} {printf("ID\n");;return ID;}
{WhiteSpace}+ {   /*Let it go!*/ ;      }
.   { printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno, yytext);}
%%
int yywrap()
{ 
    return 1;
}

