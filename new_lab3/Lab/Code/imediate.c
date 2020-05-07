#include "imediate.h"

int success_g=1;//是否成功

//双向链表;链表头,链表尾;需要的接口是再链表尾部增加节点;
//本质上生成中间代码的过程就是维护这个双向链表的过程;

struct Intercodes*inter_head=NULL;
struct Intercodes*inter_tail=NULL;
int label_cnt=0;
int var_cnt=0;//v1,v2,...
int temp_cnt=0;//t1,t2,...
const int MAX_DEPTH=2147483646;

//////////////
/////////////
//辅助函数
extern struct Node* getchild(struct Node* cur,int depth);
int strcheck(const char *a,const char* b){
	return (strcmp(a,b)==0);
}
void add_inter(struct Intercodes* now){
	if(inter_tail==NULL){
		inter_head=now;
		inter_tail=now;
	}else{
		inter_tail->next=now;
		now->prev=inter_tail;
		inter_tail=now;
	}
}
Operand new_op(int kind,int ifaddress,...){
	va_list args;
	va_start(	args,ifaddress);
	Operand op=(Operand)(malloc(sizeof(struct Operand_)));
	op->kind=kind;
	op->ifaddress=ifaddress;
	switch(kind){
		case OP_VARIABLE:{
			//char
			op->u.varname=va_arg(args,char*);
			op->u.no=var_cnt++;
			break;
		}
		case OP_FUNCTION:{
			op->u.funcname=va_arg(args,char*);
			break;
		}
		case OP_CONSTANT:{
			op->u.value=va_arg(args,int);
			break;
		}
		case OP_LABEL:{
			op->u.no=label_cnt++;
			break;
		}
		case OP_TEMPVAR:{
			op->u.no=temp_cnt++;
			break;
		}
		default:
			printf("New op to be done\n");
			assert(0);
			break;
	}

}
char *Reverse_relop(char*relop){
	//进行符号的反转比如> 变成<= 用于iffalse的翻译;
	//RELOP -> > | < | >= | <= | == | !=
	char*result=NULL;
	if(strcheck(relop,">")){
		result="<=";
	}else if(strcheck(relop,"<")){
		result=">=";
	}else if(strcheck(relop,">=")){
		result="<";
	}else if(strcheck(relop,"<=")){
		result=">";
	}else if(strcheck(relop,"==")){
		result="!=";
	}else if(strcheck(relop,"!=")){
		result="==";
	}
	return result;
}
int arithmetic_kind(char*cur){//将char类型的四则运算转换成Intercode里面的kind;
	//int result=0;
	if(strcheck(cur,"PLUS")){
		return IN_ADD;
	}else if(strcheck(cur,"MINUS")){
		return IN_SUB;
	}else if(strcheck(cur,"STAR")){
		return IN_MUL;
	}else if(strcheck(cur,"DIV")){
		return IN_DIV;
	}
	printf("arithmetic_kind error\n");
	assert(0);
}
// Operand new_temp(){
// 	//新建一个variable的OP,名字是t#
// 	Operand temp=(Operand)(malloc(sizeof(struct Operand_)));
// 	char *tempname=(char*)malloc(sizeof(char)*64);
// //	sprintf(tempname,"t%d",temp_cnt);
	
// 	temp->kind=OP_VARIABLE;
// 	//temp->u.name=tempname;
// 	temp->u.no=temp_cnt;
// 	temp_cnt+=1;
// 	temp->u.value=0;//初始化为0;
	
	
// 	;
// }
// Operand new_label(){
// 	//To be done
// 	;
// }


//IN_FUNCTION,IN_PARM
void new_intercode(int kind,...){
	va_list args;
	va_start(args,kind);
	struct Intercodes *tempcode=(struct Intercodes*)(malloc(sizeof(struct Intercodes)));
	tempcode->code.kind=kind;
	
	//根据kind来决定是几目操作符;
	switch (kind){
		//单目
		case IN_FUNCTION:
		case IN_PARAM:
		case IN_RETURN:
		case IN_LABEL:
		case IN_GOTO:
			tempcode->code.u.one.op0=va_arg(args, Operand);
			break;
		//双目
		case IN_ASSIGN:
		case IN_DEC:
			tempcode->code.u.two.left=va_arg(args, Operand);
			tempcode->code.u.two.right=va_arg(args, Operand);

			break;


		//三目 result=op1+op2
		case IN_ADD:
		case IN_SUB:
		case IN_MUL:
		case IN_DIV:
			tempcode->code.u.three.result=va_arg(args,Operand);
			tempcode->code.u.three.op1=va_arg(args,Operand);
			tempcode->code.u.three.op2=va_arg(args,Operand);
		//四目
		case IN_IFGOTO://  (if)op1,relop,op2,(goto)op3
			tempcode->code.u.four.op1=va_arg(args,Operand);
			tempcode->code.u.four.relop=va_arg(args,char*);
			tempcode->code.u.four.op2=va_arg(args,Operand);
			tempcode->code.u.four.op3=va_arg(args,Operand);
			break;
		default:
			printf("new_intercode to be done!\n");
			assert(0);
		break;

	}
	add_inter(tempcode);

}
// void oneop_intercode(int kind,Operand op){
// 	//插入中间代码并且只有一个OP

