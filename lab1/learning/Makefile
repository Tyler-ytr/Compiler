#参考了makefile中文版手册以及CSDN
# 编译器以及编译选项
CC=gcc
FLEX=flex


# 编译目标：该code目录下所有的.c文件 通过shell命令实现
CFILES=$(shell find ./ -name "*.c")
OBJS=$(CFILES:.c=.o)
LFILE=$(shell find ./ -name "*.l")
YFILE=$(shell find ./ -name "*.y")
# 使用sed s命令进行替换，参考https://blog.csdn.net/u013991521/article/details/80531019
LFC= $(shell find ./ -name "*.l" | sed s/[^/]*\\.l/lex.yy.c/)


