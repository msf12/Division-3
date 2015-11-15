#ifndef AVLTREE_H
#define AVLTREE_H
#define AVLTREE_DEBUG 1

#include <stdint.h>

template <typename Data>
class AVLTree
{
	struct Node
	{
		//pointer to the parent node
		Node* parent;

		//pointer to the left child node
		Node* leftChild;

		//pointer to the right child node
		Node* rightChild;

		//height of the tallest subtree of the node
		//leaves have 1 height to distinguish them from null nodes which have 0 height
		uint32_t height;

		//the data that the node contains
		Data data;
	};

	//root node of the tree
	Node* root;

	//size of the tree
	uint32_t size;

	//method that calculates the balance factor of a node
	//balance factor is used when rebalancing the tree after adding a new node
	//the balance factor of a node is the difference in height of its child subtrees
	//a balance factor of 2 or -2 signals an unbalanced subtree
	int8_t balanceFactor(Node* node)
	{
		return (node->leftChild ? node->leftChild->height : 0) - (node->rightChild ? node->rightChild->height : 0);
	}

	//add helper method 
	Node* add(Node*& subtree, Data data);

	//get helper method
	bool get(Data data, Node*& node);
	
	//balances the tree starting from the given node and continuing until the tree is balanced
	//adjusts node height as needed
	void balance(Node* node);

	//rotates a node right
	void rotateRight(Node* node);

	//rotates a node left
	void rotateLeft(Node* node);

	//helper method that recursively deletes a node and its children
	void clearTree(Node*& subtree);

#if AVLTREE_DEBUG
	void printTree(Node*& subtree);
#endif

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

	//returns the current size of the tree
	int getSize(){return size;}

	//calls the private add helper method
	//rebalances the tree using the returned pointer to the new leaf
	//ensures that the root pointer is correct and, if it isn't, adjusts it
	//increments size
	void add(Data data);

	//by default get returns if a node exists in the tree
	bool get(Data data);

	//calls the recursive helper method to delete all subtrees of the root
	//sets the root to a nullptr
	//sets size to 0
	void clearTree();

#if AVLTREE_DEBUG
	void printTree();
#endif

};

#endif