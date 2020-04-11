#include "semantic.h"
/*

Program -> ExtDefList
ExtDefList -> ExtDef ExtDefList
| ->
ExtDef -> Specifier ExtDecList SEMI
| Specifier SEMI
| Specifier FunDec CompSt
|Specifier FunDec SEMI//新增;用于2.1
ExtDecList -> VarDec
| VarDec COMMA ExtDecList

*/


int depth_=0;
int struct_no_name_cnt=0;
struct Symbol_bucket* global_scope=NULL;
struct Symbol_node* create_symbolnode(int kind,Type type,char*name,int ifdef,int depth)
{
	struct Symbol_node *insert_node=(struct Symbol_node *)malloc(sizeof(struct Symbol_node));
	insert_node->lnext=NULL;
	insert_node->cnext=NULL;
	insert_node->kind=kind;
	insert_node->field.type=type;
	strcpy(insert_node->field.name,name);
	insert_node->depth=depth;
	insert_node->ifdef=ifdef;
	return insert_node;
;
}
struct Node* getchild(struct Node* cur,int depth){
	struct Node*temp=cur;
	temp=temp->child;
	if(depth==0){
		return temp;
	}

	for(int i=1;i<=depth;i++){
		temp=temp->next_sib;
	}
	return temp;

}

int semantic_check(struct Node*cur){
	depth_=0;
	Program_s(cur);
}
int Program_s(struct Node* cur){
	//To be done ; 一些init工作;
	// if(init_symboltable()==0){
	// 	;
	// }else{
	// 	printf("Failed to init symboltable in Program_s");
	// }
	global_scope=init_symboltable();
	//printf("In program_s\n\n");
	struct Node* ExtDefListnode=getchild(cur,0);
	ExtDefList_s(ExtDefListnode);
	show_global_table();
	return 0;
}
int ExtDefList_s(struct Node* cur){
	//printf("In ExtDefList_s\n");
	struct Node* ExtDefnode=getchild(cur,0);
	struct Node* ExtDefListnode=getchild(cur,1);
	ExtDef_s(ExtDefnode);
	if(ExtDefListnode!=NULL)
	ExtDefList_s(ExtDefListnode);
	
	return 0;
}

