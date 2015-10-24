#include "DoublyLinkedList.h"

template <typename Data>
DoublyLinkedList<Data>::DoublyLinkedList()
{
	size = 0;
	head = nullptr;
	tail = nullptr;
}

template <typename Data>
DoublyLinkedList<Data>::~DoublyLinkedList()
{
	while(size > 0)
	{
		Node* temp = head;
		head = head->next;
		delete temp;
		size--;
	}
}

template <typename Data>
Data DoublyLinkedList<Data>::getHead()
{
	return head->data;
}

template <typename Data>
Data DoublyLinkedList<Data>::getTail()
{
	return tail->data;
}

template <typename Data>
int DoublyLinkedList<Data>::getSize()
{
	return size;
}

template <typename Data>
bool DoublyLinkedList<Data>::add(Data data)
{
	if(size<++size)
	{
		Node* node = new Node;
		node->data = data;
		node->previous = tail;
		node->next = nullptr;
		tail = node;
		return true;
	}
	return false;
}

template <typename Data>
bool DoublyLinkedList<Data>::addAt(Data data,int index)
{
	if(index < size && index > 0)
	{
		Node* node = new Node;
		node->data = data;

		//iterate from front or back
		if(index < size/2)
		{
			//set temp to point to the node after the new node
			Node* temp = head;
			for (int i = 0; i < index; ++i)
			{
				temp = temp->next;
			}
			//put the new node in the proper place
			node->next = temp;
			if(temp)
			{
				node->previous = temp->previous;
				temp->previous = node;
			}
			if(index == 0)
			{
				head = node;
			}
		}
		else
		{
			//set temp to point to the node before the new node
			Node* temp = tail;
			for (int i = size-1; i > index; --i)
			{
				temp = temp->previous;
			}
			//put the new node in the proper place
			node->next = temp;
			if(temp)
			{
				node->previous = temp->previous;
				temp->previous = node;
			}
			//is new node new tail
			if(index == size-1)
				tail = node;
		}
		size++;
	}
	return false;
}

template <typename Data>
Data DoublyLinkedList<Data>::getAt(int index)
{
	if(index < size && index > 0)
	{
		Node* node = new Node;

		//iterate from front or back
		if(index < size/2)
		{
			//set temp to point to the node after the new node
			Node* temp = head;
			for (int i = 0; i < index; ++i)
			{
				temp = temp->next;
			}
			//put the new node in the proper place
			node->next = temp;
			if(temp)
			{
				node->previous = temp->previous;
				temp->previous = node;
			}
			if(index == 0)
			{
				head = node;
			}
		}
		else
		{
			//set temp to point to the node before the new node
			Node* temp = tail;
			for (int i = size-1; i > index; --i)
			{
				temp = temp->previous;
			}
			//put the new node in the proper place
			node->next = temp;
			if(temp)
			{
				node->previous = temp->previous;
				temp->previous = node;
			}
			//is new node new tail
			if(index == size-1)
				tail = node;
		}
		size++;
	}
	return nullptr;
}