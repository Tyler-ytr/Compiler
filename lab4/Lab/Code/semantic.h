#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "symbol_table.h"
#include "common.h"
#include <string.h>
#include<stdlib.h>
#include <stdio.h>
#include <assert.h>
int semantic_check(struct Node*cur);
int Program_s(struct Node*cur);
int ExtDefList_s(struct Node* cur);
int ExtDef_s(struct Node*cur);
Type Specifier_s(struct Node*cur);//返回该节点的Type;
int ExtDecList(struct Node *cur,Type type);
FieldList Def_struct(struct Node*cur,char*struct_name,int curoffset,int*tempoffset);
FieldList VarDec_s(struct Node*cur,Type type);
FieldList Dec_struct(struct Node*cur,Type type);
char* safe_strcpy(char*des,char*source);
void error_s(int type,int column,char* content,char*content2);
char* safe_strcpy(char*des,char*source);//32位以内的strcpy,char a[31]='\0'
void FunDec_s(struct Node*cur,const int ifdef,const Type res_type,struct Symbol_bucket*scope);//节点,是否是声明,返回值类型;
int CompSt_s(struct Node*cur,struct Symbol_bucket*scope,Type res_type);
FieldList VarList_s(struct Node*cur,struct Symbol_bucket*scope);
FieldList ParamDec_s(struct Node*cur);
int DefList_s(struct Node*cur,struct Symbol_bucket*scope);
int Def_s(struct Node*cur,struct Symbol_bucket*scope);
int DecList_s(struct Node*cur,struct Symbol_bucket*scope,Type type);
int StmtList_s(struct Node*cur,struct Symbol_bucket*scope,Type res_type);
int Stmt_s(struct Node*cur,struct Symbol_bucket*scope,Type res_type);
int Dec_s(struct Node*cur,struct Symbol_bucket*scope,Type type);
int Arg_s(struct Node* cur,FieldList params);
Type Exp_s(struct Node*cur);//检查表达式得到type交给上层;
#endif