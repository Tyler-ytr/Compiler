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
int depth=0;
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
	depth=0;
	Program_s(cur);
}
int Program_s(struct Node* cur){
	//To be done ; 一些init工作;
	if(init_symboltable()==0){
		;
	}else{
		printf("Failed to init symboltable in Program_s");
	}
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
		struct Node*structnode=getchild(tempnode0,0);
		struct Node* tempnode1=getchild(tempnode0,1);
		if(tempnode1==NULL){
			printf("OptTag or Tag not found!\n");
			assert(0);
		}
		if(strcmp(tempnode1->name,"OptTag")==0){
			printf("In OptTag\n");
			struct Node* tempnode2=getchild(tempnode1,0);
			/*
				OptTag -> ID
				| 空
			*/
		//To be done
			if(tempnode2==NULL){
				;		//To be done
			}
			else if(strcmp(tempnode2->name,"ID")==0){
		//To be done
				char*struct_name=(char*)malloc(sizeof(char)*32);

			}
			;
		}else if(strcmp(tempnode1->name,"Tag")==0){
			;		//To be done
		}else{
			printf("In Specifier Neither OptTag nor Tag !\n");
			assert(0);
		}

	}else{
		printf("Specifier bug,child 0 is neither TYPE nor StructSpecifier\n");
		assert(0);
	}

	


	return type;

}
int ExtDecList(struct Node *cur,Type type){	
	//To be done
	/*ExtDecList -> VarDec
| VarDec COMMA ExtDecList
*/
	struct Node* VarDecnode=getchild(cur,0);
	struct Node*tempnode=getchild(cur,1);
	if(tempnode==NULL){
		;
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
		case 2:
			printf("Undefined function \"%s\".\n",content);
		case 3:
			printf("Redefined variable \"%s\".\n",content);
		case 4:
			printf("Redefined function \"%s\".\n",content);
		case 5:
			printf("Type mismatched for assigment.\n");
		case 6:
			printf("The left-hand side of an assignment must be a variable.\n");
		case 7:
			printf("Type mismatched for operands.\n");
		case 8:
			printf("Type mismatched for return.\n");
		case 9:
			printf("Function \"%s\" is not applicable for arguments \"%s\".\n",content,content2);
		case 10:
			printf("\"%s\" is not an array.\n",content);
		case 11:
			printf("\"%s\" is not a function.\n",content);
		case 12:
			printf("\"%s\" is not an integer.\n",content);
		case 13:
			printf("Illegal use of \"%s\".\n",content);
		case 14:
			printf("Non-existent field \"%s\".\n",content);
		case 15:
			printf("Redefined field \"%s\".\n",content);
		case 16:
			printf("Duplicated name \"%s\".\n",content);
		case 17:
			printf("Undefined structure \"%s\".\n",content);
		case 18:
			printf("Undefined function \"%s\".\n",content);
		case 19:
			printf("Inconsistent declaration of function \"%s\".\n",content);
		default:
			printf("OMG!!!Wrong semantic type!!!!\n");
			assert(0);
	}
	

}

