// #include "symbol_table.h"
// struct Symbol_bucket* global_scope=NULL;




// int main(){
// 	global_scope=init_symboltable();
// 	struct Symbol_bucket*temp1=enter_scope();
// 	struct Symbol_bucket*temp2=enter_scope();
// 	struct Symbol_bucket*temp3=enter_scope();
// //	struct Symbol_bucket*temp4=exit_scope();
// 	struct Symbol_node test1;
// 	struct Symbol_node* test2=(struct Symbol_node*)malloc(sizeof(struct Symbol_node));
// 	struct Symbol_node* test3=(struct Symbol_node*)malloc(sizeof(struct Symbol_node));
// 	strcpy(test1.field.name,"test1");
// 	strcpy(test2->field.name,"test2");
// 	strcpy(test3->field.name,"test2");
// 	test1.kind=VARIABLE;
// 	test2->kind=STRUCT_NAME;
// 	test3->kind=STRUCT_NAME;
// 	test1.ifdef=1;
// 	test2->ifdef=0;
// 	test3->ifdef=0;
// 	test1.lnext=NULL;
// 	test1.cnext=NULL;
// 	test2->lnext=NULL;
// 	test2->cnext=NULL;
// 	test3->lnext=NULL;
// 	test3->cnext=NULL;
// 	printf("%s",test2->field.name);
// 	if(global_scope->next->next->next->next!=NULL){
// 		printf("heresdsdsd");
// 	}
// 	insert_symbol2(test3,global_scope);
// 	insert_symbol2(test2,temp3);
// //	insert_symbol2(test2,temp3);
// 	printf("here\n");
// 	show_global_table();
// 	show_scope_table();
// 	struct Symbol_bucket*temp4=exit_scope();
// 	show_scope_table();	
// 	show_global_table();
// 	printf("here2\n");
// 	// struct Type_ temp;
// 	// temp.kind=BASIC;
// 	// Type ttemp=&temp;
// 	// char* name="temp";
// 	// int def=1;
// 	// Type qtemp=malloc(sizeof(struct Type_));


// 	// int qdef=0;
// 	// insert_symbol(ttemp,name,def);
// 	// query_symbol(&qtemp,"temp",&qdef);
// 	// query_symbol(&qtemp,"temp1",&qdef);
// 	return 0;





// }