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

	int8_t balanceFactor(Node* node)
	{
		return (node->leftChild ? node->leftChild->height : 0) - (node->rightChild ? node->rightChild->height : 0);
	}

	Node* add(Node*& subTree, Data data);
	void balance(Node* node);
	void rotateRight(Node* node);
	void rotateLeft(Node* node);

	void printTree(Node*& subTree);
	void clearTree(Node*& subTree);

public:
	AVLTree()
	{
		size = 0;
		root = nullptr;
	}
	~AVLTree()
	{
		if(size)
			clearTree();
		delete root;
	}

	int getSize(){return size;}
	void add(Data data);

	Data* get(int index);

	void clearTree();

#if AVLTREE_DEBUG

	void printTree();

#endif
};
#endif