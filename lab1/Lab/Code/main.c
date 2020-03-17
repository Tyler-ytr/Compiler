#include <stdio.h>
#include "syntax.tab.h"
extern FILE*yyin;
#define YYDEBUG 1
extern int yydebug;
extern struct Node* root;
int yylex();
int main(int argc,char**argv){
    
if (argc <= 1) return 1;
FILE* f = fopen(argv[1], "r");
if (!f)
{
perror(argv[1]);
	return 1;
}
yyrestart(f);
//yydebug=1;
yyparse();

//printf("%s\n",root->name);
tree_search(root,0);
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