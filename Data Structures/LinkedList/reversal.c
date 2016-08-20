//given a link to the first node of a linked list, return the same list but in reverse order

#include <stdio.h>
#include <stdlib.h>
//list node type defination
typedef struct node* link;
struct node{
	int item;
	link next;
};

//function prototype
//1 -> 2 -> 3 -> null
link reverse_1(link);
link reverse_2(link);
link create_1(int);
link create_2(int);
void display(link);
void reverse(link);
void sort(link);
int main(int argc, char* argv[]){
	//get the number of ndoes to create 
	int number = atoi(argv[1]);
	//creat the list
	link a =create_2(number);
	//display the list
	display(a);

	//create a dumb node 
	link dumb = malloc(sizeof(link));
	dumb->next = a;

	//reverse the list
	reverse(dumb);
	display(dumb->next); //display the list

	//sort the list
	sort(dumb);
	display(dumb->next); //display the list

}
//create a linked list with null tail
//declare 3 pointers 
link create_1(int number){
	if(!number){
		return NULL;
	}
	//create the firste node
	link a = malloc(sizeof(link));
	a->item = rand() % number;
	link b = a;
	for(int i = 1; i < number; i++){
		//create a node
		link c = malloc(sizeof(link));
		c->item = rand() % number;
		c->next = NULL;
		b->next = c;
		b = c;	
	}
	return a;
}

//declare 2 pointers
link create_2(int number){
	if(!number){
		return NULL;
	}
	//create the firste node
	link a = malloc(sizeof(link));
	a->item = rand() % number;
	link b = a;
	for(int i = 1; i < number; i++){
		//create a node
		b->next = malloc(sizeof(link));
		b = b->next;
		b->item = rand() % number;
		b->next = NULL;
	}
	return a;
}

void display(link a){
	//travese the list
	while(a != NULL){
		//print the value of each node
		printf("%d ",a->item);
		a=a->next;
	}
	printf("\n");
}
//method 1
link reverse_1(link a){
	link b;
	if(a!=NULL) b = a->next;
	link r = a, t;
	while(b != NULL){
		//reverse a and b
		t = b->next;
		b->next = r;
		a->next = t;
		r = b;
		b = t;
	}
	return r;
}
//method 2
link reverse_2(link a){
	link t, b = a, r = NULL;
	while(b!=NULL){
		t=b->next;
		b->next=r;
		r=b;
		b=t;
	}
	return r;
}
//method 3
//a is a dumb node. a->next is the first node of the list
void reverse(link a){
	a->next = reverse_2(a->next);
}

//a is a dumb node. a->next is the first node of the list
void sort(link a){
	link first = a->next;
	link next;
	//create another linked list to store the sorted node
	link b = malloc(sizeof(link));
	b->next = NULL;
	link i;
	while(first != NULL){
		next = first;
		first = first->next;
		//add node "next" to the new list
		
		for(i = b; i->next != NULL; i = i->next){
			//compare the node after i with node "next"
			if(i->next->item > next->item){

				break;
			}
		}
		//add node "next" after node i
		next->next = i->next;
		i->next = next;	
	}
	a->next = b->next;
}









