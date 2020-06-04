#ifndef SIMULATER_H
#define SIMULATER_H

#include "common.h"
#include "imediate.h"
struct Register{
	int state;//表示状态 0为空闲1为使用
	char *name;//表示名字
};
struct Opdict{
	//变量和寄存器的字典;
	int reg;//寄存器编号;
	Operand op;//OP指针
	struct Opdict *next;//链表结构;
};
void s_code_generate(FILE* fp);

#endif