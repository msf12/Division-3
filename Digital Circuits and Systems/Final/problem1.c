#include <stdio.h>
#include <stdlib.h>

int table1[] = { 1, 10, 2, 20, 5, 15, 7, 35, 11, 9 };
int N1       = 10;
int table2[] = { 10, 1, 20, 2, 50, 15, 35, 7, 11, 9, 22, 21 };
int N2       = 12;
int table3[] = { };
int N3       = 0;
int table4[] = { 10, 1, 20, 2, 1, 10, 2, 20, 5, 15, 7, 35, 11, 9, 50, 15, 35, 7, 11, 9, 22, 21 };
int N4       = 22;


typedef struct node {
	int  item1;
	int  item2;
	struct node *next;
} Node;

Node* addNode( int x, int y, Node* List ) {
	Node *p = (Node *) malloc( sizeof( Node ) );
	p->item1 = x;
	p->item2 = y;
	p->next = List;
	return p;
}

void display( Node* List ) {
	Node *p = List;
	printf( "List = " );
	while (  p!= NULL ) {
		printf( "(%d, %d) ", p->item1, p->item2 );
		p = p->next;
	}
	printf( "\n\n" );
}

Node* removePair(Node* list)
{
	if(list == NULL)
	{
		return NULL;
	}
	if(list->item1 > list->item2)
	{
		return removePair(list->next);
	}
	list->next = removePair(list->next);
	return list;
}

int main() {
	Node *List = NULL;
	//--- define an array (equal to table1, or table2, or table3)---
	int*  table = table1;  // array
	int   N     = N1;      // dimension of array


	//--- create a linked-list of pairs of ints taken ---
	//--- from table.                                 ---
	for ( int i=0; i<N; i += 2 ) 
		List = addNode( table[i], table[i+1], List );
	//--- display the list ---
	display( List );
	List = removePair(List);
	//--- display the list ---
	display( List );

	return 0;
}