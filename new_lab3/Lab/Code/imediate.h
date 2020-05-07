#ifndef IMEDIATE_H
#define IMEDIATE_H

#include<stdio.h>
#include"common.h"
#include "symbol_table.h"
#include <assert.h>

#define IM_DEBUG 1

typedef struct Operand_* Operand;
struct Operand_{
	enum{
			OP_VARIABLE,
			OP_CONSTANT,
			OP_FUNCTION,
			OP_TEMPVAR,
			OP_LABEL
			//To be done
		}kind;
	enum{
		OP_ADDRESS,
		OP_VAR
	}ifaddress;
	union{
		char*varname;
		int no;
		int value;
		char*funcname;
		//To be done

	}u;
};
struct InterCode{
	enum{
		//单目
		IN_FUNCTION,
		IN_PARAM,
		IN_RETURN,
		IN_LABEL,
		IN_GOTO,
		//双目
		IN_ASSIGN,
		IN_DEC,
		IN_ADD,
		IN_SUB,
		IN_MUL,
		IN_DIV,

		//四目
		IN_IFGOTO
	}kind;

	union{
		struct{
			Operand op0;
		}one;
		struct{
			Operand left;
			Operand right;

		}two;
		struct {
			Operand result;
			Operand op1;
			Operand op2;
		}three;
		struct {//实际上是op1,relop1,op2,op3
			Operand op1;
			Operand op2;
			Operand op3;
			char*relop;
		}four;

		//To be done
	}u;
};
struct Intercodes{
	struct InterCode code;
	struct Intercodes *prev;
	struct Intercodes*next;
};


int intermediate_generate(struct Node*cur);
int Program_g(struct Node* cur);
int ExtDefList_g(struct Node* cur);
int FunDec_g(struct Node* cur);
int CompSt_g(struct Node* cur);
int DefList_g(struct Node*cur);
int StmtList_g(struct Node*cur);
int Def_g(struct Node*cur);
int DecList_g(struct Node*cur);
int Dec_g(struct Node*cur);
Operand VarDec_g(struct Node*cur);
Operand Exp_g(struct Node*cur);
int Stmt_g(struct Node* cur);

int Cond_g(struct Node* cur,Operand label_true,Operand label_false);

#endif