#include <stdio.h>
#include <stdlib.h>
#define vim_ok 
struct aff{
	int *bd;
};
int init(int **a);
int request(struct aff*a);
int main(){
#if (!defined(vim_ok))
	printf("ok\n");
#endif
#if (!defined(vim_ok1))
	printf("ok1\n");
#endif
	//int a_arr[10] = {0};
	//printf("slkl = 0x%x\n", slkl);
	//printf("&slkl = 0x%x\n", &slkl);
	//int (*aim)() = slkl;
	//printf("aim = slkl = 0x%x\n", aim);
	//aim = &slkl;
	//printf("aim = &slkl = 0x%x\n", aim);
	//printf("a_arr =  0x%x\n", a_arr);
	//printf("&a_arr =  0x%x\n", &a_arr);
	struct aff* aff_t = (struct aff*)malloc(sizeof(struct aff));
	int *am =NULL;
	init(&am);
	aff_t->bd = am;
	request(aff_t);
	printf("init_test:active aff->bd = %p\n", aff_t->bd);
}