// 	struct Intercodes *tempcode=(struct Intercodes*)(malloc(sizeof(struct Intercodes)));
// 	tempcode->code.kind=kind;
// 	tempcode->code.u.one.op0=op;
// 	tempcode->next=NULL;
// 	tempcode->prev=NULL;
// 	add_inter(tempcode);
// }




int getarraysize(struct Symbol_node* cur){
	//根据传入的数组元素得到size的大小;
	//printf("In getarraysize\n");
	int result=1;
	if(cur->field.type->kind!=ARRAY){
		assert(0);
	}
	Type type=cur->field.type;
	//a[3][4][5]  3->4->5->int
	//printf("size1:%d\n",type->u.array_.size);
	while(type!=NULL){
		if(type->kind!=ARRAY)break;
		result*=type->u.array_.size;
		//printf("size:%d\n",type->u.array_.size);
		type=type->u.array_.elem;
	}
	result*=4;
	//printf("result:%d\n",result);

	return result;
}





////////////////
////////////////
//正式分析
int intermediate_generate(struct Node*cur)
{
	success_g=1;
	var_cnt=0;//v1,v2,....
	temp_cnt=0;//t1,t2...
	label_cnt=0;//label1,label2,...
	inter_head=NULL;
	inter_tail=NULL;
	Program_g(cur);

}
int Program_g(struct Node* cur){
	
	
	struct Node*Extdeflistnode=getchild(cur,0);
	int result=ExtDefList_g(Extdeflistnode);
//	printf("result:%d\n",result)
;
	return result;

}
int ExtDefList_g(struct Node* cur){
	struct Node* ExtDefnode=getchild(cur,0);
	int success=0;
	if(ExtDefnode!=NULL){
		int result1=ExtDef_g(ExtDefnode);
		success=result1?1:0;
		struct Node* ExtDefListnode=getchild(cur,1);
		if(ExtDefListnode!=NULL){
			int result2=ExtDefList_g(ExtDefListnode);
			success&=result2?1:0;
		}
	}
	//struct Node* ExtDefListnode=getchild(cur,1);
	return success;
}

int ExtDef_g(struct Node* cur){
/*
ExtDef -> Specifier ExtDecList SEMI
| Specifier SEMI
| Specifier FunDec CompSt
|Specifier FunDec SEMI//新增;用于2.1
*/
//不需要分析specifier;
	int success=1;
	struct Node*tempnode1=getchild(cur,1);
	if(strcheck(tempnode1->name,"FunDec")){
		struct Node*tempnode2=getchild(cur,2);
		if(strcheck(tempnode2->name,"CompSt")){
			
			int result1=FunDec_g(tempnode1);
			int result2=CompSt_g(tempnode2);
			success=result1&result2;
			
		}
	}

	return success;
}

