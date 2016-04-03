#ifndef AVLTREE_H
#define AVLTREE_H

#include <stdint.h>

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

		//the file information info that the node contains
		FileInfo info;
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
	Node* add(FileInfo info, Node*& subtree)
	{
		if(subtree)
		{
			Node* newLeaf;
			if(subtree->data <= data)
			{
				newLeaf = add(data, subtree->rightChild);
				if(subtree->rightChild == newLeaf)
				{
					newLeaf->parent = subtree;
				}
			}
			else
			{
				newLeaf = add(data, subtree->leftChild);
				if(subtree->leftChild == newLeaf)
				{
					newLeaf->parent = subtree;
				}
			}
			return newLeaf;
		}
		else
		{
			subtree = new Node;
			subtree->leftChild = nullptr;
			subtree->rightChild = nullptr;
			subtree->parent = nullptr;
			subtree->height = 1;
			subtree->data = data;
			return subtree;
		}
	}
	
	//balances the tree starting from the given node and continuing until the tree is balanced
	//adjusts node height as needed
	void balance(Node* node)
	{
		node->height++;
		Node* parent = node->parent;

		//balance(Parent_Of_New_Leaf);
		//https://en.wikipedia.org/wiki/AVL_tree
		// node is the child of parent whose height increases by 1.
		while (parent)
		{
			// the left subtree increased
			if (node == parent->leftChild)
			{
				if (balanceFactor(parent) == 2)
				{
					// Left Right Case
					if (balanceFactor(node) == -1)
					{
						// Reduce to Left Left Case
						rotateLeft(node);
					}
					// Left Left Case
					rotateRight(parent);
					node->height--;
					break; // Leave the loop
				}
				if (balanceFactor(parent) == -1)
				{
					// node’s height increase is absorbed at parent.
					break;
				}
			}
			// the right subtree increased
			else
			{
				if (balanceFactor(parent) == -2)
				{
					// Right Left Case
					if (balanceFactor(node) == 1)
					{
						// Reduce to Right Right Case
						rotateRight(node);
					}
					// Right Right Case
					rotateLeft(parent);
					node->height--;
					break; // Leave the loop
				}
				if (balanceFactor(parent) == 1)
				{
					// node’s height increase is absorbed at parent.
					break;
				}
			}
			parent->height = node->height+1;
			node = parent;
			parent = node->parent;
		}

		if(!parent)
			root = node;
	}

	//rotates a node right
	void rotateRight(Node* node)
	{
		Node* leftChild = node->leftChild;

		Data temp;
		temp = leftChild->data;
		leftChild->data = node->data;

		node->data = temp;
		node->leftChild = leftChild->leftChild;
		if(node->leftChild)
		{
			node->leftChild->parent = node;
		}

		leftChild->leftChild = leftChild->rightChild;
		leftChild->rightChild = node->rightChild;
		if(leftChild->rightChild)
		{
			leftChild->rightChild->parent = leftChild;
		}

		node->rightChild = leftChild;
	}

	//rotates a node left
	void rotateLeft(Node* node)
	{
		Node* rightChild = node->rightChild;

		Data temp;
		temp = rightChild->data;
		rightChild->data = node->data;
		
		node->data = temp;
		node->rightChild = rightChild->rightChild;
		if(node->rightChild)
		{
			node->rightChild->parent = node;
		}
		
		rightChild->rightChild = rightChild->leftChild;
		rightChild->leftChild = node->leftChild;
		if(rightChild->leftChild)
		{
			rightChild->leftChild->parent = rightChild;
		}

		node->leftChild = rightChild;
	}

	//get helper method
	bool get(FileInfo info, Node*& node)
	{
		if(node)
		{
			if(node.data == data)
				return true;
			if(node.data > data)
				return get(data, node->leftChild);
			if(node.data < data)
				return get(data, node->rightChild);
		}
		return false;
	}

	//helper method that recursively deletes a node and its children
	void clearTree(Node*& subtree)
	{
	if(subtree->leftChild)
		clearTree(subtree->leftChild);
	if(subtree->rightChild)
		clearTree(subtree->rightChild);
	delete subtree;
	}

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
	void add(FileInfo info)
	{
		Node* newLeaf = add(root, data);
		if(size>0)
		{
			balance(newLeaf->parent);
		}
		else
		{
			root = newLeaf;
		}

		//reset root
		while(root->parent)
		{
			root=root->parent;
		}
		size++;
	}

	//EDIT THIS
	//by default get returns if a node exists in the tree
	bool get(FileInfo info)
	{
		return get(data, root);
	}

	//calls the recursive helper method to delete all subtrees of the root
	//sets the root to a nullptr
	//sets size to 0
	void clearTree()
	{
		if(root->leftChild)
		{
			clearTree(root->leftChild);
		}
		if(root->rightChild)
		{
			clearTree(root->rightChild);
		}
		size = 0;
		root = nullptr;
	}


};

#endif