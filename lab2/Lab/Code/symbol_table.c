#include "symbol_table.h"
//目标设计：散列表+符号表栈
//主要逻辑:
//全局符号表: 一个散列表,构成是结构体数组链表,通过Imperative style形式维护;
/*
| |->a->b
| |->c
| |->d->e->f
| |->a2
栈:
--
a->f
c->d->a2
b->e
--
每一层作用域定义的变量在栈的同一层,记录在局部符号表
*/
//局部符号表: （工具人）一个二层链表,每次进入一个语句块,需要为这一层的语句块新建一个链表用来串联层中新定义的变量，每次离开一个语句块，则需要顺着代表该层的语句块的链表将所有本层定义变量全部删除；
/*
c
^
|
b->b1->b2
^
|
a->a1->a2
*/
/*
1.完成不测作用域的符号表,完成基本结构+2.1，
2.要求2.2可能的实现: 用上述的局部符号表串表达;亦或者使用十字链表;
3.要求2.1需要求改产生式,声明和定义的冲突,拟才用一个flag表示;
4.要求2.3结构等价,可以在符号表里面实现一个check typeA,B的布尔函数解决;

*/

/*基本内容*/
#define SYMBOL_LEN 0x3fff
struct Symbol_bucket global_head[SYMBOL_LEN];

int init_symboltable(){
	//to be done
	for(int i=0;i<SYMBOL_LEN;i++){
		global_head[i].head=NULL;
	}
	return 0;
}

int insert_symbol(Type type,char* name,int ifdef,int depth){
	//to be done;
	int value=hash_name(name);
	printf("in insert");
	if(global_head[value].head==NULL){
		struct Symbol_node* temp=malloc(sizeof(struct Symbol_node));
		temp->type=type;
		temp->lnext=NULL;
		strcpy(temp->name,name);
		temp->depth=depth;
		temp->ifdef=ifdef;
		global_head[value].head=temp;
	}else{
		struct Symbol_node* head=global_head[value].head;
		//头插;
		struct Symbol_node* temp=malloc(sizeof(struct Symbol_node));
		temp->type=type;
		temp->lnext=head;
		strcpy(temp->name,name);
		temp->ifdef=ifdef;
		temp->depth=depth;
		global_head[value].head=temp;
	}

	return 0;
}
int query_symbol(Type* type,char*name,int*ifdef){
	int value=hash_name(name);
	printf("In query%s\n",name);
	if(global_head[value].head==NULL){
		printf("OMG!!!!!!!We don't have this symbol!!");
		return -1;//没有命名,报错
	}else{
		struct Symbol_node*temp=global_head[value].head;
		// printf("herer");
		// if(temp->lnext==NULL){
		// 	printf("herer");
		// }
		int flag=0;
		while(temp!=NULL){
			if(strcmp(temp->name,name)==0){
			//	printf("able:%d\n",temp->type->kind);
				*type=temp->type;
				*ifdef=temp->ifdef;
				flag=1;
				return 0;
			}
			temp=temp->lnext;
			if(temp==NULL){
				break;
			}
		}
		if(flag==0){
			printf("OMG2!!!!!!!We don't have this symbol!!");
			return -1;
		}

	}

}
int delete_symbol(Type type,char*name,int*ifdef){
	//to be done;

	return 0;
}
int check_type(Type A,Type B){


	return 0;
}


//备用项;
unsigned int hash_name(char*name){
	unsigned int val=0,i;
	for(;*name;++name){
		val=(val<<2)+*name;
		if(i=val&~0x3fff)val=(val^(i>>12))&0x3fff;//0x3fff实际上是全局符号表的长度
	}
	return val;
}
