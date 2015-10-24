#include "DoublyLinkedList.h"

struct Node;

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
	if(!size && !index)
	{
		Node* node = new Node;
		node->data = data;
		node->previous = nullptr;
		node->next = nullptr;
		head = node;
		tail = node;
	}
	else if(index < size && index >= 0)
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
	if(size < 2)
		return;
	if(size == 2)
	{
		if(head->data > tail->data)
			return;
		head->next = nullptr;
		head->previous = tail;
		tail->next = head;
		tail->previous = nullptr;
		head = tail;
		tail = head->next;
		return;
	}
	Node* center = head;
	for(int i = 0; i < size/2; ++i)
	{
		center = center->next;
	}
	sort(head,center);
	sort(center->next,tail);
}

template <typename Data>
void DoublyLinkedList<Data>::sort(Node* head, Node* tail)
{
	if(head == tail)
		return;
	if(head->next == tail)
	{
		if(head->data > tail->data)
			return;
		head->next = nullptr;
		head->previous = tail;
		tail->next = head;
		tail->previous = nullptr;
		head = tail;
		tail = head->next;
		return;
	}
	Node* center = head;
	for(int i = 0; i < size/2; ++i)
	{
		center = center->next;
	}
	sort(head,center);
	sort(center->next,tail);
	merge(head,center->next);
}

template <typename Data>
Node* DoublyLinkedList<Data>::merge(Node* head1, Node* head2)
{
	if(!head)
		return head2;
	if(!tail)
		return head1;
	if(head1->data > head2->data)
	{
		head1->next = merge(head1->next,head2);
		return head1;
	}
	else
	{
		head2->next = merge(head2->next,head1);
		return head2;
	}
}

int main(int argc, char const *argv[])
{
	DoublyLinkedList<int> d;
	d.add(1);
	d.add(3);
	d.addAt(2,1);
	d.add(0);
	d.add(20);
	d.sort();
	return 0;
}