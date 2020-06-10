#ifndef SIMULATER_H
#define SIMULATER_H
#include "common.h"
#include "imediate.h"
#include "symbol_table.h"
struct Register{
	int state;//表示状态 0为空闲1为使用
	char *name;//表示名字
};

struct stack_node{
	int offset;
	// enum{
	// 	S_VARIABLE=0,
	// 	S_TEMPVAR=3
	// }kind;
	int kind;
	int no;
	struct stack_node*next;
};
struct pid_stack{
	struct stack_node*fp;
	struct pid_stack*next;
};

void s_code_generate(FILE* fp);

#endif