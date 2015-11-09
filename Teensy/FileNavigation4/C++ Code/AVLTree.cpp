#include "AVLTree.h"

#if AVLTREE_DEBUG
#include <iostream>
#include <random>
#include <ctime>
#endif

template <typename Data>
AVLTree<Data>::AVLTree()
{
	size = 0;
	root = nullptr;
}

template <typename Data>
AVLTree<Data>::~AVLTree()
{
	clearTree();
	delete parent;
}

template <typename Data>
int AVLTree<Data>::getSize()
{
	return size;
}

template <typename Data>
bool AVLTree<Data>::add(Data data)
{
	if(root)
	{
		add(root, data);
	}
	else
	{
		root = new Node;
		root->leftChild = nullptr;
		root->rightChild = nullptr;
		root->height = 0;
		root->data = data;
	}
	size++;
	return true;
}

template <typename Data>
bool AVLTree<Data>::add(Node*& subTree, Data data)
{
	if(subTree)
	{
		if(subTree->data <= data)
		{
			if(!add(subTree->rightChild, data))
				return false;
		}
		else
		{
			if(!add(subTree->leftChild, data))
				return false;
		}
		subTree->height++;
	}
	else
	{
		Node* node = new Node;
		node->leftChild = nullptr;
		node->rightChild = nullptr;
		node->height = 0;
		node->data = data;
	}
	return true;
}

template <typename Data>
Data* AVLTree<Data>::get(int index)
{
	return nullptr;
}

template <typename Data>
void AVLTree<Data>::clearTree()
{
	// while(size > 1)
	// {
	// 	Node* temp = head;
	// 	head = head->next;
	// 	delete temp;
	
	// 	size--;
	// }
	
	// head = nullptr;
	// tail = nullptr;
}

#if AVLTree_DEBUG

template <typename Data>
void AVLTree<Data>::printTree()
{
	printTree(root);
}

template <typename Data>
void AVLTree<Data>::printTree(Node*& subTree)
{
	printTree(subTree->leftChild);
	std::cout << subTree->data << " ";
	printTree(subTree->rightChild);
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
	AVLTree<FileInfo> d;

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

	FileInfo test7;
	test7.fileName[0] = 'A';
	test7.fileName[1] = 'n';
	test7.fileName[2] = 'd';
	test7.fileName[3] = 'r';
	test7.fileName[4] = 'o';
	test7.fileName[5] = 'i';
	test7.fileName[6] = 'd';
	test7.fileName[7] = '\0';
	test7.index = 544;
	d.add(test7);

	FileInfo test8;
	test8.fileName[0] = 'S';
	test8.fileName[1] = 'i';
	test8.fileName[2] = 'z';
	test8.fileName[3] = 'e';
	test8.fileName[4] = 'T';
	test8.fileName[5] = 'e';
	test8.fileName[6] = 's';
	test8.fileName[7] = 't';
	test8.fileName[8] = '.';
	test8.fileName[9] = 't';
	test8.fileName[10] = 'x';
	test8.fileName[11] = 't';
	test8.fileName[12] = '\0';
	test8.index = 608;
	d.add(test8);

	FileInfo test9;
	test9.fileName[0] = 'L';
	test9.fileName[1] = 'G';
	test9.fileName[2] = 'B';
	test9.fileName[3] = 'a';
	test9.fileName[4] = 'c';
	test9.fileName[5] = 'k';
	test9.fileName[6] = 'u';
	test9.fileName[7] = 'p';
	test9.fileName[8] = '\0';
	test9.index = 672;
	d.add(test9);

	FileInfo test10;
	test10.fileName[0] = 'M';
	test10.fileName[1] = 'u';
	test10.fileName[2] = 's';
	test10.fileName[3] = 'i';
	test10.fileName[4] = 'c';
	test10.fileName[5] = '\0';
	test10.index = 736;
	d.add(test10);

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


	// for(int i=0; i<d.getSize(); i++)
	// {
	// 	std::cout << i << " - ";
	// 	std::cout << (d.getAt(i)->fileName);
	// 	std::cout << (" - ");
	// 	std::cout << (d.getAt(i)->index) << std::endl;
	// }
	// std::cout << std::endl;

	// d.sort();

	d.printTree();

	// for(int i=0; i<d.getSize(); i++)
	// {
	// 	std::cout << i << " - ";
	// 	std::cout << (d.getAt(i)->fileName);
	// 	std::cout << (" - ");
	// 	std::cout << (d.getAt(i)->index) << std::endl;
	// }

	return 0;
}

#endif