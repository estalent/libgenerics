#include <stdio.h>
#include <time.h>
#include <generics/red_black_tree.h>

#define N 10

void print_rbt (rbnode_t* node, int t)
{
	if(node != NULL){
		int i;
		for( i = 0; i<t; i++ ){
			printf("|\t");
		}
		printf("%d (%s)\n", *((int*)node->data), (node->color == G_RB_BLACK)?"black":"red");
		
		print_rbt(node->left, t+1);
		print_rbt(node->right, t+1);
	}else{
		int i;
		for( i = 0; i<t; i++ ){
			printf("|\t");
		}
		printf("null\n");
	}
}

int is_a_valid_rbtree(rbnode_t* node, int n, int* count)
{

	if(node){
		int im_black = node->color == G_RB_BLACK;
		n += im_black;

		if(!is_a_valid_rbtree(node->left, n, count))
			return 0;

		if(!is_a_valid_rbtree(node->right, n, count))
			return 0;

		return 1;
	}

	n++;
	if(*count == 0){
		*count = n;
	}

	return n == *count;
}

void swap(int* a, int* b)
{
	int cup = *a;
	*a = *b;
	*b = cup;
}

int main()
{

	srand(time(NULL));

	rbtree_t rbt;

	rbtree_create(&rbt, sizeof(int));

	int i;

	/*
	 * insert
	 */
	int to_insert[N];
	for( i = 0; i<N; i++ )
		to_insert[i] = i;
	for( i = 0; i<N; i++ )
		swap(&(to_insert[i]), &(to_insert[rand()%N]));
	for( i = 0; i<N; i++ )
		printf("%d ", to_insert[i]);
	printf("\n");

	for( i = 0; i<N; i++ ){
		rbtree_add( &rbt, &(to_insert[i]) );
	}
	print_rbt ( rbt.root, 0 );

	/*
	 * search functions
	 */
	int n;
	rbnode_t* node;
	n = rand()%N;

	gerror_t g;
	printf("finding %d\n", n);
	if(( g = rbtree_find_node(&rbt, &n, &node) ) == GERROR_OK){
		printf("%d found in node addr [%p]\n", n, node);
		printf("color: %s\n", (node->color == G_RB_RED)?
							"red":"black");
		if(node->parent) printf("father: %d\n", *(int*)node->parent->data);
		else printf("father: null\n");
	}else{
		printf("%s\n", gerror_to_str(g));
	}

	printf("min value: ");
	if(( g = rbtree_min_value(&rbt, &n) ) == GERROR_OK)
		printf("%d\n", n);
	else
		printf("%s\n", gerror_to_str(g));

	printf("max value: ");
	if(( g = rbtree_max_value(&rbt, &n) ) == GERROR_OK)
		printf("%d\n", n);
	else
		printf("%s\n", gerror_to_str(g));

	/*
	 * verify tree
	 */
	n = 0;
	printf("It %s a valid red-black tree\n", (is_a_valid_rbtree(rbt.root, 0, &n))?"is":"is not");

	rbtree_destroy( &rbt );

	return 0;
}