int FunDec_g(struct Node* cur){
/*	FunDec -> ID LP VarList RP
| ID LP RP
*/
//FUNCTION
//PRAM
	
	
	struct Node*idnode=getchild(cur,0);
	Operand funop=new_op(OP_FUNCTION,OP_VAR,idnode->string_contant);
	// funop->u.funcname=idnode->string_contant;
	// funop->kind=OP_FUNCTION;
	new_intercode(IN_FUNCTION,funop);
	//oneop_intercode(IN_FUNCTION,funop);

	int query_ok=0;
	struct Symbol_node* fun_symbol=query_symbol2(funop->u.funcname,&query_ok);
	if(query_ok==0){
		printf("Fun query gg!\n");
		assert(0);
	}

	int paramnum=fun_symbol->field.type->u.function.paramnums;
	if(paramnum!=0){
		//遍历函数的params
		FieldList params=fun_symbol->field.type->u.function.params;
		while(params!=NULL){
			// Operand paramop=(Operand)(malloc(sizeof(struct Operand_)));
			// paramop->kind=OP_VARIABLE;
			// paramop->u.varname=params->name;
			Operand paramop=new_op(OP_VARIABLE,OP_VAR,params->name);
			new_intercode(IN_PARAM,paramop);
			// oneop_intercode(IN_PARAM,paramop);
			params=params->tail;
		}
	}
	
	return 1;
}
int CompSt_g(struct Node* cur){
	/*CompSt -> LC DefList StmtList RC
		DefList -> Def DefList
		| 空
	*/
	/*StmtList -> Stmt StmtList
	| 空*/
	struct Node* tempnode=getchild(cur,1);
	if(strcheck(tempnode->name,"DefList")){
		DefList_g(tempnode);
		struct Node* stmtlistnode=getchild(cur,2);
		if(strcheck(stmtlistnode->name,"StmtList")){
			StmtList_g(stmtlistnode);
		}
	}
	else if(strcheck(tempnode->name,"StmtList")){
			StmtList_g(tempnode);
	}
	return 1;
}
int DefList_g(struct Node*cur){
	/*	DefList -> Def DefList
	| 空 注意为空的时候使def ---> 空而不是 Deflist-->空
	Def -> Specifier DecList SEMI*/
	if(cur==NULL)return 1;
	struct Node*tempnode=getchild(cur,0);
	if(tempnode!=NULL){
		struct Node*defnode=tempnode;
		struct Node*deflistnode=getchild(cur,1);
		Def_g(defnode);
		if(deflistnode!=NULL){
			DefList_g(deflistnode);
		}
	}


	return 1;
}
int StmtList_g(struct Node*cur){

	/*StmtList -> Stmt StmtList
	| 空*/
	if(cur==NULL){
		return 1;
	}
	struct Node*Stmtnode=getchild(cur,0);
	if(Stmtnode!=NULL){
		if(strcheck(Stmtnode->name,"Stmt")){
			Stmt_g(Stmtnode);
			struct Node*tempnode=getchild(cur,1);
			if(tempnode!=NULL){
				StmtList_g(tempnode);
			}
		}
	}

	return 1;
}

int Stmt_g(struct Node* cur){
	/*
	Stmt -> Exp SEMI
	| CompSt
	| RETURN Exp SEMI
	| IF LP Exp RP Stmt
	| IF LP Exp RP Stmt ELSE Stmt
	| WHILE LP Exp RP Stmt
	*/
	struct Node*tempnode1=getchild(cur,0);
	if(strcheck(tempnode1->name,"CompSt")){
		CompSt_g(tempnode1);
	}else if(strcheck(tempnode1->name,"Exp")){
		Exp_g(tempnode1);
	}else if (strcheck(tempnode1->name,"RETURN")){
		struct Node*expnode=getchild(cur,1);
		Operand expop=Exp_g(expnode);
		new_intercode(IN_RETURN,expop);
	}else if(strcheck(tempnode1->name,"WHILE")){
		//感觉可以优化;
		//label4:
		//if cond goto label5(true label)
		//goto label6 (false label)
		//label5:...
		//goto label4
		//label 6:...
		//等价于 
		//label4:
		//if !cond goto label6(false label)
		// ....(true contant)
		//goto label4
		//label 6:
		//...
		//参考课本6.6.5 避免冗余的goto指令部分;
		Operand label1=new_op(OP_LABEL,OP_VAR);//while上面地label用来指示循环地;
		Operand label2=new_op(OP_LABEL,OP_VAR);//false label
		//Operand label3=new_op(OP_LABEL,OP_VAR);
		new_intercode(IN_LABEL,label1);
		struct Node* expnode=getchild(cur,2);
		Cond_g(expnode,NULL,label2);
		//"类似地,if-else 和while 语句地规则也将B.true设置为fall";

	}
}


int Def_g(struct Node*cur){
	/*	Def -> Specifier DecList SEMI*/
	/*
	DecList -> Dec
	| Dec COMMA DecList*/
	struct Node* Declistnode=getchild(cur,1);
	DecList_g(Declistnode);

	return 1;
}

int DecList_g(struct Node*cur){
	/*
	DecList -> Dec
	| Dec COMMA DecList
	Dec -> VarDec
	| VarDec ASSIGNOP Exp*/	
	struct Node*decnode=getchild(cur,0);
	Dec_g(decnode);
	struct Node*tempnode=getchild(cur,1);
	if(tempnode!=NULL){
		struct Node*declistnode=getchild(cur,2);
		if(declistnode!=NULL){
			DecList_g(declistnode);
		}
	}

	return 1;
}

