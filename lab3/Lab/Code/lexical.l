
%{
#include "syntax.tab.h"
#include "common.h"
//#define DEBUGNOW
//#define DEBUGBISONNOW
int lexError=0;
extern int emptyflag;





void printfdebug(char *str){
    #ifdef DEBUGNOW
    printf("debug:Line %d :input  %s : %s\n",yylineno,str,yytext);
    #endif
}

struct Node* root;

void add_lexnode(char * Name,NODE_TYPE Type);
//注意%locations应该要提前在.y文件注明https://stackoverflow.com/questions/43419585/how-to-resolve-a-yylloc-undeclared-error
    int yycolumn = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;
%}
%option yylineno
letter_ [A-Za-z_]
digit [0-9]
digits [0-9]+
OCT 0[0-7]+
OCTERROR 0[0-7]*[8-9]+[0-9]*
HEX 0[xX][0-9a-fA-F]+
HEXERROR 0[xX][0-9a-fA-F]*[g-zG-Z]+[0-9a-zA-Z]*
DEC   0|([1-9][0-9]*)
DECERROR [0-9]+[a-wA-Wy-zY-Z]+[0-9a-dA-Df-zF-Z]*|[0-9]+[0-9]+[a-dA-Df-zF-Z]+[0-9]*[0-9a-dA-Df-zF-Z]*
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
                emptyflag=1;
               return INT;}
{HEXERROR} {
    printf("Error type A at Line %d: Illegal hexadecimal number\'%s\'.\n",yylineno, yytext);
    lexError+=1;
        add_lexnode("INT",LEX_INT);
        yylval.node->int_contant=0;//暂时捕获成0,反正是错的
                emptyflag=1;

        return INT;
}
{OCT} {printfdebug("OCT");
                add_lexnode("INT",LEX_INT);
                sscanf(yytext,"%o",&yylval.node->int_contant);
                //printf("%d,%o\n",yylval.node->int_contant,yylval.node->int_contant);
                emptyflag=1;
                return INT;}
{OCTERROR}    {
    printf("Error type A at Line %d: Illegal octal number\'%s\'.\n",yylineno, yytext);
        lexError+=1;
        add_lexnode("INT",LEX_INT);
        yylval.node->int_contant=0;//暂时捕获成0,反正是错的
                emptyflag=1;
      return INT;
    }
{DEC} {printfdebug("DEC");
               add_lexnode("INT",LEX_INT);
               sscanf(yytext,"%d",&yylval.node->int_contant);
                emptyflag=1;
               return INT;}
{DECERROR} {
    printf("Error type A at Line %d: Illegal number\'%s\'.\n",yylineno, yytext);
    lexError+=1;
    add_lexnode("INT",LEX_INT);
    yylval.node->int_contant=0;
                emptyflag=1;
    return INT;
}
{FLOAT} {printfdebug("FLOAT");
                    add_lexnode("FLOAT",LEX_FLOAT);
                    sscanf(yytext,"%f",&yylval.node->float_contant);
                   //    printf("%f,%s\n",yylval.node->float_contant,yytext);
                emptyflag=1;
                    return FLOAT;}
{FLOATERROR}    {
     printf("Error type A at Line %d: Illegal floating point number\'%s\'.\n",yylineno, yytext);
     add_lexnode("FLOAT",LEX_FLOAT);
     yylval.node->float_contant=0;
       lexError+=1;
                emptyflag=1;
     return FLOAT;
    //printf("To be done !FLOATERROR\n");
    }
{LCOMMENT} { //printf("LCOMMENT\n");
//printf("%d",yylloc.first_line);
// if(yylloc.first_line==1){
//     //printf("here");
//     yylloc.first_line=yylineno;
// }
                            char a=input();char b=input();while(a!='*'||b!='/'){/*printf("%c,%c\n",a,b);*/a=b;b=input();
                           if(b=='\0'){ 
                               printf("Error type A at Line %d: Missing */.\n",yylineno);
                               break;}
                            }
                       // printf("%d",yylloc.first_line);    
                            }
{COMMENT} {char a=input();while(a!='\n')a=input();

                            //printf("COMMENT\n");

}
";" {printfdebug("SEMI");
        add_lexnode("SEMI",OTHERS);    
                emptyflag=1;
        return SEMI;}
