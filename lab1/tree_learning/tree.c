#include <stdio.h>
/*typedef strcut Node{
    node *child;
    node *nextsil;//兄弟
    char name[32];//节点名称
    char contant[128];//包含的内容
}node;
*/

int main(){
	
int a;
int b;
float c;
sscanf("0xab","%x",&a);
sscanf("015","%o",&b);
sscanf("1.001","%f",&c);
printf("%d\t %x\n",a,a);//16进制
printf("%d\t %o\n",b,b);//8进制
printf("%f",c);//8进制
;return 0;
}
