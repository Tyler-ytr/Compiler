#include "simulater.h"
extern struct Intercodes*inter_head;
struct Register _reg[32];//寄存器数组.保存寄存器编号,名字和状态
struct Opdict * dict_head;//Op与寄存器的关系,head
struct Opdict * dict_cur;//当前状态
extern int var_cnt;
extern int temp_cnt;
extern int label_cnt;
extern int dec_cnt;
extern void printop(Operand op,FILE*fp);

//朴素 从内存取出到t0,t1,t2(8,9,10)然后计算然后放回去;其余寄存器作为中间变量使用;
// t4(12) 存放地址;
void init_reg(){
	//初始化;
	//寄存器的名字和状态初始化;
	for(int i=0;i<32;++i){
		_reg[i].state=0;
	}
	//别名初始化;
	_reg[0].name="$zero";
	_reg[1].name="$at";
	_reg[2].name="$v0";
	_reg[3].name="$v1";

	_reg[4].name="$a0";
	_reg[5].name="$a1";
	_reg[6].name="$a2";
	_reg[7].name="$a3";

	_reg[8].name="$t0";
	_reg[9].name="$t1";
	_reg[10].name="$t2";
	_reg[11].name="$t3";

	_reg[12].name="$t4";
	_reg[13].name="$t5";
	_reg[14].name="$t6";
	_reg[15].name="$t7";

	_reg[16].name="$s0";
	_reg[17].name="$s1";
	_reg[18].name="$s2";
	_reg[19].name="$s3";

	_reg[20].name="$s4";
	_reg[21].name="$s5";
	_reg[22].name="$s6";
	_reg[23].name="$s7";

	_reg[24].name="$t8";
	_reg[25].name="$t9";

	_reg[26].name="$k0";
	_reg[27].name="$k1";

	_reg[28].name="$gp";
	_reg[29].name="$sp";

	_reg[30].name="$fp";
	_reg[31].name="$ra";

}
void init_data(FILE *fp){
	//数据段初始化;
	fprintf(fp,".data\n");
	fprintf(fp, "_prompt: .asciiz \"Enter an integer:\"\n_ret: .asciiz \"\\n\"\n.globl main\n");
	//对数组,结构体进行申请内存;并且记录相应的变量;

	struct Intercodes *temp=inter_head->next;
	while(temp!=inter_head){
		if(temp->code.kind==IN_DEC){
			printop(temp->code.u.two.left,fp);
			fprintf(fp,": .word ");
			fprintf(fp,"%d",temp->code.u.two.right->value);
			fprintf(fp,"\n");
		}
		temp=temp->next;
		;
	}
	//开辟var_cnt,temp_cnt;
	//int temp=dec_cnt+1;
	for(int i=dec_cnt+1;i<var_cnt;i++){
		fprintf(fp,"v%d: .word 4\n",i);
	}
		for(int i=0;i<temp_cnt;i++){
		fprintf(fp,"t%d: .word 4\n",i);
	}
	printf("var_cnt:%d, temp_cnt:%d dec_cnt:%d\n",var_cnt,temp_cnt,dec_cnt);
	//[0,var_cnt),[0,temp_cnt)
}
void init_code(FILE*fp){
	//函数段初始化;read and write
	//read
	fprintf(fp, ".text\n");
	fprintf(fp, "read:\n");
	fprintf(fp, "  li $v0, 4\n");
	fprintf(fp, "  la $a0, _prompt\n");
	fprintf(fp, "  syscall\n");
	fprintf(fp, "  li $v0, 5\n");
	fprintf(fp, "  syscall\n");
	fprintf(fp, "  jr $ra\n\n");
	//write
	fprintf(fp, "write:\n");
	fprintf(fp, "  li $v0, 1\n");
	fprintf(fp, "  syscall\n");
	fprintf(fp, "  li $v0, 4\n");
	fprintf(fp, "  la $a0, _ret\n");
	fprintf(fp, "  syscall\n");
	fprintf(fp, "  move $v0, $0\n");
	fprintf(fp, "  jr $ra\n");
}
void load_reg(Operand op,int reg,FILE*fp){//reg不使用t5,t6,t7,t8
	switch(op->kind){
		case OP_VARIABLE:{
			if(op->ifaddress==OP_ADDRESS){
				//赋给reg地址;
				fprintf(fp,"la %s, v%d\n",_reg[reg].name,op->no);
			}
			//赋给reg地址里面的值;
			else{
				fprintf(fp,"lw %s, v%d\n",_reg[reg].name,op->no);
			}
			break;
		}
		case OP_CONSTANT:{
			fprintf(fp,"li %s, %d\n",_reg[reg].name,op->value);
			break;
		}
		case OP_TEMPVAR:{
			if(op->ifaddress==OP_ADDRESS){
				//赋给临时寄存器temp var的值也就是目标的地址;
				//通过临时寄存器得到值;
				fprintf(fp,"lw %s, t%d\n",_reg[14].name,op->no);
				//t6;
				fprintf(fp,"lw %s, 0(%s)\n",_reg[reg].name,_reg[14].name);
			}else{
				//赋给寄存器temp_var里面的值;
				fprintf(fp,"lw %s, t%d\n",_reg[reg].name,op->no);
			}
			break;
		}
		case OP_FUNCTION:{
			//赋给reg FUNCTION的地址;
			fprintf(fp,"la %s, %s\n",_reg[reg].name,op->funcname);
			break;
		}
		case OP_LABEL:{
			fprintf(fp,"la %s, label%d\n",_reg[reg].name,op->no);
			break;
		}
		
	}
	;
}
void save_reg(Operand op,int reg,FILE*fp){
	switch(op->kind){
		case OP_VARIABLE:{
			if(op->ifaddress==OP_ADDRESS){
				printf("error! can't save address\n");
				assert(0);
			}
			else{
				fprintf(fp,"sw %s, v%d\n",_reg[reg].name,op->no);
			}
			break;
		}
		case OP_TEMPVAR:{
			if(op->ifaddress==OP_ADDRESS){
				fprintf(fp,"lw %s, t%d\n",_reg[14].name,op->no);
				//t6存储要存的目标地址;
				fprintf(fp,"sw %s, 0(%s)\n",_reg[reg].name,_reg[14].name);
				//往目标地址存;

			}else{
				fprintf(fp,"sw %s, t%d\n",_reg[reg].name,op->no);
			}
			break;
		}
		default:
			printf("Impossible to get here in save_reg\n");
			assert(0);
	}
}