int ExtDef_s(struct Node*cur){//ExtDef可以得到某一个全局变量的所有信息,即Type和name;
/*
ExtDef -> Specifier ExtDecList SEMI
| Specifier SEMI
| Specifier FunDec CompSt
|Specifier FunDec SEMI//新增;用于2.1
*/
	printf("In ExtDef_s\n");
	//char* name=NULL;在ExtDecList和FuncDec里面存
	Type nodetype=NULL;
	
	struct Node* Specifiernode=getchild(cur,0);
	struct Node*	tempnode1=getchild(cur,1);
	struct Node* tempnode2=getchild(cur,2);
	if(Specifiernode!=NULL)
		nodetype=Specifier_s(Specifiernode);
	if(tempnode2==NULL){
	//| Specifier SEMI		
	;
	}else{
		if(tempnode1!=NULL&&strcmp(tempnode1->name,"ExtDecList")==0){
			
			int result=ExtDecList(tempnode1,nodetype);
			//Specifier ExtDecList SEMI
			;
		}else{
			//Specifier FunDec CompSt
			if(strcmp(tempnode1->name,"FunDec")!=0){
				printf("ExtDef_s bug!Should be FuncDec!\n");
				assert(0);
			}
			struct Node*	FunDecnode=tempnode1;
			
		//	printf("should be func:%s\n",tempnode1->name);
			if(strcmp(tempnode2->name,"SEMI")==0){
				printf("Specifier Fundec SEMI;\n");
				struct Symbol_bucket* tempscope=enter_scope();
				FunDec_s(FunDecnode,0,nodetype,tempscope);//声明;
				exit_scope();

			}else{
				printf("Specifier Fundec Compst;\n");
				struct Symbol_bucket* tempscope=enter_scope();
				FunDec_s(FunDecnode,1,nodetype,tempscope);
				struct Node*compstnode=tempnode2;
				depth_+=1;
				CompSt_s(compstnode,tempscope);
				depth_-=1;

			}
			//Specifier FunDec SEMI
			
		}
		;
	}
	// if(name!=NULL)
	// printf("%s\n\n\n\n\n",name);
	return 0;
}
int CompSt_s(struct Node*cur,struct Symbol_bucket*scope){
	printf("In Compst:%s\n\n",cur->name);
	/*CompSt -> LC DefList StmtList RC
		DefList -> Def DefList
		| 空
	*/
	/*StmtList -> Stmt StmtList
| 空*/
	struct Node* tempnode=getchild(cur,1);
//	printf("should be deflist:%s\n",tempnode->name);
	if(strcmp(tempnode->name,"DefList")==0){
		DefList_s(tempnode,scope);
		struct Node* stmtlistnode=getchild(cur,2);
	}else if(strcmp(tempnode->name,"StmtList")==0){
		;
	}
//	struct Node* stmtlistnode=getchild(cur,2);
//	DefList_s(deflistnode,scope);


	return 0;
}
int DefList_s(struct Node*cur,struct Symbol_bucket*scope){
/*	DefList -> Def DefList
| 空 注意为空的时候使def ---> 空而不是 Deflist-->空
Def -> Specifier DecList SEMI*/
	printf("In deflist:%s\n",cur->name);
	struct Node*tempnode=getchild(cur,0);
	if(tempnode!=NULL){
		struct Node*defnode=tempnode;
		printf("temp: %s\n",tempnode->name);
		struct Node*deflistnode=getchild(cur,1);

		Def_s(defnode,scope);
		if(deflistnode!=NULL)
		DefList_s(deflistnode,scope);
	}
	return 0;
}
int Def_s(struct Node*cur,struct Symbol_bucket*scope){
	printf("in def:%s\n",cur->name);
/*	Def -> Specifier DecList SEMI*/
/*
DecList -> Dec
| Dec COMMA DecList*/
	struct Node*specifiernode=getchild(cur,0);
	printf("in def1\n");
	struct Node*declistnode=getchild(cur,1);
	//printf("in def2\n");
	Type type=Specifier_s(specifiernode);
	printf("in def3\n");
	DecList_s(declistnode,scope,type);
	// printf("in def4\n");
	return 0;
}
int DecList_s(struct Node*cur,struct Symbol_bucket*scope,Type type){
	/*
	DecList -> Dec
| Dec COMMA DecList
	Dec -> VarDec
| VarDec ASSIGNOP Exp*/
	printf("In declist_s:%s\n",cur->name);
	struct Node*decnode=getchild(cur,0);
	Dec_s(decnode,scope,type);
	struct Node*tempnode=getchild(cur,1);
	if(tempnode!=NULL){
		struct Node*declistnode=getchild(cur,2);
		if(declistnode!=NULL)
		DecList_s(declistnode,scope,type);
	}

	return 0;
}
int Dec_s(struct Node*cur,struct Symbol_bucket*scope,Type type){
		/*Dec -> VarDec
| VarDec ASSIGNOP Exp*/
	printf("In dec_s:%s\n",cur->name);
	struct Node*vardecnode=getchild(cur,0);
	FieldList tempfield=VarDec_s(vardecnode,type);
	//printf("here\n");
	struct Node*tempnode=getchild(cur,1);
	if(tempnode==NULL){
		int result=query_symbol_name(tempfield->name,depth_);
		if(result==0){
			;//重复了报错;
			error_s(3,cur->column,tempfield->name,NULL);
		}else{
			//插入！
			struct Symbol_node*insert_node=create_symbolnode(VARIABLE,tempfield->type,tempfield->name,1,depth_);
			insert_symbol2(insert_node,scope);
		}
	}else{
		//| VarDec ASSIGNOP Exp*/
		struct Node*expnode=getchild(cur,2);
		//Type exp_type=Exp_s(expnode);


	}
	
	;
}
Type Exp_s(struct Node*cur){
	/*Exp -> Exp ASSIGNOP Exp3
	| Exp AND Exp3
	| Exp OR Exp3
	| Exp RELOP Exp3
	| Exp PLUS Exp3
	| Exp MINUS Exp3
	| Exp STAR Exp3
	| Exp DIV Exp3
	| LP Exp RP3
	| MINUS Exp3
	| NOT Exp2
	| ID LP Args RP4
	| ID LP RP3
	| Exp LB Exp RB4
	| Exp DOT ID3
	| ID1
	| INT1
	| FLOAT1
	*/
	Type result=(Type)(malloc(sizeof(struct Type_)));
	struct Node*tempnode1=getchild(cur,0);
	struct Node*tempnode2=getchild(cur,1);
	if(tempnode2!=NULL){
		if(strcmp(tempnode1->name,"ID")){
			;
		}else if(strcmp(tempnode1->name,"INT")){
			;
		}else if(strcmp(tempnode1->name,"FLOAT")){
			;
		}
		;
	}else{
		;
	}
	
}

