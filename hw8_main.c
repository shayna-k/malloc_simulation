#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bst.h"
#include "memory.h"
#include "my_alloc.h"
#include "llist.h"

extern bst *avail_mem;
extern bst *avail_mem_add;

int main()
{
	printf("\nTesting split_memory(): \n"); 	

	memory *mem1=allocate_memory_page();
	void *split1=split_memory(mem1,20);
	char *csplit1=split1;
	csplit1-=8;
	int *rsplit1=(int*)csplit1;
	printf("split1 size [20] : %d \n",*rsplit1);

	void *split2=split_memory(mem1,70);
	char *csplit2=split2;
	csplit2-=8;
	int *rsplit2=(int*)csplit2;
	printf("split size [70] : %d \n",*rsplit2);

	void *split3=split_memory(mem1,0);
        char *csplit3=split3;
        csplit3-=8;
        int *rsplit3=(int*)csplit3;
        printf("split size [0] : %d \n",*rsplit3);


	printf("\nTesting my_malloc(): \n");
	avail_mem=NULL;
	avail_mem_add=NULL;

	void *alloc1=my_malloc(48);	
	char *calloc1=alloc1;
	calloc1-=8;
	int *ralloc1=(int*)calloc1;
	printf("alloc1 size [48] : %d \n",*ralloc1);

	printf("\n");

	void *alloc2=my_malloc(24);
	char *calloc2=alloc2;
	calloc2-=8;
	int *ralloc2=(int*)calloc2;
	printf("alloc2 size [24] : %d \n",*ralloc2);

	printf("\n");

	void *alloc4=my_malloc(56);
        char *calloc4=alloc4;
        calloc4-=8;
        int *ralloc4=(int*)calloc4;
        printf("alloc4 size [2560] : %d \n",*ralloc4);

        printf("\n");


	void *alloc5=my_malloc(1024);
        char *calloc5=alloc5;
        calloc5-=8;
        int *ralloc5=(int*)calloc5;
        printf("alloc5 size [1024] : %d \n",*ralloc5);

        printf("\n");


	void *alloc6=my_malloc(72);
	char *calloc6=alloc6;
	calloc6-=8;
	int *ralloc6=(int*)calloc6;
	printf("alloc6 size [72] : %d \n",*ralloc6);

        printf("\n");


	my_free(alloc1);

	my_free(alloc2);

	my_free(alloc4);

	my_free(alloc5);

	my_free(alloc6);


	printf("\nTesting compact_memory(): \n");
	compact_memory();
	printf("\n");
	print_memory();
	printf("\n");

	return 0;
}