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
struct dec_func*dec_func_head=NULL;//函数定义链表,最后遍历检查;
void push_function_dec(char*name,int column){
	if(dec_func_head==NULL){
		dec_func_head=(struct dec_func*)malloc(sizeof(struct dec_func));
		strcpy(dec_func_head->name,name);
		dec_func_head->column=column;
		dec_func_head->next=NULL;
	}else{
		struct dec_func*temp=dec_func_head;
		while(temp->next!=NULL){
			temp=temp->next;
		}
		temp->next=(struct dec_func*)malloc(sizeof(struct dec_func));
		strcpy(temp->next->name,name);
		temp->next->column=column;
		temp->next->next=NULL;
	}
}
void check_function_def(){
	struct dec_func*temp=dec_func_head;
	while(temp!=NULL){
		char*name=temp->name;
		int queryifdef=-1;
		Type uselesstype=(Type)(malloc(sizeof(struct Type_)));
		int result=query_symbol(&uselesstype,name,&queryifdef,0);
		if(queryifdef!=1){
			printf("Error type %d at Line %d: ", 18, temp->column);
			printf("Undefined function \"%s\".\n",temp->name);
		}

		temp=temp->next;
	}
}
struct Symbol_bucket* init_symboltable(){
	//to be done
	for(int i=0;i<SYMBOL_LEN;i++){
		global_head[i].head=NULL;
		struct_head[i].head=NULL;
	}
	scope_head=malloc(sizeof(struct Symbol_bucket));
	scope_head->next=NULL;
	scope_head->head=NULL;
	return scope_head;
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

	struct Symbol_bucket*tail=scope_head;
	struct Symbol_bucket* tailbefore=scope_head;
	while(tail->next!=NULL){
		//printf("herer\n");
		tailbefore=tail;
		tail=tail->next;
	}