int Dec_g(struct Node*cur){
	/*Dec -> VarDec
	| VarDec ASSIGNOP Exp*/
	//仿照p82 Exp1 ASSIGNOP Exp2
	struct Node*vardecnode=getchild(cur,0);
	struct Node*tempnode1=getchild(cur,1);
	if(tempnode1==NULL){
		VarDec_g(vardecnode);
	}else{
	//	Operand t1=new_temp();
		Operand op1=VarDec_g(vardecnode);
		Operand op2=Exp_g(tempnode1);

	}

	
	return 1;
}

Operand VarDec_g(struct Node*cur){
	/*	VarDec -> ID
	| VarDec LB INT RB
	*/
//不递归,因此只存在ID-->单个变量; VarDec-->数组;
	Operand result=NULL;
	if(cur==NULL){
		assert(0);
		return NULL;
	}
	struct Node*tempnode=getchild(cur,0);
	if(strcheck(tempnode->name,"ID")){
		//单个变量,不是数组;
		result=new_op(OP_VARIABLE,OP_VAR,tempnode->string_contant);
	}else{
		
		struct Node*findnode=getchild(tempnode,0);
		//遍历直到达到ID;
		while(strcmp(findnode->name,"ID")){
			findnode=findnode->child;
		}
		//现在findnode代表的是ID;
		//搜索ID的name,找到数组symbol确定size;
		int query_success=0;
		struct Symbol_node* queryid=query_symbol2(findnode->string_contant,&query_success);
		if(query_success==0){
			assert(0);
		}
		result=new_op(OP_VARIABLE,OP_VAR,findnode->string_contant);
		
		int arraysize=getarraysize(queryid);
		Operand op2=new_op(OP_CONSTANT,OP_VAR,arraysize);
		new_intercode(IN_DEC,result,op2);



		
	}

	return result;
}

Operand Exp_g(struct Node*cur){
		/*Exp -> Exp ASSIGNOP Exp3 ok
	| Exp AND Exp3 ok 
	| Exp OR Exp3 ok
	| Exp RELOP Exp3 ok
 	| Exp PLUS Exp3 ok
	| Exp MINUS Exp3 ok
	| Exp STAR Exp3 ok
	| Exp DIV Exp3 ok

	| LP Exp RP3  ok
	| MINUS Exp 2 ok
	| NOT Exp 2 ok

	| ID LP Args RP 4函数
	| ID LP RP 3

	| Exp LB Exp RB4 数组
	| Exp DOT ID3 结构体;

	| ID1 ok
	| INT1 ok
	| FLOAT1 ok
	*/
	Operand temp=NULL;
	return temp;
}