"," {printfdebug("comma");
       add_lexnode("COMMA",OTHERS);
                emptyflag=1;
       return COMMA; }
"=" {printfdebug("assignop");
        add_lexnode("ASSIGNOP",OTHERS);
                emptyflag=1;
         return ASSIGNOP;}
{RELOP} {printfdebug("RELOP");
        add_lexnode("RELOP",OTHERS);
                emptyflag=1;
         return RELOP;}
"+" {printfdebug("plus");
        add_lexnode("PLUS",OTHERS);
                emptyflag=1;
        return PLUS;}
"-" {printfdebug("minus");
        add_lexnode("MINUS",OTHERS);
                emptyflag=1;
        return MINUS;}
"*" {printfdebug("star");
        add_lexnode("STAR",OTHERS);
                emptyflag=1;
        return STAR;}
"/" {printfdebug("div");
        add_lexnode("DIV",OTHERS);
                emptyflag=1;
        return DIV;}
"&&"  {printfdebug("and");
        add_lexnode("AND",OTHERS);
                emptyflag=1;
        return AND;}
"||"  {printfdebug("or");
        add_lexnode("OR",OTHERS);
                emptyflag=1;
        return OR;}
"." {printfdebug("dot");
        add_lexnode("DOT",OTHERS);
                emptyflag=1;
        return DOT;}
"!" {printfdebug("not");
        add_lexnode("NOT",OTHERS);
                emptyflag=1;
        return NOT;}
{TYPE} {printfdebug("type");
                emptyflag=1;
        add_lexnode("TYPE",LEX_TYPE);
        strcpy(yylval.node->string_contant,yytext);
        //printf("%s\n",yylval.node->string_contant);
        return TYPE;}
{LP}  {printfdebug("LP");
                emptyflag=1;
        add_lexnode("LP",OTHERS);
        return LP;}
{RP} {printfdebug("RP");
                emptyflag=1;
        add_lexnode("RP",OTHERS);
        return RP;}
{LB} {printfdebug("LB");
                emptyflag=1;
        add_lexnode("LB",OTHERS);
        return LB;}
{RB} {printfdebug("RB");
                emptyflag=1;
        add_lexnode("RB",OTHERS);
        return RB;}
{LC} {printfdebug("LC");
                emptyflag=1;
        add_lexnode("LC",OTHERS);
        return LC;}
{RC} {printfdebug("RC");
                emptyflag=1;
        add_lexnode("RC",OTHERS);
        return RC;}
"struct" {printfdebug("STRUCT");
                emptyflag=1;
        add_lexnode("STRUCT",OTHERS);
        return STRUCT;}
"return"  {printfdebug("return");
                emptyflag=1;
        add_lexnode("RETURN",OTHERS);
        return RETURN;}
"if"  {printfdebug("if");
                emptyflag=1;
        add_lexnode("IF",OTHERS);
        return IF;}
"else"  {printfdebug("else");
                emptyflag=1;
        add_lexnode("ELSE",OTHERS);
        return ELSE;}
"while" {printfdebug("while");
                emptyflag=1;
        add_lexnode("WHILE",OTHERS);
        return WHILE;}
{ID} {printfdebug("ID");
                emptyflag=1;
        add_lexnode("ID",LEX_ID);
        strcpy(yylval.node->string_contant,yytext);
    //    printf("%s\n",yylval.node->string_contant);
        return ID;}
"\n" {yycolumn=1;}
{WhiteSpace}+ {   /*Let it go!*/ ;      }
.   {lexError+=1;printf("Error type A at Line %d: Mysterious characters \'%s\'.\n",yylineno, yytext);}
%%
int yywrap()
{ 
    return 1;
}
void add_lexnode(char * Name,NODE_TYPE Type){
//    printf("In add lexnode!!\n");
    yylval.node=(struct Node*)malloc(sizeof(struct Node));
    yylval.node->child=NULL;
    yylval.node->next_sib=NULL;
    yylval.node->column=yylineno;
    strcpy(yylval.node->name,Name);
    yylval.node->place=0;
    yylval.node->type=Type;
}



