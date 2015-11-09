#ifndef AVLTREE_H
#define AVLTREE_H
#define AVLTREE_DEBUG 1

#include <stdint.h>

template <typename Data>
class AVLTree
{
	struct Node
	{
		Node* parent;
		Node* leftChild;
		Node* rightChild;
		uint32_t height;
		Data data;
	};

	Node* root;
	uint32_t size;

	bool add(Node*& subTree, Data data);

	void printTree(Node*& subTree);

public:
	AVLTree();
	~AVLTree();

	int getSize();
	bool add(Data data);

	Data* get(int index);

	void clearTree();

#if DOUBLYLINKEDLIST_DEBUG

	void printTree();

#endif
};
#endif