void FunDec_s(struct Node*cur,const int ifdef,const Type res_type,struct Symbol_bucket* scope){
/*	FunDec -> ID LP VarList RP
| ID LP RP
	*/
	//如果之前没有定义/声明过,就插入声明,否则不插入;并且声明只插入一次
	Type query_type=NULL;
	int query_ifdef;
	struct Node*IDnode=getchild(cur,0);
	if(strcmp(IDnode->name,"ID")!=0){
		printf("FunDec_s error: should be ID but %s",IDnode->name);
		assert(0);
	}
	char*funcname=IDnode->string_contant;
	int result=query_symbol(&query_type,funcname,&query_ifdef,depth_);
	int flag=0;//0的时候都可以填表,1的时候只有定义可以填表;

	//处理VarList
	//先处理好type,然后和查到的比较,然后考虑插不插入;
	struct Node*tempnode=getchild(cur,2);
	Type functiontype=(Type)(malloc(sizeof(struct Type_)));
	FieldList params=NULL;
	if(strcmp(tempnode->name,"VarList")!=0){
		functiontype->u.function.paramnums=0;
		functiontype->u.function.params=NULL;
	}else{
		//处理VarList
		//FieldList params=NULL;
		printf("should be varlist:%s\n",tempnode->name);
		struct Node*Varlistnode=tempnode;
		depth_+=1;//VarList 是局部作用域;
		params=VarList_s(Varlistnode,scope);
		depth_-=1;
		int cnt=0;
		FieldList temp=params;
		while(temp!=NULL){
			cnt+=1;
			temp=temp->tail;
		}
	//	printf("cnt:%d\n",cnt);
		functiontype->u.function.paramnums=cnt;
		functiontype->u.function.params=params;		
	}
	functiontype->kind=FUNCTION;
	functiontype->u.function.returnparam=res_type;
	//然后进行检查;
	if(result==0){
		flag=1;//找到一次了,当且仅当定义的时候可以填表;
		if(ifdef==1){
			if(query_ifdef==1){
				error_s(4,cur->column,IDnode->name,NULL);//重复定义;
				flag=2;
			}else if(check_type(query_type,functiontype)==0){
						error_s(19,cur->column,IDnode->name,NULL);//定义和声明type不同报错;
						flag=3;	
			}else{//定义,没有重复定义,不和前面的冲突,填表!
			//	printf("herere\n\n\n\n\n");
				struct Symbol_node*insert_node=create_symbolnode(FUNCTION_NAME,functiontype,funcname,ifdef,depth_);
				insert_symbol2(insert_node,global_scope);
			}
		}else{//找到的是声明;
					if(check_type(query_type,functiontype)==0){
						error_s(19,cur->column,IDnode->name,NULL);//type不同报错;
						flag=4;
				}
		}
	}else{
		//没有找到,填;
		struct Symbol_node*insert_node=create_symbolnode(FUNCTION_NAME,functiontype,funcname,ifdef,depth_);
		insert_symbol2(insert_node,global_scope);	
		//如果是声明的话,加入到function_dec链表中;
		if(ifdef==0){
			push_function_dec(funcname);
		}

	}
	//如果是定义的话,没有重复定义就插入hash表;
	//如果是声明的话,如果之前没有声明/定义过就插入hash表;因此全局变量中,和函数同名的找到的第一个要么是定义,要么是没有定义过的声明;
	//声明的函数插入声明链表用于最后的查找;
	


	// struct Symbol_node*insert_node=create_symbolnode(FUNCTION_NAME,functiontype,funcname,ifdef,depth_);

	;
}
FieldList VarList_s(struct Node* cur,struct Symbol_bucket*scope){
	/*VarList -> ParamDec COMMA VarList
| ParamDec;*/
	//需要注册;
	printf("in VarList\n");
	struct Node*paramdecnode=getchild(cur,0);
	FieldList result=ParamDec_s(paramdecnode);
	struct Symbol_node *insert_node=create_symbolnode(VARIABLE,result->type,result->name,1,depth_);
	insert_symbol2(insert_node,scope);
	//result->tail=NULL;
	FieldList temp=result;
	struct Node*tempnode=cur;
	//这是第一个,然后把它们串起来;
	while(1){
		if(getchild(tempnode,1)==NULL){
			break;
		}
		tempnode=getchild(tempnode,2);
		struct Node*tempparam=getchild(tempnode,0);
		FieldList tempfield=ParamDec_s(tempparam);
		insert_node=create_symbolnode(VARIABLE,tempfield->type,tempfield->name,1,depth_);
		insert_symbol2(insert_node,scope);
		//往局部作用域里面插入元素;
		temp->tail=tempfield;
		temp=temp->tail;
	}
	temp->tail=NULL;
	/*test
	temp=result;
	while(temp!=NULL){
		printf("test: %d \n",temp->type->kind);
		temp=temp->tail;
	}*/


	return result;
}
FieldList ParamDec_s(struct Node*cur){
	/*ParamDec -> Specifier VarDec*/
	printf("ParamDec:");
	struct Node*Specifiernode=getchild(cur,0);
	struct Node*Vardecnode=getchild(cur,1);
	Type nodetype=Specifier_s(Specifiernode);
	FieldList result=VarDec_s(Vardecnode,nodetype);
	printf("%s\n",result->name);
	return result;

}

