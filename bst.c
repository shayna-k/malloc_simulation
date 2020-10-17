#include<stdio.h>
#include<stdlib.h>
#include "bst.h"
#include "llist.h"
#include "memory.h"
#include "my_alloc.h"
//#include "my_alloc.c" 

extern bst* avail_mem;
llist *list;

/* malloc a new node and assign the data
 * pointer to its data field
 */
node* node_new(void* data)
{       
        node *new_node=(node*)malloc(sizeof(node));
        new_node->data= data;
        new_node->left=NULL;
        new_node->right=NULL;
        return new_node;
}

/* create a new bst, initialize its root to be NULL
 */
bst* bst_new(int (*cmp)(const void* x, const void* y))
{       
        bst *x=(bst*)malloc(sizeof(bst));
        x->root=NULL;
	x->cmp=cmp; 
	return x;
}

/* Insert a node to to a subtree with a root node as parameter
 * Insertion is in sorted order. 
 * Return the new root of the modified subtree.  
 */
node* node_insert(node* root, void* data,
    int (*cmp)(const void* x, const void* y))
{       
        if(root==0)
	{
                node *n=node_new(data);
                return n;
        }
        int c=(*cmp)(root->data,data);
        if(c<0)
                root->right=node_insert(root->right,data,cmp);
        else
                root->left=node_insert(root->left,data,cmp);
        return root;
}

/* Insert a new node to the bst
 */
void bst_insert(bst* b, void* data)
{
        b->root=node_insert(b->root, data, b->cmp);
}


//helper to find max val in a subtree
node* find_max(node *root)
{
        if(root==NULL)
                return NULL;
        else if(root->right!=NULL)
                return find_max(root->right);
        return root;
}


/* delete a node from a subtree with a given root node
 * use the comparison function to search the node and delete 
 * it when a matching node is found. This function only
 * deletes the first occurrence of the node, i.e, if multiple 
 * nodes contain the data we are looking for, only the first node 
 * we found is deleted. 
 * Return the new root node after deletion.
 */
node* node_delete(node* root, void* data,
    int (*cmp)(const void* x, const void* y))
{
        if(root==NULL)
                return NULL;
        int c=(*cmp)(data,root->data);
        if(c<0)
                root->left=node_delete(root->left,data,cmp);
        else if(c>0)
                root->right=node_delete(root->right,data,cmp);
        else
        {
                //0 children
                if((root->left==NULL)&&(root->right==NULL))
                {
                        free(root);
                        return NULL;
                }
                //1 child
                else if((root->left==NULL)||(root->right==NULL))
                {
                        node *tmp;
                        if(root->left==NULL)
                                tmp=root->right;
                        else
                                tmp=root->left;
                        free(root);
                        return tmp;
                }
                //2 children
                else
                {
                        node *tmp=find_max(root->left);
                        root->data=tmp->data;
                        //free(tmp);//=NULL;
                        root->left = node_delete(root->left,tmp->data,cmp);

                }
        }
	return root;
}

/* delete a node containing data from the bst
 */
void bst_delete(bst* b, void* data)
{
        b->root=node_delete(b->root,data,b->cmp);
}

/* Search for a node containing data in a subtree with
 * a given root node. Use recursion to search that node. 
 * Return the first occurrence of node. 
 */
void* node_search(node* root, void* data,
    int (*cmp)(const void* x, const void* y))
{
        if(root==NULL)// || data==root->data)
                return NULL;
        int c=cmp(data,root->data);
        if(c == 0)
                return root->data;
        else if(c<0)
                return node_search(root->left,data,cmp);
        else
                return node_search(root->right,data,cmp);
        return NULL;
}


/* Search a node with data in a bst. 
 */
void* bst_search(bst* b, void* data)
{
        if(b->root==NULL)
        {
                printf("node_search: ERROR: empty tree");
                return NULL;
        }
        void *x=node_search(b->root,data,b->cmp);
        if(x == NULL)
        {
                printf("NOT FOUND\n");
        }
        return x;
}

/* traverse a subtree with root in ascending order. 
 * Apply func to the data of each node. 
 */
void inorder_traversal(node* root, void(*func)(void* data))
{
        if(root!=NULL)
        {
                inorder_traversal(root->left,func);
                func(root->data);
                inorder_traversal(root->right,func);
        }
	return;
}

/* traverse a bst in ascending order. 
 * Apply func to the data of each node. 
 */
void bst_inorder_traversal(bst* b, void(*func)(void* data))
{
        inorder_traversal(b->root,func);
}


// a helper function to create linked list from bst by in order traversal
void inorder_traversal_insert_llist(node* root, llist* l)
{
	//printf("entered helper\n");
	if(root==NULL)
		return;
	//printf("root!=NULL woohoo\n");
	//memory *m;
	inorder_traversal_insert_llist(root->left,l);
	insert_tail(l, root->data);
	//m=root->data;
	//printf("\ndata in item of root: %d\n",m->size);
	inorder_traversal_insert_llist(root->right,l);
}


/* an iterator to iterate through the bst in ascending order
 */
void *bst_iterate(bst *b)
{
	static llist *l=NULL;
	//static llist_node *node=NULL;
	if(b!=NULL)
	{
		l=create_llist();
		inorder_traversal_insert_llist(b->root,l);
	}
	if(l->head==NULL)
		return NULL;
	void *item=l->head->item;
	l->head=l->head->next;
	return item;
}



void free_node(node *r)
{
        if(r==NULL)
        return;
        free_node(r->left);
        free_node(r->right);
        free(r);
}


/* free the bst with 
 */
void bst_free(bst* b)
{
        free_node(b->root);
}


/*****************************
 *    HOMEWORK 7
 *****************************/
/* 
 * item_or_successor
 * 
 * find an item that is equal or, if there isn't one that is 
 * equal, find the one that is next larger to the requested
 * item.
 *
 * The purpose of this function is that, given a size of a 
 * chunk of memory desired, it attempts to find an available
 * chunk of memory the same size. If there is no chunk of 
 * memory the same size, it finds the chunk of memory that is 
 * celseto that size but larger.
 *
 * It returns a pointer to the memory struct. The compare 
 * function in the bst is the one that performs the comparisons.
 */
void* node_item_or_successor(node *n, void *item,
        int (*cmp)(const void* x, const void* y))
{
	//****ITEM IS A MEMORY FuCKING STRUCT****
	
	//if the node is null, make new mem page and new node
	if(n==NULL)
	{
		memory *new_mem=allocate_memory_page();
		n=node_new(new_mem);
	}
	//comparing memory structs
	int c=cmp(item,n->data);
	if(c==0)
		return n->data;
	else if(c<0)
	{
		if(n->left==NULL)
			return n->data;
		else
		{
			if(cmp(item,n->left->data)>0)
				return n->data;
			else
				return node_item_or_successor(n->left,item,cmp);
		}
	}
	else 
	{
		if(n->right==NULL)
			return NULL;
		return node_item_or_successor(n->right,item,cmp);
	}
}


void* bst_item_or_successor(bst *b, void *item)
{
	return node_item_or_successor(b->root,item,b->cmp);
}
