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
FieldList Def_struct(struct Node*cur,char*struct_name);
FieldList VarDec_s(struct Node*cur,Type type);
FieldList Dec_struct(struct Node*cur,Type type);
char* safe_strcpy(char*des,char*source);
void error_s(int type,int column,char* content,char*content2);
char* safe_strcpy(char*des,char*source);//32位以内的strcpy,char a[31]='\0'
#endif