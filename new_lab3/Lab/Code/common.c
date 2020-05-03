#include "common.h"
void tree_search(struct Node* cur,int depth){
  if(cur==NULL){
    return;
  }
  for(int i=0;i<depth;i++){
    printf("  ");
  }
    printf("%s",cur->name);
    if(cur->place==1){
      printf(" (%d)",cur->column);
    }
    else if(cur->place==0){
      if(cur->type==LEX_INT){
        printf(": %d",cur->int_contant);
      } 
      else if(cur->type==LEX_FLOAT){
        printf(": %f",cur->float_contant);
      }
      else if(cur->type==LEX_ID||cur->type==LEX_TYPE){
        printf(": %s",cur->string_contant);
      }else{
        ;
      }
    }
    printf("\n");
  tree_search(cur->child,depth+1);
    tree_search(cur->next_sib,depth);
}

