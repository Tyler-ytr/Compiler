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
			
			printf("should be func:%s\n",tempnode1->name);
			//Specifier FunDec SEMI
			
		}
		;
	}
	// if(name!=NULL)
	// printf("%s\n\n\n\n\n",name);
	return 0;
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
				if(query_symbol_name(struct_name)==0){
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
			int tempreuslt=query_symbol(&temptype,tempname,&tempdef);
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
	if(query_symbol_name(vardec1->name)==0){
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

