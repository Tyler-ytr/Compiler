#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include <string.h>



typedef struct Type_* Type;
typedef struct FieldList_* FieldList;

struct Type_{
	enum { BASIC=0,ARRAY=1,STRUCTURE=2,FUNCTION=3} kind;
	union{
		//基本类型
		int basic;//0表示int,1表示float;
		//数组类型信息包括元素类型与数组大小构成
		struct{ Type elem;int size;}array;
		//结构体类型的信息是一个链表
		struct{FieldList structure;char *name;}structure;
		struct{
			Type returnparam;//返回的参数类型
			int paramnums;//输入的参数数量
			FieldList params;//函数的具体参数;
			}function;
}u;
};


struct FieldList_{
	char name[32];//域的名字;和树节点名字长度一样;
	Type type;//域的类型;
	FieldList tail;//下一个域
};
struct Symbol_node{
	Type type;//该符号表节点的类型;
	struct Symbol_node* lnext;//在hash中同一个值的下一个;行next,可能未来会有十字链表;
	
	char name[32];
	int ifdef;//是定义还是声明?定义是1,声明是0
	int n,m;//表示行号和列号,备用;
	int depth;//深度信息,备用;
};

struct Symbol_bucket{
	struct Symbol_node* head;//指向该hash值的第一个节点;
	struct Symbol_bucket*next;//仅在用于控制作用域的局部符号表(链表)中使用

};//全局符号表头;

int insert_symbol(Type type,char* name,int ifdef,int depth);
int query_symbol(Type* type,char*name,int*ifdef);//给名字查type和ifdef;如果不存在返回-1,否则返回0
int init_symboltable();
int delete_symbol(Type type,char*name,int*ifdef);//删除符号;
int check_type(Type A,Type B);//结构等价判断;0表示不同,1表示相同;
unsigned int hash_name(char*name);
//需要修改,因为没有考虑要求2.2;


#endif