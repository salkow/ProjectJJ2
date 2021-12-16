#ifndef _BK_T_
#define _BK_T_

#include "vector.h"
#include "../include/core.h"

template <typename T>
class BK
{
protected:
	struct Node
	{
		Node(T *&item, int parentDistance);
		T *content;
		// distance of this node compared to it's parent node, root's distance is 0
		int parentDistance;
		bool deleted;
		bud::vector<Node *> _edges;
	};

	// calculates the distance between two "T"s
	int (*distanceFunction)(T *, T *, unsigned int);
	Node *root;

	// recursively delete all nodes, used in the destructor
	void recDelete(Node *);

	// recursively searches the tree for words that have <= maxDistance to the given query
	bud::vector<T *> recSearch(Node *parent, T *query, int maxDistance) const;

	// traverses the tree and returns a pointer if the given item is found on it, NULL if not.
	Node *traverse(T *);

public:
	// the distance function finds the distance between two "T"s
	BK(int (*distanceFunction)(T *, T *, unsigned int));
	~BK();

	// inserts a "T" into the tree
	ErrorCode insert(T *);

	// searches the tree for words that have <= maxDistance to the given query
	bud::vector<T *> search(T *query, int maxDistance) const;

	//removes the given item from the tree
	void remove(T *);

	//restores the given item to the tree
	void restore(T *);
};

#include "BK_T.cpp"

#endif