void trans_one_code(FILE *fp,struct Intercodes *cur){
	int kind=cur->code.kind;
	switch(cur->code.kind){
		case IN_FUNCTION:{
			//To be done;

			break;
		}
		case IN_PARAM:{
			//To be done;
			break;
		}
		case IN_RETURN:{
			//To be done;
			break;
		}
		case IN_LABEL:{
			//To be done;
			break;
		}
		case IN_GOTO:{
			//To be done;
			break;
		}
		case IN_WRITE:{
			//To be done;
			break;

		}
		case IN_READ:{
			//To be done;
			break;
		}
		case IN_ARG:{
			//To be done;
			break;
		
		}
		case IN_ASSIGN:{
			//To be done:
			break;
		}
		case IN_DEC:{
			//Done in init;
			break;
		}
		case IN_CALL:{
			//To be done;
			break;
		}
		case IN_ADD:{
			//To be done;
			int result_reg=8;//t0
			int left_reg=9;//t1
			int right_reg=10;//t2;
		//需要修改;
		//	load_reg(cur->code.u.three.result,result_reg,fp);
			load_reg(cur->code.u.three.op1,left_reg,fp);
			load_reg(cur->code.u.three.op2,right_reg,fp);
			fprintf(fp,"add %s, %s, %s\n",_reg[result_reg].name,_reg[left_reg].name,_reg[right_reg].name);
			save_reg(cur->code.u.three.result,result_reg,fp);
			break;
		}
		case IN_SUB:{
			//To be done;
			int result_reg=8;//t0
			int left_reg=9;//t1
			int right_reg=10;//t2;
			load_reg(cur->code.u.three.op1,left_reg,fp);
			load_reg(cur->code.u.three.op2,right_reg,fp);
			fprintf(fp,"sub %s, %s, %s\n",_reg[result_reg].name,_reg[left_reg].name,_reg[right_reg].name);
			save_reg(cur->code.u.three.result,result_reg,fp);
			break;
		}
		case IN_MUL:{
			//To be done;
			int result_reg=8;//t0
			int left_reg=9;//t1
			int right_reg=10;//t2;
			load_reg(cur->code.u.three.op1,left_reg,fp);
			load_reg(cur->code.u.three.op2,right_reg,fp);
			fprintf(fp,"mul %s, %s, %s\n",_reg[result_reg].name,_reg[left_reg].name,_reg[right_reg].name);
			save_reg(cur->code.u.three.result,result_reg,fp);
			break;
		}
		case IN_DIV:{
			//To be done;
			int result_reg=8;//t0
			int left_reg=9;//t1
			int right_reg=10;//t2;
			load_reg(cur->code.u.three.op1,left_reg,fp);
			load_reg(cur->code.u.three.op2,right_reg,fp);
			fprintf(fp,"div %s, %s\n",_reg[left_reg].name,_reg[right_reg].name);
			fprintf(fp,"mflo %s\n",_reg[result_reg]);
			save_reg(cur->code.u.three.result,result_reg,fp);
			break;
		}
		case IN_IFGOTO:{

			break;
		}
	}
}

void trans_code(FILE *fp){
	struct Intercodes *temp=inter_head->next;
	while(temp!=inter_head){
			trans_one_code(fp,temp);
			temp=temp->next;
	}
}

void s_code_generate(FILE*fp){
	
	init_reg();
	init_data(fp);
	init_code(fp);
	trans_code(fp);
}


