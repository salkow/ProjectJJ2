#ifndef _BK_T_
#define _BK_T_

#include "vector.h"

template <typename T>
class BK
{
	struct Node
	{
		Node(T *&item, int parentDistance);
		T *content;
		bool deleted;
		bud::vector<Node *> _edges;
		int parentDistance; // distance of this node compared to it's parent node, root's distance
							// is 0
	};

	int (*distanceFunction)(T *, T *); // calculates the distance between two "T"s
	Node *root;
	void recInsert(Node *parent, T *item); // inserts a "T" into a tree recusrsively
	void recDelete(Node *);				   // recursively delete all nodes, used in the destructor
	bud::vector<T> recSearch(Node *parent, T *query,
							 int maxDistance) const; // recursively searches the tree for words that
													 // have <= maxDistance to the given query
	Node *traverse(T *);							 // traverses the tree and returns a pointer if the
													 // given item is found on it, NULL if not.

public:
	BK(int (*distanceFunction)(T *,
							   T *)); // the distance function finds the distance between two "T"s
	~BK();

	void insert(T *); // inserts a "T" into the tree
	bud::vector<T> search(T *query,
						  int maxDistance) const; // searches the tree for words that
												  // have <= maxDistance to the given query
	void remove(T *);							  //removes the given item from the tree
};

#include "BK_T.cpp"

#endif
