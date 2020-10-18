#include<stdio.h>
#include<stdlib.h>
#include "memory.h"
#include "bst.h"
#include "llist.h"
#include "my_alloc.h"

extern bst* avail_mem;

/* memory_new
 * create a new memory struct, initialze its address and size
 */
memory* memory_new(void *addr, unsigned int size){
  memory* m = (memory*)malloc(sizeof(memory));
  m->addr = addr;
  m->size = size;
  return m;
}

/* free the dynamically allocated memory struct
 */
void memory_free(void* p){
  memory* m = (memory*)p;
  free(m);
}

/* compare two memory variables x and y by address
 * if x is less than y, return -1
 * if x is greater than y, return 1
 * if they are equal, return 0
 */
int memory_addr_cmp(const void* x, const void* y)
{
        memory *x1=(memory*)x;
        memory *y1=(memory*)y;
        if(x1->addr<y1->addr)
        return -1;
        else if(x1->addr>y1->addr)
        return 1;
        else
        return 0;
}

/* compare two memory variables x and y by size
 * if x is less than y, return -1
 * if x is greater than y, return 1
 * if they are equal, return 0
 */
int memory_size_cmp(const void* x, const void* y)
{
        memory *x1=(memory*)x;
        memory *y1=(memory*)y;
        if(x1->size < y1->size)
        return -1;
        else if (x1->size > y1->size)
        return 1;
        else
        return 0;
}

/* print the memory address and size
 */
void memory_print(void* data){
  if (data == NULL) return;
  memory* m = (memory*)data;
  printf("address: %d, size: %u\n", (int)m->addr, m->size);
}


memory *allocate_memory_page()
{
	memory *ret_val=(memory*)malloc(sizeof(memory));
	void *m_add=malloc(4096);
	ret_val->addr=m_add;
	ret_val->size=4088;
	return ret_val;
}

/*split_memory()
- to split a chunk of memory into two - wrt the size desired
- input: a memory struct
- output: void ptr to the usable mem after split
*/
void *split_memory(memory* data, unsigned int size_desired)
{
	//use a temp variable to access data of the memory struct
	memory *tmp=data;
	char* add=(void*)tmp->addr;
	data->size-=(size_desired+8);
	add+=(8+(data->size));
	*((int*)add)=size_desired;
	void *ret_val=(add+8);
	return ret_val;
}


memory *merge_memory(memory *first, memory *second)
{
	int s1=first->size;
	int s2=second->size;
	//ptr to usable mem of first
	char *tmp1=first->addr;
	void *ptr1; //first_size
	void *end1; //mem1_end_addr
	//move ptr back by 8 to get ptr to place where size is stored
	tmp1-=8;
	//make it void again
	ptr1=(void*)tmp1;
	//move ptr to end of first
	tmp1+=(s1+8);

	//move to add of second
	tmp1+=8;
	void *check=(void*)tmp1;
	void *tmp2=second->addr;
	if((int)check==(int)tmp2)
	{
		free(second);
		first->size+=(s2+8);
		*((int*)ptr1)=first->size;
		return first;
	}
	else
		return NULL;
}