Type Specifier_s(struct Node*cur){
	//To be done
	/*
	Specifier -> TYPE
	| StructSpecifier
	StructSpecifier -> STRUCT OptTag LC DefList RC
	| STRUCT Tag
	OptTag -> ID
	| 空
	Tag -> ID
	*/
	//	*name=(char*)malloc(sizeof(char)*32);
	//strcpy(*name,"temp");
	printf("In Specifier_s\n");
	Type type=(Type)malloc(sizeof(struct Type_));
	struct Node*	tempnode0=getchild(cur,0);
	if(tempnode0==NULL){
		printf("Specifier bug,child 0 assert!\n\n");
		assert(0);
	}
	if(strcmp(tempnode0->name,"TYPE")==0){
		NODE_TYPE nodetype=tempnode0->type;
		type->kind=BASIC;
		if(nodetype==LEX_INT){
			type->u.basic=0;//INT
		}else{
			type->u.basic=1;//FLOAT
		}
	}else if(strcmp(tempnode0->name,"StructSpecifier")==0){
		;
		type->kind=STRUCTURE;
		struct Node*structnode=getchild(tempnode0,0);
		struct Node* tempnode1=getchild(tempnode0,1);
		if(tempnode1==NULL){
			printf("OptTag or Tag not found!\n");
			assert(0);
		}
		if(strcmp(tempnode1->name,"OptTag")==0){
		//	printf("In OptTag\n");
			struct Node* tempnode2=getchild(tempnode1,0);
			/*
				OptTag -> ID
				| 空
			*/
			if(tempnode2==NULL){
				;		//本来以为是匿名结构。。
			}
			else if(strcmp(tempnode2->name,"ID")==0){
				char*struct_name=tempnode2->string_contant;
			//	printf("struct name:%s\n",struct_name);
				if(query_symbol_name(struct_name,depth_)==0){
					error_s(16,tempnode2->column,struct_name,NULL);
					return NULL;
				}else{
					//不存在该名字的结构体,获取type之后存进struct表
					//StructSpecifier -> STRUCT OptTag LC DefList RC 现在开始处理DefList
					/*
						DefList -> Def DefList
					| 空
						Def -> Specifier DecList SEMI
						DecList -> Dec
					| Dec COMMA DecList
					Dec -> VarDec
					| VarDec ASSIGNOP Exp
					*/
				
					type->u.structure_.name=(char*)malloc(sizeof(char)*32);
					strcpy(type->u.structure_.name,struct_name);//赋值struct name;用于返回给上层;同时struct name作为hash值将填到struct hash表里面;
					struct Node* DefListnode=getchild(tempnode0,3);
					if(strcmp(DefListnode->name,"DefList")!=0){
						//空结构体;
						//printf("In Specifier,reach wrong place of DefList!\n");
						//assert(0);
						type->u.structure_.structure=NULL;//域不存在;
					}else{
						;//这里相当于DefList; 
						/*

												DefList -> Def DefList
					| 空*/
					struct Node*tempdeflistnode=DefListnode;
					FieldList result=NULL;
					FieldList tempfield=NULL;
				//	printf("Specifier should be deflist:%s\n",DefListnode->name);
					while(1){
				//				printf("gggg0\n");
						struct Node* tempdefnode=getchild(tempdeflistnode,0);
					//		printf("gggg1\n");
						if(tempdefnode==NULL){
							break;
						}else{
						//	printf("gggg1.5:%s\n",tempdefnode->name);
						}
						FieldList tempdeffield=Def_struct(tempdefnode,struct_name);
				//		printf("fieldname:%s\n",tempdeffield);
					//	printf("filename:%s\n",tempdeffield->name);
						//To be done
						if(result==NULL){
							result=tempdeffield;
							tempfield=result;
						}else{
							tempfield->tail=tempdeffield;
							tempfield=tempfield->tail;
						}
					//	printf("gggg3\n");
						tempdeflistnode=getchild(tempdeflistnode,1);
						if(tempdeflistnode==NULL){
						break;
						}
			//				printf("gggg4\n");
					}
					type->u.structure_.structure=result;
					// FieldList test=result;
					// while(test!=NULL){
					// 	printf("test:%d %s \n",test->type->kind,test->name);
					// 	test=test->tail;
					// }
					}
					
				}

				struct Symbol_node *insert_node=create_symbolnode(STRUCT_NAME,type,struct_name,1,depth_);
				insert_symbol2(insert_node,global_scope);
				//insert_symbol(type,struct_name,1,depth_);//不允许结构体和变量重名;
			}
			
			;
		}else if(strcmp(tempnode1->name,"Tag")==0){
			;		//To be done 需要检查这个结构体在不在结构体表里面;
		/*	Tag -> ID
	*/	
			struct Node* ID_node=getchild(tempnode1,0);
			//printf("In tag!!! name:%s\n",ID_node->string_contant);
			char*tempname=ID_node->string_contant;
			Type temptype=NULL;
			int tempdef;
			int tempreuslt=query_symbol(&temptype,tempname,&tempdef,depth_);
			//printf("out of query:%d\n",tempreuslt);
			if(tempreuslt!=0){
		//		printf("tempresult:1\n");
				error_s(17,ID_node->column,tempname,NULL);
				return NULL;
			}else if (temptype==NULL||temptype->kind!=STRUCTURE){
		//		printf("tempresult:2\n");
				error_s(17,ID_node->column,tempname,NULL);
				return NULL;
			}else {
				return temptype;//找到这个结构体的type那就返回;
			}

		}else if(strcmp(tempnode1->name,"LC")==0){
			;//匿名结构 To be dones
		//	printf("匿名结构\n");
					struct_no_name_cnt+=1;
					char*struct_name=(char*)malloc(32+1);
					sprintf(struct_name,"no_name %d",struct_no_name_cnt);
		//			printf("no name:%s\n",struct_name);
					type->u.structure_.name=(char*)malloc(sizeof(char)*32);
					strcpy(type->u.structure_.name,struct_name);//赋值struct name;用于返回给上层;同时struct name作为hash值将填到struct hash表里面;
					struct Node* DefListnode=getchild(tempnode0,2);
					if(strcmp(DefListnode->name,"DefList")!=0){
						//空结构体;
						//printf("In Specifier,reach wrong place of DefList!\n");
						//assert(0);
						type->u.structure_.structure=NULL;//域不存在;
					}else{
						;//这里相当于DefList; 
						/*

												DefList -> Def DefList
					| 空*/
					struct Node*tempdeflistnode=DefListnode;
					FieldList result=NULL;
					FieldList tempfield=NULL;
				//	printf("Specifier should be deflist:%s\n",DefListnode->name);
					while(1){
					//			printf("gggg0\n");
						struct Node* tempdefnode=getchild(tempdeflistnode,0);
					//		printf("gggg1\n");
						if(tempdefnode==NULL){
							break;
						}else{
						//	printf("gggg1.5:%s\n",tempdefnode->name);
						}
						FieldList tempdeffield=Def_struct(tempdefnode,struct_name);
					//	printf("fieldname:%s\n",tempdeffield);
					//	printf("filename:%s\n",tempdeffield->name);
						//To be done
						if(result==NULL){
							result=tempdeffield;
							tempfield=result;
						}else{
							tempfield->tail=tempdeffield;
							tempfield=tempfield->tail;
						}
					//	printf("gggg3\n");
						tempdeflistnode=getchild(tempdeflistnode,1);
						if(tempdeflistnode==NULL){
						break;
						}
						//	printf("gggg4\n");
					}
					type->u.structure_.structure=result;
					// FieldList test=result;
					// while(test!=NULL){
					// 	printf("test:%d %s \n",test->type->kind,test->name);
					// 	test=test->tail;
					// }
					}
					
		}
		else{	
			printf("In Specifier Neither OptTag nor Tag :%s!\n",tempnode1->name);
			assert(0);
		}

	}else{
		printf("Specifier bug,child 0 is neither TYPE nor StructSpecifier:%s\n",tempnode0->name);
		assert(0);
	}

	


	return type;

}
char* safe_strcpy(char*des,char*source){
    //32位以内,防止溢出;
    char* r=des;
    assert((des != NULL) && (source != NULL));
    int cnt=0;
    while(cnt<32&&(*r++ = *source++)!='\0'){cnt+=1;}
    des[31]='\0';
    return des;
}

