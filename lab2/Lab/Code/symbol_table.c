#include "symbol_table.h"
//目标设计：散列表+符号表栈

/*基本内容*/
struct Type_{
	enum {BASIC=0,ARRAY=1,STRUCTURE} kind;
	union{
		//基本类型
		int basic;
		//数组类型信息包括元素类型与数组大小构成
		struct{Type elem;int size;}array;
		//结构体类型的信息是一个链表
		FieldList structure;
}u;
};


struct FieldList_{
	char*name;//域的名字;
	Type type;//域的类型;
	FieldList tail;//下一个域
};




//备用项;
unsigned int hash_pjw(char*name){
	unsigned int val=0,i;
	for(;*name;++name){
		val=(val<<2)+*name;
		if(i=val&~0x3fff)val=(val^(i>>12))&0x3fff;
	}
	return val;
//主要逻辑:
//全局符号表: 一个散列表+栈,构成是结构体数组链表+一个表示顺序的栈,通过Imperative style形式维护;
/*
| |->a->b
| |->c
| |->d->e->f
| |
*/
//局部符号表: 一个二层链表,每次进入一个语句块,需要为这一层的语句块新建一个链表用来串联层中新定义的变量，每次离开一个语句块，则需要顺着代表该层的语句块的链表将所有本层定义变量全部删除；
/*
c
^
|
b->b1->b2
^
|
a->a1->a2
*/

}