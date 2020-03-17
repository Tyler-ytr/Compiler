/*%{
#include <stdio.h>
//#include "lex.yy.c"
void yyerror(const char* msg) {}
%}

%token T_NUM

%left '+' '-'
%left '*' '/'

%%

S   :   S E '\n'        { printf("ans = %d\n", $2); }
    |   /* empty */    //{ /* empty */ }
/*    ;

E   :   E '+' E         { $$ = $1 + $3; }
    |   E '-' E         { $$ = $1 - $3; }
    |   E '*' E         { $$ = $1 * $3; }
    |   E '/' E         { $$ = $1 / $3; }
    |   T_NUM           { $$ = $1; }
    |   '(' E ')'       { $$ = $2; }
    ;

%%
*/
%{
#include <stdio.h>
#include "lex.yy.c"
#include <stdarg.h>
//#define YYSTYPE struct Node*
/*Declarations*/
%}
/*declared types*/
%union {
  struct Node* node;
}
/*declared tokens(terminal symbol) from 7.1.1*/

%token  <node>INT
%token  <node>FLOAT
%token  <node>ID
%token  <node>SEMI
%token  <node>COMMA
%token  <node>ASSIGNOP
%token  <node>RELOP
%token  <node>PLUS
%token  <node>MINUS
%token  <node>STAR
%token  <node>DIV
%token  <node>AND
%token  <node>OR
%token  <node>DOT
%token  <node>NOT
%token  <node>TYPE
%token  <node>LP
%token  <node>RP
%token  <node>LB
%token  <node>RB
%token  <node>LC
%token  <node>RC
%token  <node>STRUCT
%token  <node>RETURN 
%token  <node>IF
%token  <node>ELSE
%token  <node>WHILE
/*declared  nonterminal symbol  from other 7*/
/*
//7.1.2
%type <node>Program

%type <node>ExtDecList
%type <node>ExtDef
%type <node>ExtDefList
//7.1.3
%type <node>Specifier
%type  <node>StructSpecifier
%type <node>OptTag
%type  <node>Tag
//7.1.4
%type <node> VarDec
%type  <node>FunDec
%type  <node>VarList
%type <node>ParamDec
//7.1.5
%type <node> CompSt
%type  <node>StmtList
%type  <node>Stmt
//7.1.6
%type <node>DefList
%type <node>Def
%type <node>DecList
%type <node>Dec
//7.1.7
%type <node>Exp
%type <node>Args
*/
%start Program
%right ASSIGNOP
%left OR
%left AND 
%left  RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left  DOT 
%left LB RB 
%left LP RP
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%%
//7.1.2 
/*初始语法单元Program*/
Program: ExtDefList  ;
ExtDefList: ExtDef ExtDefList;
                     |  /*Empty*/ ;
ExtDef:Specifier ExtDecList SEMI;
             | Specifier SEMI;
             | Specifier FunDec CompSt;
ExtDecList: VarDec;
                      | VarDec COMMA ExtDecList;
//7.1.3
Specifier: TYPE;
                   | StructSpecifier;
StructSpecifier: STRUCT OptTag LC DefList RC;
                            | STRUCT Tag;
OptTag:ID;
                | /*Empty*/;
Tag:ID;
//7.1.4
VarDec:ID;
              | VarDec LB INT RB;
FunDec:ID LP VarList RP;
                |  ID LP RP;
VarList:ParamDec COMMA VarList;
              | ParamDec;
ParamDec:  Specifier VarDec;
//7.1.5 有移入规约冲突
CompSt:LC DefList StmtList RC;
StmtList:Stmt StmtList;
                  | /*Empty*/;
Stmt:Exp SEMI;
          |CompSt; 
          |RETURN Exp SEMI;
          |IF LP Exp RP Stmt %prec LOWER_THAN_ELSE;
          |IF LP Exp RP Stmt ELSE Stmt;
          |WHILE LP Exp RP Stmt;
//7.1.6
DefList:Def DefList;
               |/*Empty*/;
Def:Specifier DecList SEMI;
DecList:Dec;
                | Dec COMMA DecList;
Dec:VarDec;
        |VarDec ASSIGNOP Exp;
//7.1.7
Exp:Exp ASSIGNOP Exp;
        | Exp AND Exp;
        | Exp OR Exp;
        | Exp RELOP Exp;
        | Exp PLUS Exp;
        |Exp MINUS Exp;
        |Exp STAR Exp;
        |Exp DIV Exp;
        |LP Exp RP;
        |MINUS Exp;
        |NOT Exp;
        |ID LP Args RP;
        |ID LP RP;
        |Exp LB Exp RB;
        |Exp DOT ID;
        |ID;
        |INT ;
        |FLOAT;
Args:Exp COMMA Args;
          | Exp;
%%
struct Node * add_bisonnode(char *Name,int num,...){
//printf("in add bison node!\n");
//va list 参考了 https://www.runoob.com/cprogramming/c-macro-va_arg.html


;
}