FieldList Def_struct(struct Node*cur,char* struct_name){
	/*这里处理好Def;
	Def -> Specifier DecList SEMI
	DecList -> Dec
		| Dec COMMA DecList
	Dec -> VarDec// 给出FieldList里面的内容,给出数组或者ID
		| VarDec ASSIGNOP Exp//报错！
	需要给一个连好了的FieldList链表给上一层;
	*///	printf("Def_struct,struct name:%s\n",struct_name);
		struct Node* Specifier_node=getchild(cur,0);
		Type nowtype=Specifier_s(Specifier_node);
		struct Node* DecList_node=getchild(cur,1);
		if(strcmp(DecList_node->name,"DecList")!=0){
	//		printf("Def_struct bug of DecList!\n");
			assert(0);
		}
		struct Node*temp_declist=DecList_node;
		FieldList result=NULL;
		FieldList temp2=NULL;
		while(getchild(temp_declist,1)!=NULL){
			struct Node* Dec_node=getchild(temp_declist,0);
			FieldList tempdec_f=Dec_struct(Dec_node,nowtype);
			char*dec_name=(char*)malloc(1+strlen(struct_name)+strlen(tempdec_f->name));
		//	printf("Defstruct,dec name:%s\n",dec_name);
			strcpy(dec_name,tempdec_f->name);
			strcat(dec_name,struct_name);
			if(query_struct_name(dec_name)==0){
				//重名;
				error_s(15,Dec_node->column,tempdec_f->name,NULL);
			}
			else{
				insert_struct(tempdec_f->type,dec_name);
			}
			//串联field;
			if(result==NULL){
				result=tempdec_f;
				temp2=result;
			}else{
				temp2->tail=tempdec_f;
				temp2=temp2->tail;
			}

			temp_declist=getchild(temp_declist,2);

			if(temp_declist==NULL){
				printf("Def_struct bug in while\n");
				assert(0);
			}

		}
		//最后一个Dec
		struct Node* Dec_node=getchild(temp_declist,0);
		FieldList tempdec_f=Dec_struct(Dec_node,nowtype);
			char*dec_name=(char*)malloc(1+strlen(struct_name)+strlen(tempdec_f->name));
			strcpy(dec_name,tempdec_f->name);
			strcat(dec_name,struct_name);
		//		printf("Defstruct,dec name:%s\n",tempdec_f->name);
			if(query_struct_name(dec_name)==0){
				//重名;
				error_s(15,Dec_node->column,tempdec_f->name,NULL);
			}
			else{
				insert_struct(tempdec_f->type,dec_name);
			}
			//串联field;
			if(result==NULL){
				result=tempdec_f;
				temp2=result;
			}else{
				temp2->tail=tempdec_f;
				temp2=temp2->tail;
			}
			return result;

	
}
FieldList Dec_struct(struct Node*cur,Type type){
	//主要要处理 Dec -> VarDec: 给出FieldList里面的内容,给出数组或者ID
	//	| VarDec ASSIGNOP Exp//报错！
	//	printf("Dec_struct:%s\n",cur->name);
	FieldList field=NULL;
	struct Node* tempnode=getchild(cur,1);
	struct Node* VarDecnode=getchild(cur,0);
	field=VarDec_s(VarDecnode,type);
	if(tempnode!=NULL){
		error_s(15,cur->column,field->name,NULL);
		//return NULL;
	}
//	printf("Dec_struct field:%s\n",field->name);
	return field;

}
FieldList VarDec_s(struct Node*cur,Type type){
	FieldList field=(FieldList)(malloc(sizeof(struct FieldList_)));
	field->tail=NULL;
	/*	VarDec -> ID
	| VarDec LB INT RB
	*/
	//	printf("In vardec\n");
	struct Node* tempnode=getchild(cur,0);
	if(strcmp(tempnode->name,"ID")==0){
		field->type=type;
		
	//	printf("In vardec ID:%s\n",tempnode->string_contant);
		
		strcpy(field->name,tempnode->string_contant);
		//printf("In vardec ID field:%s\n",field->name);
		return field;
	}else{
		//递归;
		//首先获得名字;
	//	printf("vardec array1\n");
		while(tempnode->child!=NULL){
			tempnode=tempnode->child;
		}
	//	printf("vardec array2:%s\n",tempnode->string_contant);
		if(strcmp(tempnode->name,"ID")!=0){
	//		printf("Vardec bug!! check the while!\n");
			assert(0);
		}
		strcpy(field->name,tempnode->string_contant);
	//	printf("vardec array name:%s\n",field->name);

		tempnode=getchild(cur,0);
		//现在是数组;a[10][3][2] 访问顺序:2->3->10->ID;
		//Type head=(Type)malloc(sizeof(struct Type_));
		Type temp_type=NULL;
		struct Node*INT_node=NULL;
	//	printf("tempnode:%s\n",tempnode->name);
		while(tempnode->child!=NULL){
			Type cur_type=(Type)malloc(sizeof(struct Type_));
			INT_node=tempnode->next_sib->next_sib;
			if(strcmp(INT_node->name,"INT")!=0){
		//		printf("Vardecbug! INT error\n");
				assert(0);
			}
			cur_type->kind=ARRAY;
			cur_type->u.array_.size=INT_node->int_contant;
		//	printf("array number:%d\n",INT_node->int_contant);
			if(temp_type==NULL){//对应2;
				cur_type->u.array_.elem=type;
				temp_type=cur_type;//第二步:3->temptype(2)
			}else{
				cur_type->u.array_.elem=temp_type;
				temp_type=cur_type;
			}
		//	printf("eadfafrerer\n");
			tempnode=tempnode->child;
			if(tempnode==NULL){break;}

		}
		if(strcmp(tempnode->name,"ID")!=0){
			printf("Vardec bug!! check the while!\n");
			assert(0);
		}
		field->type=temp_type;
		//printf("ooutLLLL：：：eadfafrerer\n");
		return field;//可以优化 记得debug;



	}

}



