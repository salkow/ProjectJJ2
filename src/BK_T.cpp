#ifndef _BK_T_CPP_
#define _BK_T_CPP_

#include <cassert>

#include "BK_T.h"

template <typename T>
BK<T>::Node::Node(T *&content, int parentDistance) : content(content), parentDistance(parentDistance) {}

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
	while (parent->_edges.getSize() > 0)
	{
		Node *edge = parent->_edges.pop();
		recDelete(edge);
	}
	delete parent;
}

template <typename T>
typename BK<T>::Node *BK<T>::traverse(T *item)
{
	bool found = false;
	Node *current = root;
	while (!found)
	{
		if (current != item)
		{
		}
	}
}

template <typename T>
void BK<T>::remove(T *item)
{
	Node *n = traverse(item);

	assert(n != NULL);
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
	for (int i = 1; i <= parent->_edges.getSize(); i++)
	{
		Node *edge = parent->_edges.at(i);
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
		parent->_edges.push(newNode);
	}
}

template <typename T>
List<T> BK<T>::search(T *query, int maxDistance) const
{
	// handle null root
	if (root == NULL)
		return List<T>();
	else
		return recSearch(root, query, maxDistance);
}

template <typename T>
List<T> BK<T>::recSearch(Node *parent, T *query, int maxDistance) const
{
	List<T> results;

	// if the parent node fits our criteria, add to results and explore it's children
	if (distanceFunction(parent->content, query) <= maxDistance)
		results.push(parent->content);

	// iterate over parent's children, explore only those with
	// |child->parentDistance - maxDistance| <= maxDistance <= |child->parentDistance + maxDistance|
	for (int i = 1; i <= parent->_edges.getSize(); i++)
	{
		Node *edge = parent->_edges.at(i);

		// if the child's parentDistance fits our criteria, call recSearch
		if (abs(edge->parentDistance - maxDistance) <= maxDistance ||
			maxDistance <= abs(edge->parentDistance + maxDistance))
		{

			List<T> temp_results = recSearch(edge, query, maxDistance);
			// add it's results to ours
			while (temp_results.getSize() > 0)
			{
				results.push(temp_results.pop());
			}
		}
	}
	return results;
}

#endif
