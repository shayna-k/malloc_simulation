#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "memory.h"
#include "llist.h"
#include "my_alloc.h"

/* Global variables
 * By declaring these outside of a function:
 *   Scope: Every function inside of this file may use them. 
 *   		No functions in other files may use them.
 *   Lifetime: They exist for the entire program - they are created
 *		when the program begins and exist until the program
 *		ends.
 */   		

/* This is the tree that holds the available memory. */
bst *avail_mem = NULL;
bst *avail_mem_add = NULL;

/* This includes all of the functions for the memory allocator.
 * The last two functions (my_malloc and my_free) are public 
 * functions with prototypes in the header function. The rest of the
 * functions are helper functions only called by my_malloc and 
 * my_free. You must implement these helper functions. If you wish,
 * you may add helper functions we did not define.
 */

llist *make_llist()
{
	llist *l=create_llist();
	void *x;
	for(x=bst_iterate(avail_mem_add);x!=NULL;x=bst_iterate(NULL))
	{
		insert_tail(l,x);
	}
	return l;
}


/* compact_memory
 * Search through all available memory and attempt to merge memory
 * that is stored next to each other.
 * This uses global variable avail_mem, so it does not need any
 * input parameters. Look at the bst functions and memory functions.
 */
void compact_memory()
{
	llist *l=make_llist();
	llist_node *tmp=l->head;
	memory *res;
	while(tmp->next!=NULL)
	{
		res=merge_memory(tmp->item,tmp->next->item);
		if(res==NULL)
			tmp=tmp->next;
		else
		{
			llist_node *temp_node=tmp->next;
			tmp->next=tmp->next->next;
			bst_delete(avail_mem,temp_node->item);
			bst_delete(avail_mem_add,temp_node->item);
			free(temp_node);
		}
	}
}

/* print_available_memory 
 *
 * Print out all available memory in ascending order by size.
 */
void print_memory()
{
	printf("printing avail_mem: \n");
	bst_inorder_traversal(avail_mem,memory_print);
	printf("printing avail_mem_add: \n");
	bst_inorder_traversal(avail_mem_add,memory_print);
}

/* init_alloc
 *
 * Initializes the data structures. This initializes avail_mem so 
 * that, instead of being a NULL pointer, it points to a valid bst
 * struct whose root pointer is NULL.
 */
void init_alloc()
{
	avail_mem=bst_new(memory_size_cmp);
	avail_mem_add=bst_new(memory_addr_cmp);
}

/* my_malloc
 * 
 * function that finds a piece of available memory that is at least
 * num_bytes size. A pointer to the beginning of the usable piece of
 * that chunk is returned.
 */
void *my_malloc(int num_bytes)
{
	if(num_bytes<24)
		num_bytes=24;
	if(num_bytes%8!=0)
		num_bytes+=(8-(num_bytes%8));
	if(avail_mem==NULL)
	{
		memory *new_mem=allocate_memory_page();
		init_alloc();
		avail_mem->root=node_new(new_mem);
		avail_mem_add->root=node_new(new_mem);
	}
	//tmp mem struct to call bst_t/s with
	memory* tmp=memory_new(NULL,num_bytes);
	void *tmp_new=(void*)tmp;
	//usable memory = res
	void *res=bst_item_or_successor(avail_mem,tmp_new);

	//check if res is null - compact memory
	if(res==NULL)
	{
		compact_memory();
		res=bst_item_or_successor(avail_mem,tmp_new);
		//check if res is still null - could not be compacted
		//need to create a new page
		if(res==NULL)
		{
			memory *mem_new=allocate_memory_page();
			bst_insert(avail_mem_add,mem_new);
			bst_insert(avail_mem,mem_new);
			res=bst_item_or_successor(avail_mem,tmp);
		}
	}
	//cast res to mem struct
	memory *res_mem=(memory*)res;
	//size of the amount of memory res points to
	int res_size=res_mem->size;
	printf("res_size: %d\n",res_size);
	//delete from the tree whatever the fn found

	bst_delete(avail_mem,res);
	bst_delete(avail_mem_add,res);

	//if the size of the thing it found is > 2x of what u want
	if(res_size>=(2*num_bytes))
	{
		//ret_res points to usable mem after the split
		void *ret_res=split_memory(res_mem,num_bytes);
		printf("after split : %d \n",res_mem->size);

		//put the remainder back into the tree
		bst_insert(avail_mem,res_mem);
		bst_insert(avail_mem_add,res_mem);

		return ret_res;
	}
	else
		return res_mem->addr;
}

/* my_free
 * 
 * Function that returns the memory chunk whose usable piece starts
 * at that address back to the set of available memory so that it can
 * be reused in a subsequent free call
 */
void my_free(void *address)
{
	void *add=address;
	long ptr=(long)add;
	ptr-=8;
	add=(void*)ptr;
	//deref what add holds at the place it points to
	int size=*(int*)add;
	ptr+=8;
	add=(void*)ptr;
	memory *m=memory_new(add,size);
	if(size!=0)
	{
		bst_insert(avail_mem,m);
		bst_insert(avail_mem_add,m);
	}
}