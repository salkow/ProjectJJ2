#ifndef _BK_T_CPP_
#define _BK_T_CPP_

#include <cassert>

#include "BK_T.h"
#include "vector.h"

template <typename T>
BK<T>::Node::Node(T *&content, int parentDistance) : content(content), parentDistance(parentDistance), deleted(false) {}

template <typename T>
BK<T>::BK(int (*distanceFunction)(T *, T *)) : distanceFunction(distanceFunction), root(NULL) {}

template <typename T>
BK<T>::~BK()
{
	if (root != NULL)
		recDelete(root);
}

template <typename T>
void BK<T>::recDelete(Node *parent)
{
	// traverse the tree and call delete all nodes recursively
	for (const auto &edge : parent->_edges)
	{
		recDelete(edge);
	}
	delete parent;
}

template <typename T>
typename BK<T>::Node *BK<T>::traverse(T *item)
{
	bud::vector<Node *> lookList;
	lookList.push_back(root);

	while (lookList)
	{
		Node *edge = lookList.back();
		if (edge->content == item)
			return edge;
		else
		{
			lookList.pop_back();
			for (int i = 1; i <= edge->_edges.getSize(); i++)
			{
				lookList.push_back(edge->_edges.at(i));
			}
		}
	}
	return NULL;
}

template <typename T>
void BK<T>::remove(T *item)
{
	Node *n = traverse(item);

	assert(n != NULL);
	n->deleted = true;
	// if (n == NULL) return;
}

template <typename T>
void BK<T>::insert(T *item)
{
	// item must not be a null pointer
	if (item == NULL)
		return;
	// set root if not existent
	if (root == NULL)
	{
		root = new Node(item, 0); // root's parentDistance is 0 since there's no parent
	}
	else
	{
		recInsert(root, item); // recursively traverse the tree to find an empty spot
	}
}

template <typename T>
void BK<T>::recInsert(Node *parent, T *item)
{
	// calculate distance of node-to-insert vs parent
	int distance = distanceFunction(parent->content, item);

	// look for a child node with the same distance
	bool found = false;
	for (const auto &edge : parent->_edges)
	{
		// child node found, look inside it
		if (edge->parentDistance == distance)
		{
			recInsert(edge, item);
			found = true;
			break;
		}
	}

	// child node not found, make a new node
	if (!found)
	{
		Node *newNode = new Node(item, distance);
		parent->_edges.push_back(newNode);
	}
}

template <typename T>
bud::vector<T> BK<T>::search(T *query, int maxDistance) const
{
	// handle null root
	if (root == NULL)
		return bud::vector<T>();
	else
		return recSearch(root, query, maxDistance);
}

template <typename T>
bud::vector<T> BK<T>::recSearch(Node *parent, T *query, int maxDistance) const
{
	bud::vector<T> results;

	// if the parent node fits our criteria, add to results and explore it's children
	if (distanceFunction(parent->content, query) <= maxDistance && !parent->deleted)
		results.push_back(parent->content);

	// iterate over parent's children, explore only those with
	// |child->parentDistance - maxDistance| <= maxDistance <= |child->parentDistance + maxDistance|
	for (const auto &edge : parent->_edges)
	{
		// if the child's parentDistance fits our criteria, call recSearch
		if (abs(edge->parentDistance - maxDistance) <= maxDistance ||
			maxDistance <= abs(edge->parentDistance + maxDistance))
		{

			bud::vector<T> temp_results = recSearch(edge, query, maxDistance);
			// add it's results to ours
			for (const auto &result : temp_results)
			{
				results.push_back(result);
			}
		}
	}
	return results;
}

#endif
