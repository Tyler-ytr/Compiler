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
//每进入一个作用域,局部符号表的链表新建一个元素a到末尾,返回这个元素a到外层(给一个函数),这个就是作用域;在该作用域中,新建的元素都要纵向的和a链接;退出作用域的时候顺着a删除;对于函数int temp(int a){int b=a;};b,a应该在一个作用域里面;
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
//结构体也需要一个符号表来存;暂记

/*基本内容*/
#define SYMBOL_LEN 0x3fff
struct Symbol_bucket global_head[SYMBOL_LEN];
struct Symbol_bucket struct_head[SYMBOL_LEN];
//struct Symbol_bucket* stack_head=NULL;//每进入一个大括号的时候往链表中插一个新的节点;
struct Symbol_bucket* scope_head=NULL;//作用域控制链表;

int init_symboltable(){
	//to be done
	for(int i=0;i<SYMBOL_LEN;i++){
		global_head[i].head=NULL;
		struct_head[i].head=NULL;
	}
	scope_head=malloc(sizeof(struct Symbol_bucket));
	scope_head->next=NULL;
	scope_head->head=NULL;
	return 0;
}
struct Symbol_bucket*enter_scope(){
	struct Symbol_bucket *result=malloc(sizeof(struct Symbol_bucket));
	result->next=NULL;
	result->head=NULL;
	struct Symbol_bucket *tail=scope_head;
	while(tail->next!=NULL){
		tail=tail->next;
	}
	tail->next=result;
	return result;
}
struct Symbol_bucket* exit_scope(){
	;//To be done; 主要要做一个十字链表的插入和删除;
}

int insert_symbol(Type type,char* name,int ifdef,int depth){
	//to be done;
	int value=hash_name(name);
	printf("in insert\n");
	if(global_head[value].head==NULL){
		struct Symbol_node* temp=malloc(sizeof(struct Symbol_node));
		//temp->type=type;
		temp->field.type=type;
		temp->lnext=NULL;
		strcpy(temp->field.name,name);
		temp->depth=depth;
		temp->ifdef=ifdef;
		global_head[value].head=temp;
	}else{
		struct Symbol_node* head=global_head[value].head;
		//头插;
		struct Symbol_node* temp=malloc(sizeof(struct Symbol_node));
		temp->field.type=type;
		temp->lnext=head;
		strcpy(temp->field.name,name);
		temp->ifdef=ifdef;
		temp->depth=depth;
		global_head[value].head=temp;
	}

	return 0;
}
int insert_struct(Type type,char*name){
	int value=hash_name(name);
	printf("in insert struct\n");
	if(struct_head[value].head==NULL){
		struct Symbol_node*temp=malloc(sizeof(struct Symbol_node));
		temp->field.type=type;
		temp->lnext=NULL;
		//strcpy(temp->field.name,name);
		char*structsymbol_name=(char*)malloc(strlen(name)+1);
		strcpy(structsymbol_name,name);
		temp->structsymbol_name=structsymbol_name;
		struct_head[value].head=temp;
	}else{
		//结构体不允许重名;
		struct Symbol_node*head=struct_head[value].head;
		if(strcmp(head->field.name,name)==0){
			printf("In insert_struct redifined struct\n");
			return -1;
		}
		else{
			struct Symbol_node*temp=malloc(sizeof(struct Symbol_node));
			temp->field.type=type;
			temp->lnext=head;
			strcpy(temp->field.name,name);
			struct_head[value].head=temp;//头插;
		}
		;
	}
	return 0;
}

int query_struct(Type*type,char*name){//存在 return 0,不存在return -1
	int value=hash_name(name);
	printf("In struct query\n");
	if(struct_head[value].head==NULL){
		return -1;//不存在
	}
	else{
		struct Symbol_node*temp=struct_head[value].head;
		int flag=0;
		while(temp!=NULL){
			if(strcmp(temp->structsymbol_name,name)==0){
				*type=temp->field.type;
				flag=1;
				return 0;//找到了
			}
			temp=temp->lnext;
			if(temp==NULL){
				break;
			}
		}
		if(flag==0){
			return -1;//没有找到;
		}
		
	}

}
int query_struct_name(char*name){
	Type nulltype=(Type)malloc(sizeof(struct Type_));
	
	return query_struct(&nulltype,name);
}
int query_symbol_name(char*name){
	Type nulltype=(Type)malloc(sizeof(struct Type_));
	int nulldef;
	return query_symbol(&nulltype,name,&nulldef);
}
int query_symbol(Type* type,char*name,int*ifdef){//存在 return 0,不存在return -1
	int value=hash_name(name);
	printf("In query%s\n",name);
	if(global_head[value].head==NULL){
	//	printf("OMG!!!!!!!We don't have this symbol!!");
		return -1;//没有命名,
	}else{
		struct Symbol_node*temp=global_head[value].head;
		// printf("herer");
		// if(temp->lnext==NULL){
		// 	printf("herer");
		// }
		int flag=0;
		while(temp!=NULL){
			if(strcmp(temp->field.name,name)==0){
			//	printf("able:%d\n",temp->type->kind);
				*type=temp->field.type;
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
		//	printf("OMG2!!!!!!!We don't have this symbol!!");
			return -1;//没有找到
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
