#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_DEBUG 0

#include <stdint.h>

template <typename Data>
class DoublyLinkedList
{
	struct Node
	{
		Node* previous;
		Node* next;
		Data data;
	};

	Node* head;
	Node* tail;
	uint32_t size;

	void sort(Node*& n1, Node*& n2,int distance);
	Node* merge(Node* head1, Node* head2);

public:
	DoublyLinkedList();
	~DoublyLinkedList();
	
	Data getHead();
	Data getTail();
	int getSize();
	bool add(Data data);
	bool addAt(Data data,int index);
	Data getAt(int index);
	void sort();

#if DOUBLYLINKEDLIST_DEBUG
	void printList();
#endif
};


#endif