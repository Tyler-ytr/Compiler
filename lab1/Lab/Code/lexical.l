%option yylineno

%{
#include "syntax.tab.h"
//#define DEBUGNOW
typedef enum {
    LEX_INT=0,
    LEX_FLOAT=1,
    LEX_ID=2,
    LEX_TYPE=3,
    OTHERS
}NODE_TYPE;

void printfdebug(char *str){
    #ifdef DEBUGNOW
    printf("debug:Line %d :input  %s : %s\n",yylineno,str,yytext);
    #endif
}
typedef struct Node{//参考结构 https://segmentfault.com/a/1190000002451356
    struct Node *child;//第一个孩子;
    struct Node* next_sib;//下一个兄弟节点
    char name[32];//节点名称
    union{
        int int_contant;
        float float_contant;
        char string_contant[32];//包含的内容
    };
    int place;//节点的类型,0表示lexical里面的,1表示syntax里面的;
    int column;//表示行数;
    NODE_TYPE type;// 0 表示int,1表示float,2表示ID,3表示type;
}NODE;

void add_lexnode(char * Name,NODE_TYPE Type);


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
{HEX} {printfdebug("HEX");
                add_lexnode("INT",LEX_INT);
                sscanf(yytext,"%x",&yylval.node->int_contant);
                //printf("%d,0x%x\n",yylval.node->int_contant,yylval.node->int_contant);
               return INT;}
{OCT} {printfdebug("OCT");
                add_lexnode("INT",LEX_INT);
                sscanf(yytext,"%o",&yylval.node->int_contant);
                //printf("%d,%o\n",yylval.node->int_contant,yylval.node->int_contant);
                return INT;}
{OCTERROR}    {printf("To be done OCTERROR\n");}
{DEC} {printfdebug("DEC");
               add_lexnode("INT",LEX_INT);
               sscanf(yytext,"%d",&yylval.node->int_contant);
               return INT;}
{FLOAT} {printfdebug("FLOAT");
                    add_lexnode("FLOAT",LEX_FLOAT);
                    sscanf(yytext,"%f",&yylval.node->float_contant);
                   //    printf("%f,%s\n",yylval.node->float_contant,yytext);
                    return FLOAT;}
{FLOATERROR}    {printf("To be done !FLOATERROR\n");}
{LCOMMENT} { //printf("LCOMMENT\n");
                            char a=input();char b=input();while(a!='*'||b!='/'){/*printf("%c,%c\n",a,b);*/a=b;b=input();}}
{COMMENT} {char a=input();while(a!='\n')a=input();
                            //printf("COMMENT\n");
}
";" {printfdebug("SEMI");
        add_lexnode("SEMI",OTHERS);    
        return SEMI;}
"," {printfdebug("comma");
       add_lexnode("COMMA",OTHERS);
       return COMMA; }
"=" {printfdebug("assignop");
        add_lexnode("ASSIGNOP",OTHERS);
         return ASSIGNOP;}
{RELOP} {printfdebug("RELOP");
        add_lexnode("RELOP",OTHERS);
         return RELOP;}
"+" {printfdebug("plus");
        add_lexnode("PLUS",OTHERS);
        return PLUS;}
"-" {printfdebug("minus");
        add_lexnode("MINUS",OTHERS);
        return MINUS;}
"*" {printfdebug("star");
        add_lexnode("STAR",OTHERS);
        return STAR;}
"/" {printfdebug("div");
        add_lexnode("DIV",OTHERS);
        return DIV;}
"&&"  {printfdebug("and");
        add_lexnode("AND",OTHERS);
        return AND;}
"||"  {printfdebug("or");
        add_lexnode("OR",OTHERS);
        return OR;}
"." {printfdebug("dot");
        add_lexnode("DOT",OTHERS);
        return DOT;}
"!" {printfdebug("not");
        add_lexnode("NOT",OTHERS);
        return NOT;}
{TYPE} {printfdebug("type");
        add_lexnode("TYPE",LEX_TYPE);
        strcpy(yylval.node->string_contant,yytext);
        //printf("%s\n",yylval.node->string_contant);
        return TYPE;}
{LP}  {printfdebug("LP");
        add_lexnode("LP",OTHERS);
        return LP;}
{RP} {printfdebug("RP");
        add_lexnode("RP",OTHERS);
        return RP;}
{LB} {printfdebug("LB");
        add_lexnode("LB",OTHERS);
        return LB;}
{RB} {printfdebug("RB");
        add_lexnode("RB",OTHERS);
        return RB;}
{LC} {printfdebug("LC");
        add_lexnode("LC",OTHERS);
        return LC;}
{RC} {printfdebug("RC");
        add_lexnode("RC",OTHERS);
        return RC;}
"struct" {printfdebug("STRUCT");
        add_lexnode("STRUCT",OTHERS);
        return STRUCT;}
"return"  {printfdebug("return");
        add_lexnode("RETURN",OTHERS);
        return RETURN;}
"if"  {printfdebug("if");
        add_lexnode("IF",OTHERS);
        return IF;}
"else"  {printfdebug("else");
        add_lexnode("ELSE",OTHERS);
        return ELSE;}
"while" {printfdebug("while");
        add_lexnode("WHILE",OTHERS);
        return WHILE;}
{ID} {printfdebug("ID");
        add_lexnode("ID",OTHERS);
        strcpy(yylval.node->string_contant,yytext);
    //    printf("%s\n",yylval.node->string_contant);
        return ID;}
{WhiteSpace}+ {   /*Let it go!*/ ;      }
.   { printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno, yytext);}
%%
int yywrap()
{ 
    return 1;
}
void add_lexnode(char * Name,NODE_TYPE Type){
//    printf("In add lexnode!!\n");
    yylval.node=(NODE*)malloc(sizeof(NODE));
    yylval.node->child=NULL;
    yylval.node->next_sib=NULL;
    yylval.node->column=yylineno;
    strcpy(yylval.node->name,Name);
    yylval.node->place=0;
    yylval.node->type=Type;
}