int ExtDecList(struct Node *cur,Type type){	
	//To be done
	/*ExtDecList -> VarDec
| VarDec COMMA ExtDecList
*/
	printf("In ExtDecList\n");
	struct Node* VarDecnode=getchild(cur,0);
	FieldList vardec1=VarDec_s(VarDecnode,type);
	printf("name:%s\n",vardec1->name);
	if(query_symbol_name(vardec1->name,depth_)==0){
		error_s(3,cur->column,vardec1->name,NULL);
	}

	struct Symbol_node *insert_node=create_symbolnode(VARIABLE,type,vardec1->name,1,depth_);
	insert_symbol2(insert_node,global_scope);
//	insert_symbol(type,vardec1->name,1,depth_);

	struct Node*tempnode=getchild(cur,1);
	if(tempnode==NULL){
		return 0;
	}else{
		struct Node*ExtDecListnode=getchild(cur,2);
		if(ExtDecListnode!=NULL){
			ExtDecList(ExtDecListnode,type);
			;
		}
	}
	return 0;
}

void error_s(int type,int column,char* content,char*content2){
	printf("Error type %d at Line %d: ", type, column);
	switch (type){
		case 1:
			printf("Undefined variable \"%s\".\n",content);
			break;
		case 2:
			printf("Undefined function \"%s\".\n",content);
			break;
		case 3:
			printf("Redefined variable \"%s\".\n",content);
			break;
		case 4:
			printf("Redefined function \"%s\".\n",content);
			break;
		case 5:
			printf("Type mismatched for assigment.\n");
			break;
		case 6:
			printf("The left-hand side of an assignment must be a variable.\n");
			break;
		case 7:
			printf("Type mismatched for operands.\n");
			break;
		case 8:
			printf("Type mismatched for return.\n");
			break;
		case 9:
			printf("Function \"%s\" is not applicable for arguments \"%s\".\n",content,content2);
			break;
		case 10:
			printf("\"%s\" is not an array.\n",content);
			break;
		case 11:
			printf("\"%s\" is not a function.\n",content);
			break;
		case 12:
			printf("\"%s\" is not an integer.\n",content);
			break;
		case 13:
			printf("Illegal use of \"%s\".\n",content);
			break;
		case 14:
			printf("Non-existent field \"%s\".\n",content);
			break;
		case 15:
			printf("Redefined field \"%s\".\n",content);
			break;
		case 16:
			printf("Duplicated name \"%s\".\n",content);
			break;
		case 17:
			printf("Undefined structure \"%s\".\n",content);
			break;
		case 18:
			printf("Undefined function \"%s\".\n",content);
			break;
		case 19:
			printf("Inconsistent declaration of function \"%s\".\n",content);
			break;
		default:
			printf("OMG!!!Wrong semantic type!!!!content is :%s\n",content);
			//assert(0);
			break;
	}


}

