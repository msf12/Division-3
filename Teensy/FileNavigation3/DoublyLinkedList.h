#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

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

public:
	DoublyLinkedList();
	~DoublyLinkedList();
	
	Data getHead();
	Data getTail();
	int getSize();
	bool add(Data data);
	bool addAt(Data data,int index);
	Data getAt(int index);
};


#endif