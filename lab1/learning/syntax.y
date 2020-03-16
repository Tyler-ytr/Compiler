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
/*Declarations*/
%}
/*declared types*/
%union {
  int type_int;
  float type_float;
  double type_double;
}
/*declared tokens(terminal symbol) from 7.1.1*/
%token <type_int > INT
%token  FLOAT
%token  ID
%token  SEMI
%token  COMMA
%token  ASSIGNOP
%token  RELOP
%token  PLUS
%token  MINUS
%token  STAR
%token  DIV
%token  AND
%token  OR
%token  DOT
%token  NOT
%token  TYPE
%token  LP
%token  RP
%token  LB
%token  RB
%token  LC
%token  RC
%token  STRUCT
%token  RETURN 
%token  IF
%token  ELSE
%token  WHILE
/*declared  nonterminal symbol  from other 7*/
/*
//7.1.2
%token  Program

%token ExtDecList
%token ExtDef
%token ExtDefList
//7.1.3
%token Specifier
%token  StructSpecifier
%token OptTag
%token  Tag
//7.1.4
%token  VarDec
%token  FunDec
%token  VarList
%token ParamDec
//7.1.5
%token  CompSt
%token  StmtList
%token  Stmt
//7.1.6
%token DefList
%token Def
%token DecList
%token Dec
//7.1.7
%token Exp
%token Args
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
