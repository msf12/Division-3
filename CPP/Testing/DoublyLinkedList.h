#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

template <typename Data>
class DoublyLinkedList
{
	//TODO: add comparision operators to Node simplify sorting code
	struct Node
	{
		Node* previous;
		Node* next;
		Data data;
	};

	Node* head;
	Node* tail;
	uint32_t size;

	//Merge sort
	void sort(Node*& node1, Node*& node2,int distance)
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
		
		//node1 is a reference to the head pointer so that pointer doesn't need to be adjusted further
		node1 = merge(node1,centerNext);
		
		//ensure the tail is the last Node
		while(tail->next)
		{
			tail = tail->next;
		}
	}

	Node* merge(Node* head1, Node* head2)
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
			//set the previous pointer for the next node
			head1->next->previous = head1;
			return head1;
		}
		else
		{
			head2->next = merge(head1,head2->next);
			//set the previous pointer for the next node
			head2->next->previous = head2;
			return head2;
		}
	}

public:
	DoublyLinkedList()
	{
		size = 0;
		head = nullptr;
		tail = nullptr;
	}

	~DoublyLinkedList()
	{
		while(head != tail)
		{
			Node* temp = head;
			head = head->next;
			delete temp;
		
			size--;
		}
		
		delete head;
	}

	Data getHead()
	{
		return head->data;
	}

	Data getTail()
	{
		return tail->data;
	}

	int getSize()
	{
		return size;
	}

	bool add(Data data)
	{
		if(size<size+1) //ensures no overflow of size
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

	bool addAt(Data data,int index)
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

	Data* getAt(int index)
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
				return &nodeToReturn->data;
			}
			else
			{
				nodeToReturn = tail;
				for (int i = size-1; i > index; --i)
				{
					nodeToReturn = nodeToReturn->previous;
				}
				return &nodeToReturn->data;
			}
		}
		else
			return nullptr;
	}

	void sort()
	{
		sort(head,tail,size);
	}

	void clearList()
	{
		while(size > 0)
		{
			Node* temp = head;
			head = head->next;
			delete temp;
			size--;
		}
		head = nullptr;
		tail = nullptr;
	}

	class iterator
	{
		Node *node;
		public:
		iterator(Node *n) : node(n) {}
		
		iterator& operator++()
		{
			node = node->next;
			return *this;
		}
		
		iterator operator++(int)
		{
			iterator tmp(*this);
			operator++();
			return tmp;
		}
		
		bool operator==(const iterator& rhs)
		{
			return node==rhs.node;
		}

		bool operator!=(const iterator& rhs)
		{
			return node!=rhs.node;
		}
		
		Data& operator*()
		{
			return node->data;
		}

		//returns an iterator for the next list element if this iterator is not null (and therefor out of bounds)
		//otherwise returns this iterator
		iterator next()
		{
			return (*this).isNull()? *this : iterator(node->next);
		}

		bool isNull()
		{
			return node == nullptr;
		}
		
	};

	iterator begin()
	{
		return iterator(head);
	}

	iterator end()
	{
		return iterator(tail);
	}

};
#endif