	if(tail==scope_head){
		printf("Can't exit scope_head\n");
		assert(0);
	}else if(tail->head!=NULL){
			//printf("herer\n");

			struct Symbol_node* scope_tail=tail->head;//目的是这个纵向链表的尾部
			struct Symbol_node* scope_tail_before=scope_tail;//目的是这个纵向链表的尾部的前一个;
			int cnt=0;//记录链表的个数

			while(scope_tail->cnext!=NULL){
				scope_tail_before=scope_tail;
				scope_tail=scope_tail->cnext;
				cnt+=1;
			}
			struct  Symbol_node** scope_list=(struct Symbol_node**)malloc(sizeof(struct Symbol_node**)*(cnt+2));
			scope_tail=tail->head;
			scope_tail_before=scope_tail;
			cnt=0;
			scope_list[cnt]=scope_tail;//第一个
			while(scope_tail->cnext!=NULL){
				scope_list[cnt]=scope_tail;
				scope_tail_before=scope_tail;
				scope_tail=scope_tail->cnext;
				cnt+=1;
			}
			scope_list[cnt]=scope_tail;//最后一个;
			int tempcnt=cnt;
			//首先找到这个symbol在table里面的前一项,然后横向删除,然后纵向删除;
			for(;cnt>=0;cnt--){
		//		printf("cnt:%d",cnt);
				int value=hash_name(scope_list[cnt]->field.name);
				if(global_head[value].head==NULL){
					printf("drop table bug, %s not found!\n",scope_list[cnt]->field.name);
					assert(0);
				}
				struct Symbol_node*temp=global_head[value].head;//横向查找;
				int flag=0;
				if(temp==scope_list[cnt]){//global_head的头部就是要找的
					struct Symbol_node*temp_next=scope_list[cnt]->lnext;
					global_head[value].head=temp_next;
					//PR("drops:%s \n",scope_list[cnt]->field.name);
					free(scope_list[cnt]);
				}else{//global_head的头部不是要找的;b,head->a->b temp = head;temp=a;temp->lnext=b;退出;
				while(temp->lnext!=NULL){
					if(temp->lnext==scope_list[cnt]){
						flag=1;
						break;
					}
					temp=temp->lnext;
				}
				if(flag==0){
					printf("drop table bug, %s not found!\n",scope_list[cnt]->field.name);
					assert(0);
				}
				//temp----> scope_list[cnt]----> temp_next
				//scope_list[cnt-1]--->scope_list[cnt]

				PR("drops:%s \n",scope_list[cnt]->field.name);
				struct Symbol_node*temp_next=scope_list[cnt]->lnext;
				temp->lnext=temp_next;
				free(scope_list[cnt]);
				}
			}
			
		//	free(tail->head);//也就是free(scope_list[0]);

			free(scope_list);
			scope_list=NULL;
	}
	//释放scope链表的tail;
	tailbefore->next=NULL;
	free(tail);
	tail=NULL;
	return tailbefore;
}
void show_global_table(){
	printf("-----------------------global_table_below----------------------\n");
	for(int i=0;i<SYMBOL_LEN;i++){
		if(global_head[i].head!=NULL){
			printf("i:%d ",i);
			struct Symbol_node*temp=global_head[i].head;
			while(temp!=NULL){
					printf("name:%s type:%d ,",temp->field.name,temp->field.type->kind);
					temp=temp->lnext;
			}
			printf("\n");
		};
	};
	printf("-----------------------global_table_above----------------------\n");
}
void show_scope_table(){
	printf("-----------------------scope_table_below----------------------\n");
	struct Symbol_bucket*temp=scope_head;
	int cnt=0;
	while(temp!=NULL){
		printf("cnt: %d ",cnt);	
		struct Symbol_node *temp1=temp->head;
		while(temp1!=NULL){
			printf("%s ",temp1->field.name);
			temp1=temp1->cnext;
		}
		printf("\n");
		temp=temp->next;
		cnt+=1;
	}

	printf("-----------------------scope_table_above----------------------\n");
}
void show_struct_table(){
	printf("-----------------------scope_struct_below----------------------\n");
		for(int i=0;i<SYMBOL_LEN;i++){
		if(struct_head[i].head!=NULL){
			printf("i:%d ",i);
			struct Symbol_node*temp=struct_head[i].head;
			while(temp!=NULL){
					printf("name:%s type:%d ,",temp->structsymbol_name,temp->field.type->kind);
					temp=temp->lnext;
			}
			printf("\n");
		};
	};

	printf("-----------------------scope_struct_above----------------------\n");
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
int insert_symbol2(struct Symbol_node*p,struct Symbol_bucket* scope){
	
	char *symbol_name=p->field.name;
//	printf("in insert2:%s\n",symbol_name);
	if(p->cnext!=NULL||p->lnext!=NULL){
		printf("p->cnext/lnext should be NULL!\n");
		assert(0);

	}
	//struct Symbol_node* newer=()malloc
	//不确定是否需要新建一个节点赋值;
	int value=hash_name(symbol_name);
	//假设这个p从未在表里面出现(用insert之前使用者应该先query);
	//p->cnext=NULL;
	//p->lnext=NULL;
	if(scope==NULL){
		printf("insert_symbol2:Scope is NULL!\n");
		assert(0);
	}else
	{
		struct Symbol_node*scope_tail=scope->head;
		if(scope_tail==NULL){
			scope->head=p;
			//scope->head->lnext=NULL;
			
		}else{
			while(scope_tail->cnext!=NULL){
				scope_tail=scope_tail->cnext;
			}//找到当前控制域的尾节点;
			scope_tail->cnext=p;
		}
		if(global_head[value].head==NULL){
			global_head[value].head=p;
		}else{
			struct Symbol_node*head=global_head[value].head;
			p->lnext=head;
			global_head[value].head=p;
		}
	}
}

int insert_struct(Type type,char*name){
	int value=hash_name(name);
	//printf("in insert struct\n");
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
			//printf("In insert_struct redifined struct\n");
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
//	printf("In struct query\n");
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
int query_symbol_name(char*name,int depth){
	Type nulltype=(Type)malloc(sizeof(struct Type_));
	int nulldef;
	return query_symbol(&nulltype,name,&nulldef,depth);
}
int query_symbol_exist2(Type* type,char*name,int*ifdef,int depth,int*kind){//存在 return 0,不存在return -1
	int value=hash_name(name);
//	printf("In query%s\n",name);
	if(global_head[value].head==NULL){
	//	printf("OMG!!!!!!!We don't have this symbol!!");
		return -1;//没有命名,
	}else{
		struct Symbol_node*temp=global_head[value].head;
		int flag=0;
		while(temp!=NULL){
			if(strcmp(temp->field.name,name)==0&&depth>=temp->depth){//进入一个局部作用域之后depth+=1,因此当要找的depth小于depth的时候说明该层的前一层有;
			//	printf("able:%d\n",temp->type->kind);
				*type=temp->field.type;
				// if(temp->field.type==NULL){
				// 	printf("GG\n");
				// }
				*ifdef=temp->ifdef;
				*kind=temp->kind;
				flag=1;
				return 0;
			}
			temp=temp->lnext;
			if(temp==NULL){
				break;
			}
		}
		if(flag==0){
			return -1;//没有找到
		}
	}

}



int query_symbol(Type* type,char*name,int*ifdef,int depth){//存在 return 0,不存在return -1
	int value=hash_name(name);
//	printf("In query%s\n",name);
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
			if(strcmp(temp->field.name,name)==0&&depth==temp->depth){//在同一层才算被找到,否则不算;
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
int query_symbol_exist(Type* type,char*name,int*ifdef,int depth){
		int value=hash_name(name);
//	printf("In query%s\n",name);
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
			if(strcmp(temp->field.name,name)==0&&depth>=temp->depth){//进入一个局部作用域之后depth+=1,因此当要找的depth小于depth的时候说明该层的前一层有;
			//	printf("able:%d\n",temp->type->kind);
				*type=temp->field.type;
				// if(temp->field.type==NULL){
				// 	printf("GG\n");
				// }
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
	FieldList A_f=A->u.structure_.structure;
	FieldList B_f=B->u.structure_.structure;
	if(A==B){
		return 1;
	}
	else{
		if(A==NULL){
			printf("Type A is NULL\n");
			assert(0);
			return 0;
		}else if(B==NULL){
			printf("Type B is NULL\n");
			assert(0);
			return 0;
		}
		if(A->kind!=B->kind){
			return 0;
		}
		else{
			// FieldList A_f=A->u.structure_.structure;
			// FieldList B_f=B->u.structure_.structure;
			switch (A->kind){
				case BASIC:{
					//printf("BASIC: A:%d B:%d \n",A->u.basic,B->u.basic);
					return A->u.basic==B->u.basic;
					break;}
				case ARRAY:{//维度一样就行了;
					// if(A->u.array_.size!=B->u.array_.size){
					// 	return 0;
					// }
					// //否则size一样,比较type
					// int result1=check_type(A->u.array_.elem,B->u.array_.elem);
					// return result1;
					//数组弱等价: 维度一样;
					int result2=0;
					int cnt1=0;int cnt2=0;
					Type temp1=A;Type temp2=B;
					while(temp1!=NULL){
						temp1=temp1->u.array_.elem;
						cnt1+=1;
					}
					while(temp2!=NULL){
						temp2=temp2->u.array_.elem;
						cnt2+=1;
					}
				//	printf("cnt1:%d cnt2:%d \n",cnt1,cnt2);
					result2=(cnt1==cnt2);

					return result2;
					break;}
				case STRUCTURE:{
					//神必报错:a label can only be part of a statement and a declaration is not a statement,加了一个大括号就好了;
				//	PR("HERE\n");
					FieldList A_f=A->u.structure_.structure;
					FieldList B_f=B->u.structure_.structure;
					while(A_f!=NULL&&B_f!=NULL){
						A_f=A_f->tail;
						B_f=B_f->tail;
						;
					}
					if(A_f!=NULL||B_f!=NULL){
						return 0;
					}
					//保证两者等长度;
					A_f=A->u.structure_.structure;
					B_f=B->u.structure_.structure;
					int flag=0;
					while(A_f!=NULL&&B_f!=NULL){

					//	PR("A: %d,B: %d\n",A_f->type->kind,B_f->type->kind);
						if(A_f->type->kind!=B_f->type->kind){
							return 0;
						}
						if(A_f->type->kind==ARRAY){
							int array_result=check_type_array_strong(A_f->type,B_f->type);
							if(array_result==0)return 0;
							else return 1;
						}else{
							if(check_type(A_f->type,B_f->type)==0){
								return 0;
							};
						}
						A_f=A_f->tail;
						B_f=B_f->tail;
						;
					}
					return 1;
					break;
				}
				case FUNCTION:{
					//To be done;
					//printf("FUNCTION check_type: To be done\n");
					if(A->u.function.paramnums!=B->u.function.paramnums){
						return 0;//函数参数个数不一样;
					}
					if(check_type(A->u.function.returnparam,B->u.function.returnparam)==0){
						return 0;
					}
					FieldList A_f=A->u.function.params;
					FieldList B_f=B->u.function.params;
					int flag=0;
					while(A_f!=NULL&&B_f!=NULL){
						if(check_type(A_f->type,B_f->type)==0){
							return 0;
						};
						A_f=A_f->tail;
						B_f=B_f->tail;						
					}
					return 1;
					break;
				}
				default:
					printf("check_type bug,A->kind cant't find!\n");assert(0);return 0;
			}

		}
	}

	return 0;
}

int check_type_array_strong(Type A,Type B){
//	PR("Strong!\n");
	if(A->u.array_.size!=B->u.array_.size){
		return 0;
	}
	//否则size一样,比较type
	int result1;
	if(A->u.array_.elem->kind!=B->u.array_.elem->kind){
		return 0;
	}
	if(A->u.array_.elem->kind==ARRAY){
		result1=check_type_array_strong(A->u.array_.elem,B->u.array_.elem);
	}else{
	result1=check_type(A->u.array_.elem,B->u.array_.elem);}
	return result1;
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
