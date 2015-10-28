#include "DoublyLinkedList.h"

#if DOUBLYLINKEDLIST_DEBUG
#include <iostream>
#include <random>
#include <ctime>
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
Data* DoublyLinkedList<Data>::getAt(int index)
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
			return &(nodeToReturn->data);
		}
		else
		{
			nodeToReturn = tail;
			for (int i = size-1; i > index; --i)
			{
				nodeToReturn = nodeToReturn->previous;
			}
			return &(nodeToReturn->data);
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

#include <unistd.h>

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

	int i = 0;
	Node* test;

	std::cout << "Sorted" << std::endl;
	for (test = node1; test != center; test = test->next)
	{
		std::cout << i << " - ";
		std::cout << (test->data.fileName);
		std::cout << (" - ");
		std::cout << (test->data.index) << std::endl;
		i++;
	}
	std::cout << i << " - ";
	std::cout << (test->data.fileName);
	std::cout << (" - ");
	std::cout << (test->data.index) << std::endl << "-----------------------------------" << std::endl;
	i++;
	for (test = centerNext; test != node2; test = test->next)
	{
		std::cout << i << " - ";
		std::cout << (test->data.fileName);
		std::cout << (" - ");
		std::cout << (test->data.index) << std::endl;
		i++;
	}
	std::cout << i << " - ";
	std::cout << (test->data.fileName);
	std::cout << (" - ");
	std::cout << (test->data.index);
	std::cout << std::endl;
	std::cout << std::endl;

	node1 = merge(node1,centerNext);
	
	i=0;
	std::cout << "Merged" << std::endl;
	for (test = node1; test != center; test = test->next)
	{
		std::cout << i << " - ";
		std::cout << (test->data.fileName);
		std::cout << (" - ");
		std::cout << (test->data.index) << std::endl;
		i++;
	}
	std::cout << i << " - ";
	std::cout << (test->data.fileName);
	std::cout << (" - ");
	std::cout << (test->data.index) << std::endl << "-----------------------------------" << std::endl;
	i++;
	for (test = centerNext; test != node2; test = test->next)
	{
		std::cout << i << " - ";
		std::cout << (test->data.fileName);
		std::cout << (" - ");
		std::cout << (test->data.index) << std::endl;
		i++;
	}
	std::cout << i << " - ";
	std::cout << (test->data.fileName);
	std::cout << (" - ");
	std::cout << (test->data.index);
	std::cout << std::endl;
	std::cout << std::endl;
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
		std::cout << temp->data.fileName << " - " << temp->data.index << std::endl;
		temp = temp->next;
	}
	std::cout << std::endl;
}

struct FileInfo {
	uint32_t index;
	char fileName[250] = {};
};

bool operator==(const FileInfo &left, const FileInfo &right)
{
	for (uint i = 0; left.fileName[i] != '\0' && right.fileName[i] != '\0'; ++i)
	{
		if(tolower(left.fileName[i]) != tolower(right.fileName[i]))
			return false;
	}
	return true;
}

bool operator!=(const FileInfo &left, const FileInfo &right)
{
	return !(left==right);
}

bool operator<(const FileInfo &left, const FileInfo &right)
{
	for (uint i = 0; left.fileName[i] != '\0' && right.fileName[i] != '\0'; ++i)
	{

		if(tolower(left.fileName[i]) > tolower(right.fileName[i]))
			return false;
		else if(tolower(left.fileName[i]) < tolower(right.fileName[i]))
			return true;
	}
	return false;
}

bool operator>(const FileInfo &left, const FileInfo &right)
{
	return right<left;
}

bool operator<=(const FileInfo &left, const FileInfo &right)
{
	return !(right<left);
}

bool operator>=(const FileInfo &left, const FileInfo &right)
{
	return !(left<right);
}

