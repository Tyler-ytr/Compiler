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
		//双向链表插入;
		//head-->.......->now
		now->next=inter_head;
		now->prev=inter_tail;
		inter_tail->next=now;
		inter_head->prev=now;
		inter_tail=now;
}
Operand new_op(int kind,int ifaddress,...){
	va_list args;
	va_start(args,ifaddress);
	Operand op=(Operand)(malloc(sizeof(struct Operand_)));
	op->kind=kind;
	op->ifaddress=ifaddress;
	switch(kind){
		case OP_VARIABLE:{
			 op->varname=va_arg(args,char*);
			 op->no=var_cnt;
			 var_cnt++;
			break;
		}
		case OP_FUNCTION:{
			op->funcname=va_arg(args,char*);
			break;
		}
		case OP_CONSTANT:{
			op->value=va_arg(args,int);
			break;
		}
		case OP_LABEL:{
			op->no=label_cnt;
			label_cnt++;
			break;
		}
		case OP_TEMPVAR:{
			op->no=temp_cnt;
			temp_cnt++;
			break;
		}
		default:
		if(IM_DEBUG){
			printf("New op to be done\n");
			assert(0);}
			break;
	}
	va_end(args);
	return op;
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
Operand copyop(Operand op){
	Operand result=(Operand)malloc(sizeof(struct Operand_));
	result->kind=op->kind;
	result->ifaddress=op->ifaddress;
	result->varname=op->varname;
	result->no=op->no;
	result->funcname=op->funcname;
	result->value=op->value;
	result->depth=op->depth;
	
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
	if(IM_DEBUG){
	printf("arithmetic_kind error\n");
	assert(0);
	}
}
void printop(Operand op,FILE*fp){
	if(IM_DEBUG){
		//printf("printop: %d:\n",op->kind);
	}
	switch(op->kind){
		case OP_VARIABLE:{
			if(op->ifaddress==OP_ADDRESS){
				fprintf(fp,"&");
			}
			fprintf(fp,"v%d",op->no);
			if(IM_DEBUG){
				if(op->ifaddress==OP_ADDRESS)
				{
					printf("&");
				}
				printf("v%d",op->no);

			}
			break;
		}
		case OP_CONSTANT:{
			fprintf(fp,"#%d",op->value);
			if(IM_DEBUG){
				printf("#%d",op->value);
			}
			break;
		}
		case OP_FUNCTION:{
			fprintf(fp,"%s",op->funcname);
			if(IM_DEBUG){
				printf("%s",op->funcname);
			}
			break;
		}
		case OP_TEMPVAR:{
			if(op->ifaddress==OP_ADDRESS){
				fprintf(fp,"*");
			}
			fprintf(fp,"t%d",op->no);
			if(IM_DEBUG){
				if(op->ifaddress==OP_ADDRESS){
						printf("*");
					}
				printf("t%d",op->no);
			}
			break;
		}
		case OP_LABEL:{
			fprintf(fp,"%d",op->no);
			if(IM_DEBUG){
				printf("%d",op->no);
			}
			break;
		}
		default:
			break;
		
	}
}
void fprintintercode(FILE*fp){
	//遍历链表,打印;
	
	// if(IM_DEBUG){
	// 	;//测试版本
	// 	printf("---------------intercode-----------------------\n");
	// 	struct Intercodes *temp=inter_head->next;
	// 	while(temp!=inter_head){
	// 		//printf("%d\n",temp->code.kind);
	// 		switch(temp->code.kind){
	// 			case IN_FUNCTION:{
	// 				printf("FUNCTION ");
	// 				printop(temp->code.u.one.op0,fp);
	// 				printf(" : \n");
	// 				break;
	// 			}
	// 			case IN_PARAM:{
	// 				printf("PARAM ");
	// 				printop(temp->code.u.one.op0,fp);
	// 				printf("\n");
	// 				break;
	// 			}
	// 			case IN_RETURN:{
	// 				printf("RETURN ");
	// 				printop(temp->code.u.one.op0,fp);
	// 				printf("\n");
	// 				break;
	// 			}
	// 			case IN_LABEL:{
	// 				printf("LABEL label");
	// 				printop(temp->code.u.one.op0,fp);
	// 				printf(" : \n");
	// 				break;
	// 			}
	// 			case IN_GOTO:{
	// 				printf("GOTO label");
	// 				printop(temp->code.u.one.op0,fp);
	// 				printf("\n");
	// 				break;
	// 			}
	// 			case IN_WRITE:{
  //         printf("WRITE ");
  //         printop(temp->code.u.one.op0, fp);
  //         printf("\n");
	// 				break;
	// 			}
	// 			case IN_READ:{
  //         printf("READ ");
  //         printop(temp->code.u.one.op0, fp);
  //         printf("\n");
	// 				break;
	// 			}
	// 			case IN_ARG:{
	// 				printf("ARG ");
  //         printop(temp->code.u.one.op0, fp); 
  //         printf(" \n");
  //         break;
	// 			}
	// 			case IN_ASSIGN:{
	// 				printop(temp->code.u.two.left,fp);
	// 				printf(" := ");
	// 				printop(temp->code.u.two.right,fp);
	// 				printf("\n");
	// 				break;
	// 			}
	// 			case IN_DEC:{
	// 				printf("DEC ");
	// 				printop(temp->code.u.two.left,fp);
	// 				printf(" %d",temp->code.u.two.right->value);
	// 				printf("\n");
	// 				break;
	// 			}
	// 			case IN_CALL:{
  //         printop(temp->code.u.two.left, fp);
  //         printf(" := CALL ");
  //         printop(temp->code.u.two.right, fp);
  //         printf("\n");
	// 				break;
	// 			}
	// 			case IN_ADD:{
	// 				printop(temp->code.u.three.result,fp);
	// 				printf(" := ");
	// 				printop(temp->code.u.three.op1,fp);
	// 				printf(" + ");
	// 				printop(temp->code.u.three.op2,fp);
	// 				printf("\n");
	// 				//printf("ADD GG\n");
	// 				break;
	// 			}
	// 			case IN_SUB:{
	// 				printop(temp->code.u.three.result,fp);
	// 				printf(" := ");
	// 				printop(temp->code.u.three.op1,fp);
	// 				printf(" - ");
	// 				printop(temp->code.u.three.op2,fp);
	// 				printf("\n");
	// 				break;
	// 			}
	// 			case IN_MUL:{
	// 				printop(temp->code.u.three.result,fp);
	// 				printf(" := ");
	// 				printop(temp->code.u.three.op1,fp);
	// 				printf(" * ");
	// 				printop(temp->code.u.three.op2,fp);
	// 				printf("\n");
	// 				break;
	// 			}
	// 			case IN_DIV:{
	// 				printop(temp->code.u.three.result,fp);
	// 				printf(" := ");
	// 				printop(temp->code.u.three.op1,fp);
	// 				printf(" / ");
	// 				printop(temp->code.u.three.op2,fp);
	// 				printf("\n");
	// 				break;
	// 			}
	// 			case IN_IFGOTO:{
	// 				printf("IF ");
	// 				printop(temp->code.u.four.op1,fp);
	// 				printf(" %s ",temp->code.u.four.relop);
	// 				printop(temp->code.u.four.op2,fp);
	// 				printf(" GOTO label");
  //         printop(temp->code.u.four.op3, fp);
  //         printf("\n");
	// 				break;
	// 			}
	// 			default:{
	// 				;
	// 				break;
	// 			}
	// 		}
			
			
	// 		temp=temp->next;
	// 	}
	// 	printf("---------------intercode above-----------------\n");
	// }

	struct Intercodes *temp=inter_head->next;
	while(temp!=inter_head){
		//printf("%d\n",temp->code.kind);
		switch(temp->code.kind){
			case IN_FUNCTION:{
				fprintf(fp,"FUNCTION ");
				printop(temp->code.u.one.op0,fp);
				fprintf(fp," : \n");
				break;
			}
			case IN_PARAM:{
				fprintf(fp,"PARAM ");
				printop(temp->code.u.one.op0,fp);
				fprintf(fp,"\n");
				break;
			}
			case IN_RETURN:{
				fprintf(fp,"RETURN ");
				printop(temp->code.u.one.op0,fp);
				fprintf(fp,"\n");
				break;
			}
			case IN_LABEL:{
				fprintf(fp,"LABEL label");
				printop(temp->code.u.one.op0,fp);
				fprintf(fp," : \n");
				break;
			}
			case IN_GOTO:{
				fprintf(fp,"GOTO label");
				printop(temp->code.u.one.op0,fp);
				fprintf(fp,"\n");
				break;
			}
			case IN_WRITE:{
				fprintf(fp,"WRITE ");
				printop(temp->code.u.one.op0, fp);
				fprintf(fp,"\n");
				break;
			}
			case IN_READ:{
				fprintf(fp,"READ ");
				printop(temp->code.u.one.op0, fp);
				fprintf(fp,"\n");
				break;
			}
			case IN_ARG:{
				fprintf(fp,"ARG ");
				printop(temp->code.u.one.op0, fp); 
				fprintf(fp," \n");
				break;
			}
			case IN_ASSIGN:{
				printop(temp->code.u.two.left,fp);
				fprintf(fp," := ");
				printop(temp->code.u.two.right,fp);
				fprintf(fp,"\n");
				break;
			}
			case IN_DEC:{
				fprintf(fp,"DEC ");
				printop(temp->code.u.two.left,fp);
				fprintf(fp," %d",temp->code.u.two.right->value);
				fprintf(fp,"\n");
				break;
			}
			case IN_CALL:{
				printop(temp->code.u.two.left, fp);
				fprintf(fp," := CALL ");
				printop(temp->code.u.two.right, fp);
				fprintf(fp,"\n");
				break;
			}
			case IN_ADD:{
				printop(temp->code.u.three.result,fp);
				fprintf(fp," := ");
				printop(temp->code.u.three.op1,fp);
				fprintf(fp," + ");
				printop(temp->code.u.three.op2,fp);
				fprintf(fp,"\n");
				//printf("ADD GG\n");
				break;
			}
			case IN_SUB:{
				printop(temp->code.u.three.result,fp);
				fprintf(fp," := ");
				printop(temp->code.u.three.op1,fp);
				fprintf(fp," - ");
				printop(temp->code.u.three.op2,fp);
				fprintf(fp,"\n");
				break;
			}
			case IN_MUL:{
				printop(temp->code.u.three.result,fp);
				fprintf(fp," := ");
				printop(temp->code.u.three.op1,fp);
				fprintf(fp," * ");
				printop(temp->code.u.three.op2,fp);
				fprintf(fp,"\n");
				break;
			}
			case IN_DIV:{
				printop(temp->code.u.three.result,fp);
				fprintf(fp," := ");
				printop(temp->code.u.three.op1,fp);
				fprintf(fp," / ");
				printop(temp->code.u.three.op2,fp);
				fprintf(fp,"\n");
				break;
			}
			case IN_IFGOTO:{
				fprintf(fp,"IF ");
				printop(temp->code.u.four.op1,fp);
				fprintf(fp," %s ",temp->code.u.four.relop);
				printop(temp->code.u.four.op2,fp);
				fprintf(fp," GOTO label");
				printop(temp->code.u.four.op3, fp);
				fprintf(fp,"\n");
				break;
			}
			default:{
				;
				break;
			}
		}
		
		
		temp=temp->next;
	}
	fclose(fp);

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
	tempcode->next=NULL;
	
	//根据kind来决定是几目操作符;
	switch (kind){
		//单目
		case IN_FUNCTION:
		case IN_PARAM:
		case IN_RETURN:
		case IN_LABEL:
		case IN_GOTO:
		case IN_WRITE:
		case IN_READ:
		case IN_ARG:
			tempcode->code.u.one.op0=va_arg(args, Operand);
			//printf("op0:%s\n",tempcode->code.u.one.op0->funcname);
			break;
		//双目
		case IN_ASSIGN:
		case IN_DEC:
		case IN_CALL:{
			tempcode->code.u.two.left=va_arg(args, Operand);
			tempcode->code.u.two.right=va_arg(args, Operand);
			break;
		}
		//三目 result=op1+op2
		case IN_ADD:
		case IN_SUB:
		case IN_MUL:
		case IN_DIV:{
			tempcode->code.u.three.result=va_arg(args,Operand);
			tempcode->code.u.three.op1=va_arg(args,Operand);
			tempcode->code.u.three.op2=va_arg(args,Operand);
			break;}
		//四目
		case IN_IFGOTO://  (if)op1,relop,op2,(goto)op3
			tempcode->code.u.four.op1=va_arg(args,Operand);
			tempcode->code.u.four.relop=va_arg(args,char*);
			tempcode->code.u.four.op2=va_arg(args,Operand);
			tempcode->code.u.four.op3=va_arg(args,Operand);
		if(IM_DEBUG)	printf("create relop:%s\n",tempcode->code.u.four.relop);
			break;
		default:
		if(IM_DEBUG){
			printf("new_intercode to be done!\n");
			assert(0);}
		break;

	}
	add_inter(tempcode);
	va_end(args);

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


int gettypesize(Type cur){
	//如果是int返回4,如果是结构体那么返回结构体大小;
//	printf("herer\n");
	if(cur->kind==BASIC){
		return 4;
	}else if(cur->kind==STRUCTURE){
		//从上到下得到每一个元素的大小然后相加;
		int result=0;
		FieldList temp=cur->u.structure_.structure;
		//printf("tempname1:%s\n",cur->u.structure_.name);
		//Type cur2=temp->type;
		//printf("tempname2:%d\n",cur2->kind);
		// Type cur3=temp->tail->type;
		// printf("tempname3:%d\n",cur3->kind);
		while(temp!=NULL){
			Type temptype=temp->type;
			int tempsize=gettypesize(temptype);
			result+=tempsize;
			//printf("tempsize:%d \n",tempsize);
			temp=temp->tail;
		}
	//	printf("result:%d\n",result);
		return result;

	}else if(cur->kind==ARRAY){
		//对于struct TEMP a[3][4][5],因为遍历顺序是3-4-5-TEMP,先找到TEMP的大小然后再相乘就行;
		int mulresult=1;
		Type type=cur;
		while(type!=NULL){
			if(type->kind!=ARRAY)break;
			//printf("type size:%d\n",type->u.array_.size);
			mulresult*=type->u.array_.size;
			type=type->u.array_.elem;
		}
		int typesize=gettypesize(type);
		mulresult*=typesize;
		//printf("mulresult:%d\n",mulresult);
		return mulresult;

	}
}

// int getarraysize(struct Symbol_node* cur){
// 	//根据传入的数组元素得到size的大小;
// 	//printf("In getarraysize\n");
// 	int result=1;
// 	if(cur->field.type->kind!=ARRAY){
// 		assert(0);
// 	}
// 	Type type=cur->field.type;
// 	//a[3][4][5]  3->4->5->int
// 	//printf("size1:%d\n",type->u.array_.size);
// 	while(type!=NULL){
// 		if(type->kind!=ARRAY)break;
// 		result*=type->u.array_.size;
// 		//printf("size:%d\n",type->u.array_.size);
// 		type=type->u.array_.elem;
// 	}
// 	result*=4;
// 	//需要修改哦 如果是结构体咋整.jpg
// 	//printf("result:%d\n",result);

// 	return result;
// }






////////////////
////////////////
//正式分析
int intermediate_generate(struct Node*cur,FILE*fp)
{
	
	
	success_g=1;
	var_cnt=0;//v1,v2,....
	temp_cnt=0;//t1,t2...
	label_cnt=0;//label1,label2,...
	inter_head=(struct Intercodes*)malloc(sizeof(struct Intercodes));
	inter_head->next=inter_head;
	inter_head->prev=inter_head;
	inter_tail=inter_head;
	Program_g(cur);
if(IM_DEBUG)	show_global_table();
if(IM_DEBUG)	show_struct_table();
	fprintintercode(fp);
}
int Program_g(struct Node* cur){
	
	
	struct Node*Extdeflistnode=getchild(cur,0);
	int result=ExtDefList_g(Extdeflistnode);
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
	if(IM_DEBUG)printf("funop: %s\n",funop->funcname);
	new_intercode(IN_FUNCTION,funop);
	//oneop_intercode(IN_FUNCTION,funop);

	int query_ok=0;
	struct Symbol_node* fun_symbol=query_symbol2(funop->funcname,&query_ok);
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
			if(IM_DEBUG)printf("param,name:%s\n",params->name);
			Operand paramop=NULL;
			if(params->type->kind==ARRAY||params->type->kind==STRUCTURE){
				paramop=new_op(OP_VARIABLE,OP_ADDRESS,(char*)params->name);
				}
			else{
				paramop=new_op(OP_VARIABLE,OP_VAR,(char*)params->name);
			}
		//	paramop=new_op(OP_VARIABLE,OP_VAR,(char*)params->name);
			//	printf("%s",paramop->u.varname);
			//先找到这个变量的name,然后更新var_no;
			int query_ok2=0;
			struct Symbol_node* queryid=query_symbol2(params->name,&query_ok2);
			if(query_ok2==0)assert(0);
			
			queryid->var_no=paramop->no;
			
			queryid->ifaddress=paramop->ifaddress;
			paramop->ifaddress=OP_VAR;
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
	Stmt -> Exp SEMI ok
	| CompSt ok
	| RETURN Exp SEMI ok
	| IF LP Exp RP Stmt ok
	| IF LP Exp RP Stmt ELSE Stmt ok
	| WHILE LP Exp RP Stmt ok
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
		struct Node*stmtnode=getchild(cur,4);
		if(strcheck(stmtnode->name,"Stmt")!=1){
			if(IM_DEBUG){
			printf("while error\n");
			assert(0);}
		}
		Stmt_g(stmtnode);
		new_intercode(IN_GOTO,label1);
		new_intercode(IN_LABEL,label2);
		
	}else if(strcheck(tempnode1->name,"IF")){
		Operand label1=new_op(OP_LABEL,OP_VAR);//false label
		//和while一样省略true label;
		//if !cond goto false label
		// stmt
		//false label:
		//IF LP Exp RP Stmt
		//IF LP Exp RP Stmt ELSE Stmt
		struct Node* expnode=getchild(cur,2);
		Cond_g(expnode,NULL,label1);
		struct Node* stmtnode=getchild(cur,4);
		Stmt_g(stmtnode);
		struct Node* tempnode6=getchild(cur,5);
		if(tempnode6==NULL){
			new_intercode(IN_LABEL,label1);
		}else{
			Operand label2=new_op(OP_LABEL,OP_VAR);
			//if !cond goto false label
			//stmt
			//goto label2 也就是跳出if else
			//false label:
			//else 对应的stmt
			//
			//label 2:
			//
			new_intercode(IN_GOTO,label2);
			new_intercode(IN_LABEL,label1);
			struct Node*stmtnode2=getchild(cur,6);
			Stmt_g(stmtnode2);
			new_intercode(IN_LABEL, label2);
		}

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
		struct Node* tempnode2=getchild(cur,2);
		Operand op2=Exp_g(tempnode2);
		
		new_intercode(IN_ASSIGN,op1,op2);

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
		int query_success=0;
		struct Symbol_node* queryid=query_symbol2(tempnode->string_contant,&query_success);

		int typesize=gettypesize(queryid->field.type);
		if(IM_DEBUG)printf("vardec name:%s\n",tempnode->string_contant);
		// char*able=(char*)malloc(sizeof(char)*64);
		// strcpy(able,tempnode->string_contant);
		result=new_op(OP_VARIABLE,OP_VAR,tempnode->string_contant);
		queryid->ifaddress=result->ifaddress;
		queryid->var_no=result->no;//更新symbol_node里面对应的var_no;
		if(typesize==4){
			;//int 不用管;
		}else{
			;//struct 申请空间;
			Operand op2=new_op(OP_CONSTANT,OP_VAR,typesize);
			new_intercode(IN_DEC,result,op2);
		}
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
		if(IM_DEBUG)printf("findnode name:%s\n",findnode->string_contant);
		// char*able=(char*)malloc(sizeof(char)*64);
		// strcpy(able,findnode->string_contant);
		result=new_op(OP_VARIABLE,OP_VAR,findnode->string_contant);
		queryid->ifaddress=result->ifaddress;
		queryid->var_no=result->no;
		int arraysize=gettypesize(queryid->field.type);
		Operand op2=new_op(OP_CONSTANT,OP_VAR,arraysize);
		new_intercode(IN_DEC,result,op2);



		
	}

	return result;
}


void Arg_g(struct Node*cur,FieldList param){
	// Args -> Exp COMMA Args
	// | Exp
	if(cur==NULL)return;
	if(param==NULL)return;
	//printf("In args\n\n\n");
	//递归检查参数;
	struct Node*tempnode1=getchild(cur,0);
	if(Exp_g(tempnode1)!=NULL){
	Operand tempop=Exp_g(tempnode1);//使用局部变量防止修改原来的值;
	if(IM_DEBUG)printf("ori value:%d",tempop->value);
	Operand op=copyop(tempop);
	
	if(param->type->kind==STRUCTURE||param->type->kind==ARRAY){
		if(op->ifaddress==OP_ADDRESS){
			
			op->ifaddress=OP_VAR;//如果是address就不用专门的&了
		}else{
			op->ifaddress=OP_ADDRESS;
		}
	}
	struct Node*tempnode2=getchild(cur,1);
	if(tempnode2==NULL){
		;
	}else{
	struct Node*tempnode3=getchild(cur,2);
		Arg_g(tempnode3,param->tail);
	}
	// if(op!=NULL)
	new_intercode(IN_ARG,op);
	// else{
	// 	printf("GG in exp\n");
	// }
	}else{
		if(IM_DEBUG){
		printf("GG in exp arg\n\n\n");
		assert(0);
		}
	}


	;
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

	| ID LP Args RP 4函数 ok
	| ID LP RP 3 ok

	| Exp LB Exp RB4 数组 maybeok
	| Exp DOT ID3 结构体; 

	| ID1 ok
	| INT1 ok
	| FLOAT1 ok
	*/
	Operand temp=NULL;

	struct Node*tempnode1=getchild(cur,0);
	if(tempnode1==NULL)return temp;
	
	if(strcheck(tempnode1->name,"ID")){
		//To be done!!!;
		//所有vairable注册的时候都要添加到symbol_table里面的va_no里面,这样子就可以查找了;
		struct Node*tempnode2=getchild(cur,1);
		if(tempnode2==NULL){
		//	printf("ID:%s\n",tempnode1->string_contant);
			int queryok=0;
			struct Symbol_node*queryid=query_symbol2(tempnode1->string_contant,&queryok);
			if(queryok==0)assert(0);
			//如果是基本变量,那么返回;如果是数组或者结构体,那么感觉最好改成Address然后返回;
			
			if(queryid->field.type->kind==ARRAY||queryid->field.type->kind==STRUCTURE){
				if(queryid->ifaddress==OP_ADDRESS){
					//printf("id_address in ID\n");
					temp=new_op(OP_VARIABLE,OP_ADDRESS,tempnode1->string_contant);
				}
				else{
					//不确定;
					temp=new_op(OP_VARIABLE,OP_VAR,tempnode1->string_contant);
				}
				var_cnt--;
				temp->no=queryid->var_no;
				if(queryid->var_no==-1)assert(0);
			//	printf("varname:%s\n",temp->varname);
				temp->depth=0;
				return temp;
			}else{
				temp=new_op(OP_VARIABLE,OP_VAR,tempnode1->string_contant);
				var_cnt--;
				temp->no=queryid->var_no;
				if(queryid->var_no==-1)assert(0);
				temp->depth=0;
				return temp;
			}
			//return temp;
		}else{
			//函数部分To be done;
			//| ID LP Args RP 4函数
			//| ID LP RP 3
			struct Node*tempnode3=getchild(cur,2);


			if(strcheck(tempnode1->string_contant,"write")){
				if(strcheck(tempnode3->name,"Args")){
					//write(n) 
					struct Node* tempnode31=getchild(tempnode3,0);
					if(strcheck(tempnode31->name,"Exp")){
						temp=Exp_g(tempnode31);
					}
					if(temp!=NULL)
					new_intercode(IN_WRITE,temp);
					else{
						if(IM_DEBUG){
						printf("GG in exp\n");
						assert(0);
						}
					}
					return temp;
				}
				// if(IM_DEBUG)
				// assert(0);
			}

			temp=new_op(OP_TEMPVAR,OP_VAR);
			if(strcheck(tempnode1->string_contant,"read")){
				new_intercode(IN_READ,temp);
				return temp;
			}

			Operand functionname=new_op(OP_FUNCTION,OP_VAR,tempnode1->string_contant);
			if(strcheck(tempnode3->name,"RP")){
				//ID LP RP, CALL function.name
				new_intercode(IN_CALL,temp,functionname);
				return temp;
			}else if(strcheck(tempnode3->name,"Args")){
			//| ID LP Args RP 4函数
				int queryok=0;
				struct Symbol_node*queryid=query_symbol2(tempnode1->string_contant,&queryok);
				
				Arg_g(tempnode3,queryid->field.type->u.function.params);
				new_intercode(IN_CALL,temp,functionname);
				return temp;
			}
			//if(IM_DEBUG)assert(0);
			return temp;


		}
	}else if(strcheck(tempnode1->name,"INT")){
		if(IM_DEBUG)printf("In exp INT:%d\n",tempnode1->int_contant);
		temp=new_op(OP_CONSTANT,OP_VAR,tempnode1->int_contant);
		return temp;

	}else if(strcheck(tempnode1->name,"FLOAT")){
		//理论上不应该出现;如果出现那就标作常数0吧!!
		temp=new_op(OP_CONSTANT,OP_VAR,0);
		return temp;	
		
	}else if(strcheck(tempnode1->name,"LP")){
		struct Node*expnode=getchild(cur,1);
		return Exp_g(expnode);
	}else if(strcheck(tempnode1->name,"MINUS")){
		//zero 就是#0
		//op1是处理的exp部分;
		//t1=#0-op1;return t1;
		Operand zero=new_op(OP_CONSTANT,OP_VAR,0);
		struct Node*expnode=getchild(cur,1);
		Operand op1=Exp_g(expnode);
		Operand op2=new_op(OP_TEMPVAR,OP_VAR);
		new_intercode(IN_SUB,op2,zero,op1);//op2=zero-op1;
		temp=op2;
		return temp;
	}else if(strcheck(tempnode1->name,"NOT")||
		(strcheck(tempnode1->name,"Exp")&&tempnode1->next_sib!=NULL&&strcheck(tempnode1->next_sib->name,"RELOP"))||
		(strcheck(tempnode1->name,"Exp")&&tempnode1->next_sib!=NULL&&strcheck(tempnode1->next_sib->name,"AND"))||
		(strcheck(tempnode1->name,"Exp")&&tempnode1->next_sib!=NULL&&strcheck(tempnode1->next_sib->name,"OR"))
	){
		//来自讲义,尽管我觉得需要优化;
		Operand label1=new_op(OP_LABEL,OP_VAR);
		Operand label2=new_op(OP_LABEL,OP_VAR);
		temp=new_op(OP_TEMPVAR,OP_VAR);
		Operand zero=new_op(OP_CONSTANT,OP_VAR,0);
		new_intercode(IN_ASSIGN,temp,zero);
		Cond_g(cur,label1,label2);
		new_intercode(IN_LABEL,label1);
		Operand one=new_op(OP_CONSTANT,OP_VAR,1);
		new_intercode(IN_ASSIGN,temp,one);
		new_intercode(IN_LABEL,label2);
		return temp;

	}else if(strcheck(tempnode1->name,"Exp")){
		
		struct Node*tempnode2=getchild(cur,1);
		if(
				(strcheck(tempnode2->name,"PLUS"))||
				(strcheck(tempnode2->name,"MINUS"))||
				(strcheck(tempnode2->name,"STAR"))||
				(strcheck(tempnode2->name,"DIV"))
			){
				int in_kind=arithmetic_kind(tempnode2->name);
				temp=new_op(OP_TEMPVAR,OP_VAR);
				struct Node*expnode1=tempnode1;
				struct Node*expnode2=getchild(cur,2);
				Operand op1=Exp_g(expnode1);
				Operand op2=Exp_g(expnode2);
				if(op1!=NULL&&op2!=NULL)
				new_intercode(in_kind,temp,op1,op2);
				else{
					if(IM_DEBUG){
					printf("GG in exp\n");
					assert(0);
					}
				}
				return temp;
			}
		else if(strcheck(tempnode2->name,"ASSIGNOP")){
			//	temp=new_op(OP_TEMPVAR,OP_VAR);
				struct Node*expnode1=tempnode1;
				struct Node*expnode2=getchild(cur,2);
				Operand op1=Exp_g(expnode1);
				Operand op2=Exp_g(expnode2);
				if(op1!=NULL&&op2!=NULL)
				new_intercode(IN_ASSIGN,op1,op2);
				else{
				//	assert(0);
				if(IM_DEBUG){
					printf("GG in exp\n");
					assert(0);
				}
				}
			
				temp=op1;
				return temp;
		}
		else if(strcheck(tempnode2->name,"DOT")){
			//需要实现的函数:确定元素位置;
			//| Exp DOT ID3 结构体; 
			//返回一个地址;
		//	printf("structure\n");
			Operand expop=Exp_g(tempnode1);
			Operand tempexpop=copyop(expop);
			//printf("exp ifaddress:%d no:%d\n",expop->ifaddress,expop->no);
			struct Node* tempnode3=getchild(cur,2);
			int queryok=0;
			struct Symbol_node* queryid=query_symbol2(tempnode3->string_contant,&queryok);
			int offset=queryid->offset;
		//	printf("name:%s offset:%d no\n",tempnode3->string_contant,offset);
			//Operand tempop=new_op(OP_VARIABLE,OP_ADDRESS,tempnode3->string_contant);
			//var_cnt--;
			//需要找struct的id;
			
			//tempop->no=querystruct->var_no;
			if(offset==0){
				Operand ttemp=new_op(OP_TEMPVAR,OP_VAR);
				if(tempexpop->ifaddress==OP_ADDRESS)
				tempexpop->ifaddress=OP_VAR;//反正只是地址的计算;
				else{
					tempexpop->ifaddress=OP_ADDRESS;
				}
				new_intercode(IN_ASSIGN,ttemp,tempexpop);
				temp=copyop(ttemp);
				temp->ifaddress=OP_ADDRESS;
				temp->varname=tempnode3->string_contant;
				return temp;
			}else{
				Operand constantop=new_op(OP_CONSTANT,OP_VAR,offset);
				Operand ttemp=new_op(OP_TEMPVAR,OP_VAR);
			//	Operand tempexpop=copyop(expop);
				//if(expop->ifaddress=OP_ADDRESS)
				if(tempexpop->ifaddress==OP_ADDRESS)
				tempexpop->ifaddress=OP_VAR;//反正只是地址的计算;
				else{
					tempexpop->ifaddress=OP_ADDRESS;
				}

				//printf("tempexpop no:%d\n",tempexpop->no);
				new_intercode(IN_ADD,ttemp,tempexpop,constantop);
			//	printf("tempexpop ifaddress:%d\n",tempexpop->ifaddress);
				temp=copyop(ttemp);
				temp->ifaddress=OP_ADDRESS;
				temp->varname=tempnode3->string_contant;
				return temp;
			}
			
		}
		else if(strcheck(tempnode2->name,"LB")){
			//Exp LB Exp RB4 
		//	return NULL;
		//	printf("herere\n\n");
			Operand expop1=copyop(Exp_g(tempnode1));
		//	printf("expop1 ifaddress:%d",expop1->ifaddress);
			int depth=expop1->depth;

			int query_ok=0;
			struct Symbol_node *queryid=query_symbol2(expop1->varname,&query_ok);
			Type ttemptype=queryid->field.type;
			Type temptype=ttemptype;
			int cnt=0;
			while(temptype->kind==ARRAY){
				cnt+=1;
			//	printf("size:%d\n",temptype->u.array_.size);
				temptype=temptype->u.array_.elem;
			}
		//	printf("typekind:%d\n",temptype->kind);
			int typesize=gettypesize(temptype);
		//	printf("cnt:%d\n",cnt);
			int*arraysize=(int*)malloc(sizeof(int)*(cnt+1));
			cnt=0;
			temptype=ttemptype;
			while(temptype->kind==ARRAY){
				arraysize[cnt]=temptype->u.array_.size;
				cnt+=1;
				temptype=temptype->u.array_.elem;
			}
		//	printf("cnt:%d\n",cnt);
			int ptr=cnt-1;
			int tempdepth=cnt-depth-1;
			int offset=1;
			while(tempdepth){
				offset*=arraysize[ptr];
				tempdepth-=1;
				ptr-=1;
			}
		//	printf("offset:%d\n",offset);
			free(arraysize);
			offset=offset*typesize;//得到最终偏移量,还需要乘[exp];

			struct Node*tempnode3=getchild(cur,2);
			Operand expop2=Exp_g(tempnode3);
			
			Operand tempop1=new_op(OP_TEMPVAR,OP_VAR);
			Operand constantop1=new_op(OP_CONSTANT,OP_VAR,offset);
			new_intercode(IN_MUL,tempop1,expop2,constantop1);

			Operand tempop2=new_op(OP_TEMPVAR,OP_VAR);
			tempop2->varname=expop1->varname;
			tempop2->depth=depth+1;
			//printf("expkind:%d",expop1->kind);
			//temp=NULL;
			if(depth==0&&expop1->ifaddress==OP_VAR){
				expop1->ifaddress=OP_ADDRESS;
			}else{
				expop1->ifaddress=OP_VAR;
			}
			new_intercode(IN_ADD,tempop2,expop1,tempop1);
		 // printf("tempop2depth:%d,cnt:%d\n",tempop2->depth,cnt);

			temp=copyop(tempop2);
			if(tempop2->depth==cnt){
				temp->ifaddress=OP_ADDRESS;
			}


			return temp;


		}
		
		


	}

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
	//printf("In cond\n");
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
				if(label_true!=NULL&&label_false!=NULL){
					if(op1!=NULL)
					new_intercode(IN_IFGOTO,op1,"!=",zero,label_true);
					new_intercode(IN_GOTO,label_false);
				}else if(label_true!=NULL){
					if(op1!=NULL)
					new_intercode(IN_IFGOTO,op1,"!=",zero,label_true);//if(a!=0){...}
				}else if(label_false!=NULL){
					if(op1!=NULL)
					new_intercode(IN_IFGOTO,op1,"==",zero,label_false);//if(a!=0){...}else{...} 因此如果a==0那就去else;
				}

			}else if(strcheck(tempnode2->name,"AND")){
				if(label_false!=NULL){
					Cond_g(tempnode1,NULL,label_false);
					struct Node*tempnode3=getchild(cur,2);
					Cond_g(tempnode3,label_true,label_false);		
				}else{
					Operand new_label=new_op(OP_LABEL,OP_VAR);
					Cond_g(tempnode1,NULL,new_label);
					struct Node*tempnode3=getchild(cur,2);
					Cond_g(tempnode3,label_true,label_false);
					new_intercode(IN_LABEL,new_label);
				}				//可能需要修改;需要测试!		
			}else if(strcheck(tempnode2->name,"OR")){
				//6-40 
				if(label_true!=NULL){
					Cond_g(tempnode1,label_true,NULL);
					struct Node*tempnode3=getchild(cur,2);
					Cond_g(tempnode3,label_true,label_false);
				}else{
					Operand new_label=new_op(OP_LABEL,OP_VAR);
					Cond_g(tempnode1,new_label,NULL);
					struct Node*tempnode3=getchild(cur,2);
					Cond_g(tempnode3,label_true,label_false);
					new_intercode(IN_LABEL,new_label);			
				}
				//可能需要修改;
			}else if(strcheck(tempnode2->name,"RELOP")){
				Operand op1=Exp_g(tempnode1);
				struct Node*tempnode3=getchild(cur,2);
				Operand op2=Exp_g(tempnode3);

				if(label_true!=NULL&&label_false!=NULL){
					if(op1!=NULL)
					new_intercode(IN_IFGOTO,op1,tempnode2->string_contant,op2,label_true);
					new_intercode(IN_GOTO,label_false);
				}else if(label_true!=NULL){
					if(op1!=NULL)
					new_intercode(IN_IFGOTO,op1,tempnode2->string_contant,op2,label_true);					
				}else if(label_false!=NULL){
					if(op1!=NULL)
					new_intercode(IN_IFGOTO,op1,Reverse_relop(tempnode2->string_contant),op2,label_false);
				}

			}else if(strcheck(tempnode2->name,"PLUS")||strcheck(tempnode2->name,"DIV")||strcheck(tempnode2->name,"STAR")||strcheck(tempnode2->name,"MINUS")){
				Operand op1=Exp_g(tempnode1);
				struct Node* tempnode3=getchild(cur,2);
				Operand op2=Exp_g(tempnode3);
				int in_kind=arithmetic_kind(tempnode2->string_contant);
				Operand result=new_op(OP_TEMPVAR,OP_VAR);
				if(op1!=NULL&&op2!=NULL)
				new_intercode(in_kind,result,op1,op2);
				else{
					if(IM_DEBUG){
						printf("GG in cond\n");
						assert(0);
					}
				}
				if(label_true!=NULL&&label_false!=NULL){
					new_intercode(IN_IFGOTO,result,"!=",zero,label_true);
					new_intercode(IN_GOTO,label_false);
				}
				else if(label_true!=NULL){
					new_intercode(IN_IFGOTO,result,"!=",zero,label_true);
				}
				else if(label_false!=NULL){
					new_intercode(IN_IFGOTO,result,"==",zero,label_false);
				}
			}else if(strcheck(tempnode2->name,"LB")){
				Operand op=Exp_g(cur);
				if(label_true!=NULL&&label_false!=NULL){
					new_intercode(IN_IFGOTO,op,"!=",zero,label_true);
					new_intercode(IN_GOTO,label_false);
				}else if(label_true!=NULL){
					new_intercode(IN_IFGOTO,op,"!=",zero,label_true);
				}else if(label_false!=NULL){
					new_intercode(IN_IFGOTO,op,"==",zero,label_false);
				}

			}else if(strcheck(tempnode2->name,"DOT")){
				Operand op=Exp_g(cur);
				if(label_true!=NULL&&label_false!=NULL){
					new_intercode(IN_IFGOTO,op,"!=",zero,label_true);
					new_intercode(IN_GOTO,label_false);
				}else if(label_true!=NULL){
					new_intercode(IN_IFGOTO,op,"!=",zero,label_true);
				}else if(label_false!=NULL){
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
			if(label_true!=NULL&&label_false!=NULL){
				new_intercode(IN_IFGOTO,op,"!=",zero,label_true);
				new_intercode(IN_GOTO,label_false);
			}else if(label_true!=NULL){
				new_intercode(IN_IFGOTO,op,"!=",zero,label_true);
			}else if(label_false!=NULL){
				new_intercode(IN_IFGOTO,op,"==",zero,label_false);
			}
		}else if(strcheck(tempnode1->name,"LP")){
			;//LP Exp RP
			//printf("cond LP\n");
			struct Node*expnode=getchild(cur,1);
			Cond_g(expnode,label_true,label_false);
		}else if(strcheck(tempnode1->name,"ID")){
			Operand op=Exp_g(cur);
			if(label_true!=NULL&&label_false!=NULL){
				new_intercode(IN_IFGOTO,op,"!=",zero,label_true);
				new_intercode(IN_GOTO,label_false);
			}else if(label_true!=NULL){
				new_intercode(IN_IFGOTO,op,"!=",zero,label_true);
			}else if(label_false!=NULL){
				new_intercode(IN_IFGOTO,op,"==",zero,label_false);
			}

		}else if(strcheck(tempnode1->name,"INT")){
			//如果>0 那么就去true 否则去false
			//不是很确定;
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








