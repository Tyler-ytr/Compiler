#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "symbol_table.h"
#include "common.h"
#include <string.h>
#include<stdlib.h>
#include <stdio.h>
int semantic_check(struct Node*cur);
int Program_s(struct Node*cur);
int ExtDefList_s(struct Node* cur);
int ExtDef_s(struct Node*cur);
Type Specifier_s(struct Node*cur,char**name);//返回该节点的Type;

#endif