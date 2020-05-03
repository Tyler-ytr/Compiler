#ifndef TREE_H
#define TREE_H
#include <string.h>
#include <stdio.h>
#define _DEBUG_ 1

#if _DEBUG_
#define PR(...) printf(__VA_ARGS__)
#else
#define PR(...) 
#endif
typedef enum {
    LEX_INT=0,
    LEX_FLOAT=1,
    LEX_ID=2,
    LEX_TYPE=3,
    OTHERS
}NODE_TYPE;
struct Node{
    //参考结构 https://segmentfault.com/a/1190000002451356
    struct Node *child;//第一个孩子;
    struct Node* next_sib;//下一个兄弟节点
    char name[32];//节点名称
    union{
        int int_contant;
        float float_contant;
        char string_contant[32];//包含的内容
    };
    int place;//节点的类型,0表示lexical里面的,1表示syntax里面的;
    int column;//表示行数;
    NODE_TYPE type;// 0 表示int,1表示float,2表示ID,3表示type;
};
extern void tree_search(struct Node* cur,int depth);

#endif