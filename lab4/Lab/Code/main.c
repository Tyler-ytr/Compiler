#include <stdio.h>
#include "syntax.tab.h"
#include "common.h"
#include "semantic.h"
#include "imediate.h"
#include "simulater.h"
extern FILE*yyin;
//#define YYDEBUG 1

extern int yydebug;
extern int lexError;
extern int syntaxError;
extern struct Node* root;
extern int yyrestart(FILE*);
extern int yyparse();
extern int success_g;
int yylex();
int main(int argc,char**argv){
    
if (argc <= 1) return 1;
FILE* f = fopen(argv[1], "r");
if (!f)
{
perror(argv[1]);
	return 1;
}
//printf("%s",argv[2]);
FILE* fp=fopen(argv[2],"wt+");
if(!fp){
    perror(argv[2]);
    return 1;
}
yyrestart(f);
//yydebug=1;
yyparse();



//printf("%s\n",root->name);
if(lexError==0&&syntaxError==0){
//  printf("l: %d,\ts:%d\n",lexError,syntaxError);
//tree_search(root,0);
semantic_check(root);
//printf("success_g:%d\n",success_g);
intermediate_generate(root,fp);
s_code_generate(fp);
}
return 0;
/*
    if(argc>1){
        if(!(yyin=fopen(argv[1],"r"))){
            perror(argv[1]);
            return 1;
        }
    }
while (yylex() != 0);
    return 0;
*/

};