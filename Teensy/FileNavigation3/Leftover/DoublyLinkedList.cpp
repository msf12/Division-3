#include "DoublyLinkedList.h"

#if DOUBLYLINKEDLIST_DEBUG
#include <iostream>
#endif

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
	while(size > 1)
	{
		Node* temp = head;
		head = head->next;
		delete temp;
	
		size--;
	}
	
	delete head;
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
	if(size<size+1)
	{

		Node* node = new Node;
		node->data = data;
		node->previous = tail;
		if(tail)
			tail->next = node;
		node->next = nullptr;
		tail = node;
		if(!size)
		{
			head = node;
		}
		size++;
		return true;
	}
	return false;
}

template <typename Data>
bool DoublyLinkedList<Data>::addAt(Data data,int index)
{
	// std::cout<<"Index is "<<index<<std::endl;
	// std::cout<<"Size is "<<size<<std::endl;
	// std::cout<<"Size/2 is "<<size/2<<std::endl;
	// std::cout<<"Index < size/2 is "<<(index<size/2)<<std::endl;
	if(!size && !index)
	{
		Node* node = new Node;
		node->data = data;
		node->previous = nullptr;
		node->next = nullptr;
		head = node;
		tail = node;
	}
	else if(index <= size && index >= 0)
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
			for (int i = size-1; i >= index; --i)
			{
				temp = temp->previous;
			}
			//put the new node in the proper place
			node->previous = temp;
			if(temp)
			{
				node->next = temp->next;
				temp->next = node;
			}
			//is new node new tail
			if(index == size-1)
				tail = node;
		}
		size++;
	}
	else
		return false;
	return true;
}

template <typename Data>
Data DoublyLinkedList<Data>::getAt(int index)
{
	if(index < size && index >= 0)
	{
		Node* nodeToReturn;

		//iterate from front or back
		if(index < size/2)
		{
			nodeToReturn = head;
			for (int i = 0; i < index; ++i)
			{
				nodeToReturn = nodeToReturn->next;
			}
			return nodeToReturn->data;
		}
		else
		{
			nodeToReturn = tail;
			for (int i = size-1; i > index; --i)
			{
				nodeToReturn = nodeToReturn->previous;
			}
			return nodeToReturn->data;
		}
	}
	else
		return nullptr;
}

template <typename Data>
void DoublyLinkedList<Data>::sort()
{
	sort(head,tail,size);
}

//Merge sort
template <typename Data>
void DoublyLinkedList<Data>::sort(Node*& node1, Node*& node2,int distance)
{
	if(node1 == node2)
	{
		return;
	}
	if(node1->next == node2)
	{
		if(node1->data < node2->data)
			return;

		node1->next = node2->next;
		node2->previous = node1->previous;
		node1->previous = node2;
		node2->next = node1;
		node1 = node2;
		node2 = node1->next;

		return;
	}
	Node* center = node1;
	Node* centerNext;
	for(int i = 0; i<(distance-1)/2; ++i)
	{
		center = center->next;
	}
	centerNext = center->next;
	center->next = nullptr;
	sort(node1,center,distance/2);
	sort(centerNext,node2,(distance-1)/2);
	node1 = merge(node1,centerNext);
}

template <typename Data>
typename DoublyLinkedList<Data>::Node* DoublyLinkedList<Data>::merge(Node* head1, Node* head2)
{
	if(!head1 || head1==head2)
	{
		return head2;
	}
	if(!head2)
	{
		return head1;
	}
	if(head1->data < head2->data)
	{
		head1->next = merge(head1->next,head2);
		return head1;
	}
	else
	{
		head2->next = merge(head1,head2->next);
		return head2;
	}
}

#if DOUBLYLINKEDLIST_DEBUG

template <typename Data>
void DoublyLinkedList<Data>::printList()
{
	Node* temp = head;
	for (int i = 0; i < size; ++i)
	{
		std::cout << temp->data << std::endl;
		temp = temp->next;
	}
	std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
	DoublyLinkedList<int> d;
	d.add(2);
	d.add(1);
	d.addAt(2,0);
	d.add(20);
	d.add(0);
	d.add(752);
	d.add(1);
	d.add(2);
	d.add(5);
	d.add(16);
	d.add(-28);
	d.add(41);
	d.add(5);
	d.add(7);
	d.add(3);
	d.sort();
	d.printList();
	return 0;
}

#endif