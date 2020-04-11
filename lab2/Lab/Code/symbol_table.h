#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include <string.h>
#include <assert.h>


typedef struct FieldList_* FieldList;

typedef struct Type_* Type;
//typedef struct FieldList_* FieldList;
struct FieldList_{
	char name[32];//域的名字;和树节点名字长度一样;
	Type type;//域的类型;
	FieldList tail;//下一个域
};
struct Type_{
	enum { BASIC=0,ARRAY=1,STRUCTURE=2,FUNCTION=3} kind;
	union{
		//基本类型
		int basic;//0表示int,1表示float;
		//数组类型信息包括元素类型与数组大小构成
		struct{ Type elem;int size;}array_;
		//结构体类型的信息是一个链表
		struct{FieldList structure;char *name;}structure_;
		struct{
			Type returnparam;//返回的参数类型
			int paramnums;//输入的参数数量
			FieldList params;//函数的具体参数;
			}function;
}u;
};



struct Symbol_node{
	//Type type;//该符号表节点的类型;
	enum { VARIABLE=0, STRUCT_NAME=1, FUNCTION_NAME=2 } kind;
	struct Symbol_node* lnext;//在hash中同一个值的下一个;行next,可能未来会有十字链表;
	struct Symbol_node* cnext;//用来控制作用域的,主要是退出compst等的时候删除相应的元素,初始化为NULL;
	struct FieldList_ field;//用来存类型+名字;一般的tail为NULL;
	//char name[32];
	char* structsymbol_name;//sturct 里面的元素专用,防止超过32位;
	int ifdef;//是定义还是声明?定义是1,声明是0
	int depth;//深度信息,备用;
};

struct Symbol_bucket{
	struct Symbol_node* head;//指向该hash值的第一个节点;
	struct Symbol_bucket*next;//仅在用于控制作用域的局部符号表(链表)中使用

};//全局符号表头;
struct dec_func{
	char name[32];
	struct dec_func* next;
};

int insert_symbol(Type type,char* name,int ifdef,int depth);
int insert_symbol2(struct Symbol_node*p,struct Symbol_bucket* scope );//使用的时候p 的 lnext,cnext需要首先赋值NULL;
int query_symbol(Type* type,char*name,int*ifdef,int depth);//给名字查type和ifdef;如果不存在返回-1,否则返回0
int query_struct_name(char*name);//仅仅指查名字;
int query_symbol_name(char*name,int depth);//仅仅指查名字,并且depth=0;
int query_struct(Type*type,char*name);//结构体域里面使用的;
int insert_struct(Type type,char*name);
struct Symbol_bucket* init_symboltable();
int delete_symbol(Type type,char*name,int*ifdef);//删除符号;
int check_type(Type A,Type B);//结构等价判断;0表示不同,1表示相同;
unsigned int hash_name(char*name);
//需要修改,因为没有考虑要求2.2;
struct Symbol_bucket *enter_scope();//进入作用域;返回一个函数头;
struct Symbol_bucket* exit_scope();//出作用域,删除;返回尾部作用域;
void show_global_table();
void show_scope_table();
void push_function_dec(char*name);



#endif