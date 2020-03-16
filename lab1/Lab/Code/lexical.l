%option yylineno

%{
#include "syntax.tab.h"
//#define DEBUGNOW
void printfdebug(char *str){
    #ifdef DEBUGNOW
    printf("debug:Line %d :input  %s : %s\n",yylineno,str,yytext);
    #endif
}

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
{HEX} {printfdebug("HEX");return INT;}
{OCT} {printfdebug("OCT");return INT;}
{OCTERROR}    {printf("To be done OCTERROR\n");}
{DEC} {printfdebug("DEC");return INT;}
{FLOAT} {printfdebug("FLOAT");return FLOAT;}
{FLOATERROR}    {printf("To be done !FLOATERROR\n");}
{LCOMMENT} {printf("LCOMMENT\n");char a=input();char b=input();while(a!='*'||b!='/'){/*printf("%c,%c\n",a,b);*/a=b;b=input();}}
{COMMENT} {char a=input();while(a!='\n')a=input();printf("COMMENT\n");}
";" {printfdebug("SEMI");return SEMI;}
"," {printfdebug("comma");return COMMA; }
"=" {printfdebug("assignop");return ASSIGNOP;}
{RELOP} {printfdebug("RELOP");return RELOP;}
"+" {printfdebug("plus");return PLUS;}
"-" {printfdebug("minus");return MINUS;}
"*" {printfdebug("star");return STAR;}
"/" {printfdebug("div");return DIV;}
"&&"  {printfdebug("and");return AND;}
"||"  {printfdebug("or");return OR;}
"." {printfdebug("dot");return DOT;}
"!" {printfdebug("not");return NOT;}
{TYPE} {printfdebug("type");return TYPE;}
{LP}  {printfdebug("LP");return LP;}
{RP} {printfdebug("RP");return RP;}
{LB} {printfdebug("LB");return LB;}
{RB} {printfdebug("RB");return RB;}
{LC} {printfdebug("LC");return LC;}
{RC} {printfdebug("RC");return RC;}
"struct" {printfdebug("STRUCT");return STRUCT;}
"return"  {printfdebug("return");return RETURN;}
"if"  {printfdebug("if");return IF;}
"else"  {printfdebug("else");return ELSE;}
"while" {printfdebug("while");return WHILE;}
{ID} {printfdebug("ID");return ID;}
{WhiteSpace}+ {   /*Let it go!*/ ;      }
.   { printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno, yytext);}
%%
int yywrap()
{ 
    return 1;
}