int main(int argc, char const *argv[])
{
	DoublyLinkedList<FileInfo> d;

	FileInfo test;
	test.fileName[0] = '.';
	test.fileName[1] = '_';
	test.fileName[2] = '.';
	test.fileName[3] = 'T';
	test.fileName[4] = 'r';
	test.fileName[5] = 'a';
	test.fileName[6] = 's';
	test.fileName[7] = 'h';
	test.fileName[8] = 'e';
	test.fileName[9] = 's';
	test.fileName[10] = '\0';
	test.index = 96;
	d.add(test);

	FileInfo test2;
	test2.fileName[0] = '.';
	test2.fileName[1] = 'T';
	test2.fileName[2] = 'r';
	test2.fileName[3] = 'a';
	test2.fileName[4] = 's';
	test2.fileName[5] = 'h';
	test2.fileName[6] = 'e';
	test2.fileName[7] = 's';
	test2.fileName[8] = '\0';
	test2.index = 192;
	d.add(test2);

	FileInfo test3;
	test3.fileName[0] = '.';
	test3.fileName[1] = 'S';
	test3.fileName[2] = 'p';
	test3.fileName[3] = 'o';
	test3.fileName[4] = 't';
	test3.fileName[5] = 'l';
	test3.fileName[6] = 'i';
	test3.fileName[7] = 'g';
	test3.fileName[8] = 'h';
	test3.fileName[9] = 't';
	test3.fileName[10] = '-';
	test3.fileName[11] = 'V';
	test3.fileName[12] = '1';
	test3.fileName[13] = '0';
	test3.fileName[14] = '0';
	test3.fileName[15] = '\0';
	test3.index = 288;
	d.add(test3);

	FileInfo test4;
	test4.fileName[0] = '.';
	test4.fileName[1] = 'f';
	test4.fileName[2] = 's';
	test4.fileName[3] = 'e';
	test4.fileName[4] = 'v';
	test4.fileName[5] = 'e';
	test4.fileName[6] = 'n';
	test4.fileName[7] = 't';
	test4.fileName[8] = 's';
	test4.fileName[9] = 'd';
	test4.fileName[10] = '\0';
	test4.index = 352;
	d.add(test4);

	FileInfo test5;
	test5.fileName[0] = 'L';
	test5.fileName[1] = 'O';
	test5.fileName[2] = 'S';
	test5.fileName[3] = 'T';
	test5.fileName[4] = '.';
	test5.fileName[5] = 'D';
	test5.fileName[6] = 'I';
	test5.fileName[7] = 'R';
	test5.fileName[8] = '\0';
	test5.index = 384;
	d.add(test5);

	FileInfo test6;
	test6.fileName[0] = '.';
	test6.fileName[1] = 'a';
	test6.fileName[2] = 'n';
	test6.fileName[3] = 'd';
	test6.fileName[4] = 'r';
	test6.fileName[5] = 'o';
	test6.fileName[6] = 'i';
	test6.fileName[7] = 'd';
	test6.fileName[8] = '_';
	test6.fileName[9] = 's';
	test6.fileName[10] = 'e';
	test6.fileName[11] = 'c';
	test6.fileName[12] = 'u';
	test6.fileName[13] = 'r';
	test6.fileName[14] = 'e';
	test6.fileName[15] = '\0';
	test6.index = 480;
	d.add(test6);

	// FileInfo test7;
	// test7.fileName[0] = 'A';
	// test7.fileName[1] = 'n';
	// test7.fileName[2] = 'd';
	// test7.fileName[3] = 'r';
	// test7.fileName[4] = 'o';
	// test7.fileName[5] = 'i';
	// test7.fileName[6] = 'd';
	// test7.fileName[7] = '\0';
	// test7.index = 544;
	// d.add(test7);

	// FileInfo test8;
	// test8.fileName[0] = 'S';
	// test8.fileName[1] = 'i';
	// test8.fileName[2] = 'z';
	// test8.fileName[3] = 'e';
	// test8.fileName[4] = 'T';
	// test8.fileName[5] = 'e';
	// test8.fileName[6] = 's';
	// test8.fileName[7] = 't';
	// test8.fileName[8] = '.';
	// test8.fileName[9] = 't';
	// test8.fileName[10] = 'x';
	// test8.fileName[11] = 't';
	// test8.fileName[12] = '\0';
	// test8.index = 608;
	// d.add(test8);

	// FileInfo test9;
	// test9.fileName[0] = 'L';
	// test9.fileName[1] = 'G';
	// test9.fileName[2] = 'B';
	// test9.fileName[3] = 'a';
	// test9.fileName[4] = 'c';
	// test9.fileName[5] = 'k';
	// test9.fileName[6] = 'u';
	// test9.fileName[7] = 'p';
	// test9.fileName[8] = '\0';
	// test9.index = 672;
	// d.add(test9);

	// FileInfo test10;
	// test10.fileName[0] = 'M';
	// test10.fileName[1] = 'u';
	// test10.fileName[2] = 's';
	// test10.fileName[3] = 'i';
	// test10.fileName[4] = 'c';
	// test10.fileName[5] = '\0';
	// test10.index = 736;
	// d.add(test10);

	// ._.Trashes - 96
	// .Trashes - 192
	// .Spotlight-V100 - 288
	// .fseventsd - 352
	// LOST.DIR - 384
	// .android_secure - 480
	// Android - 544
	// SizeTest.txt - 608
	// LGBackup - 672
	// Music - 736
	
	// d.printList();


	for(int i=0; i<d.getSize(); i++)
	{
		std::cout << i << " - ";
		std::cout << (d.getAt(i)->fileName);
		std::cout << (" - ");
		std::cout << (d.getAt(i)->index) << std::endl;
	}
	std::cout << std::endl;

	d.sort();
	
	// d.printList();
	

	for(int i=0; i<d.getSize(); i++)
	{
		std::cout << i << " - ";
		std::cout << (d.getAt(i)->fileName);
		std::cout << (" - ");
		std::cout << (d.getAt(i)->index) << std::endl;
	}

	return 0;
}

#endif