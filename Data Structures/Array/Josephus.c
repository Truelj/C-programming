/* To represent people arranged in a circle, we build a circular linked list, */
/* with a link from each person to the person on the left in the circle. */

#include <stdio.h>
#include <stdlib.h>

typedef struct node* link;
struct node {int item; link next;};

int main(int argc, char *argv[]){
	int N = atoi(argv[1]), M = atoi(argv[2]);
	//creat the head of the link
	link t = malloc(sizeof(link)), x = t;
	t->item = 1; t->next = t;
	//creat the circular list
	for(int i = 2; i <= N; i++){
		x = x->next = malloc(sizeof(link));
		x->item = i; 
		x->next = t;
	}
	//now t links to the frist node, x links to the last node
	while( x != x->next){
		for(int i = 1 ; i < M; i++){//skip M-1 nodes
			x = x->next;
		}
		printf("%d\n",x->next->item);
		x->next = x->next->next;
	}
	printf("The president is %d: \n", x->item);
	return 0;

}