//这里的cur应该是exp;
int Cond_g(struct Node* cur,Operand label_true,Operand label_false){
	//参考编译原理课本6.6.5 p261 避免冗余的goto指令部分,进行优化;
	//我觉得原始的思想如我在while中写的部分
	///////////
	//感觉可以优化;
	//label4:
	//if cond goto label5(true label)
	//goto label6 (false label)
	//label5:...
	//goto label4
	//label 6:...
	//等价于 
	//label4:
	//if !cond goto label6(false label)
	// ....(true contant)
	//goto label4
	//label 6:
	//...	
	//////////
	/*Exp -> Exp ASSIGNOP Exp3 ok
	| Exp AND Exp3 ok 
	| Exp OR Exp3 ok
	| Exp RELOP Exp3 ok
 	| Exp PLUS Exp3 ok
	| Exp MINUS Exp3 ok
	| Exp STAR Exp3 ok
	| Exp DIV Exp3 ok

	| LP Exp RP3  ok
	| MINUS Exp 2 ok
	| NOT Exp 2 ok

	| ID LP Args RP 4函数
	| ID LP RP 3

	| Exp LB Exp RB4 数组
	| Exp DOT ID3 结构体;

	| ID1 ok
	| INT1 ok
	| FLOAT1 ok
	*/
	//////////
	//NULL 表示课本中的fall
	//#0:
	Operand zero=new_op(OP_CONSTANT,OP_VAR,0);

	if(cur==NULL){;}
	else{
		struct Node*tempnode1=getchild(cur,0);
		if(strcheck(tempnode1->name,"Exp")){
			;//To be done
			struct Node* tempnode2=getchild(cur,1);
			if(strcheck(tempnode2->name,"ASSIGNOP")){
				//IF(A=B)..
				//实际上是赋值玩意儿;不是0都去true否则去false;
				Operand op1=Exp_g(tempnode1);
				struct Node*tempnode3=getchild(cur,2);
				Operand op2=Exp_g(tempnode3);
				new_intercode(IN_ASSIGN,op1,op2);//op1=op2
				if(label_true!=NULL){
					new_intercode(IN_IFGOTO,op1,"!=",zero,label_true);//if(a!=0){...}
				}
				if(label_false!=NULL){
					new_intercode(IN_IFGOTO,op1,"==",zero,label_false);//if(a!=0){...}else{...} 因此如果a==0那就去else;
				}

			}else if(strcheck(tempnode2->name,"AND")){;
				Cond_g(tempnode1,NULL,label_false);
				struct Node*tempnode3=getchild(cur,2);
				Cond_g(tempnode3,label_true,label_false);		
				//可能需要修改;需要测试!		
			}else if(strcheck(tempnode2->name,"OR")){
				//6-40 
				Cond_g(tempnode1,label_true,NULL);
				struct Node*tempnode3=getchild(cur,2);
				Cond_g(tempnode3,label_true,label_false);
				//可能需要修改;
			}else if(strcheck(tempnode2->name,"RELOP")){
				Operand op1=Exp_g(tempnode1);
				struct Node*tempnode3=getchild(cur,2);
				Operand op2=Exp_g(tempnode3);
				if(label_true!=NULL&&label_false!=NULL){
					new_intercode(IN_IFGOTO,op1,tempnode2->string_contant,op2,label_true);
					new_intercode(IN_GOTO,label_false);
				}else if(label_true!=NULL){
					new_intercode(IN_IFGOTO,op1,tempnode2->string_contant,op2,label_true);					
				}else if(label_false!=NULL){
					new_intercode(IN_IFGOTO,op1,Reverse_relop(tempnode2->string_contant),op2,label_false);
				}

			}else if(strcheck(tempnode2->name,"PLUS")||strcheck(tempnode2->name,"DIV")||strcheck(tempnode2->name,"STAR")||strcheck(tempnode2->name,"MINUS")){
				Operand op1=Exp_g(tempnode1);
				struct Node* tempnode3=getchild(cur,2);
				Operand op2=Exp_g(tempnode3);
				int in_kind=arithmetic_kind(tempnode2->string_contant);
				Operand result=new_op(OP_TEMPVAR,OP_VAR);
				new_intercode(in_kind,result,op1,op2);

				if(label_true!=NULL){
					new_intercode(IN_IFGOTO,result,"!=",zero,label_true);
				}
				if(label_false!=NULL){
					new_intercode(IN_IFGOTO,result,"==",zero,label_false);
				}
			}else if(strcheck(tempnode2->name,"LB")){
				Operand op=Exp_g(cur);
				if(label_true!=NULL){
					new_intercode(IN_IFGOTO,op,"!=",zero,label_true);
				}
				if(label_false!=NULL){
					new_intercode(IN_IFGOTO,op,"==",zero,label_false);
				}

			}else if(strcheck(tempnode2->name,"DOT")){
				Operand op=Exp_g(cur);
				if(label_true!=NULL){
					new_intercode(IN_IFGOTO,op,"!=",zero,label_true);
				}
				if(label_false!=NULL){
					new_intercode(IN_IFGOTO,op,"==",zero,label_false);
				}
			}

		}else if(strcheck(tempnode1->name,"NOT")){
			struct Node*expnode=getchild(cur,1);
			Cond_g(expnode,label_true,label_false);
		}else if(strcheck(tempnode1->name,"MINUS")){
			
			//printf("cond Minus\n");
			//不确定可不可以优化
			//没有优化的版本:
			Operand op=Exp_g(cur);
			if(label_true!=NULL){
				new_intercode(IN_IFGOTO,op,"!=",zero,label_true);
			}
			if(label_false!=NULL){
				new_intercode(IN_IFGOTO,op,"==",zero,label_false);
			}
		}else if(strcheck(tempnode1->name,"LP")){
			;//LP Exp RP
			//printf("cond LP\n");
			struct Node*expnode=getchild(cur,1);
			Cond_g(expnode,label_true,label_false);
		}else if(strcheck(tempnode1->name,"ID")){
			;//To be done
			printf("cond ID\n");
			Operand op=Exp_g(cur);
			if(label_true!=NULL){
				new_intercode(IN_IFGOTO,op,"!=",zero,label_true);
			}
			if(label_false!=NULL){
				new_intercode(IN_IFGOTO,op,"==",zero,label_false);
			}

		}else if(strcheck(tempnode1->name,"INT")){
			;//To be done
			//printf("cond INT\n");
			//如果>0 那么就去true 否则去false
			if(label_true!=NULL&&tempnode1->int_contant){
				new_intercode(IN_GOTO,label_true);
			}
			if(label_false!=NULL&&!tempnode1->int_contant){
				new_intercode(IN_GOTO,label_false);
			}
		}else if(strcheck(tempnode1->name,"FLOAT")){
			;//Impossible;
		}
	}
	
	
	
	return 0;
}








