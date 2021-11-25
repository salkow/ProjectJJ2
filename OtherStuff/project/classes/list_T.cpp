#ifndef _LIST_T_CPP_
#define _LIST_T_CPP_

#include "list_T.hpp"
#include <cassert>

template <class T>
List<T>::Node::Node(T *& v) : _content(v), _next(NULL) {}

template <class T>
void List<T>::Node::next(List<T>::Node * a) {
	_next = a;
}

template <class T>
typename List<T>::Node * List<T>::Node::next() const {
	return _next;
}

template <class T>
void List<T>::Node::content(T *& v) {
	_content = v;
}

template <class T>
T * List<T>::Node::content() const {
	return _content;
}

template <class T>
void List<T>::removeAtIndex(const int index) {
	assert(index > 0);
	assert(index <=
		   size); // index must be a non zero positive value that's not larger than the list size

	Node *curs = start, *prev = curs;
	int ctr = 1;

	while (ctr != index) { // find the node that's at the index position
		prev = curs;
		curs = curs->next();
		ctr++;
	}
	if (curs->next() == NULL) { // check if it's the last node on the list
		if (start == curs)		// check if it's also the first node on the list
			start = NULL;
		else
			prev->next(NULL);
	} else {
		if (start == curs) { // check if it's the first node on the list
			start = curs->next();
		} else {
			prev->next(curs->next());
		}
	}

	delete curs;
	size--; // since we deleted a node we should update the size
}

template <class T>
List<T>::List() : start(NULL), size(0){};

template <class T>
List<T>::List(const List & l) : start(NULL), size(0) {
	// push given list's items to `this`
	for (int i = 1; i <= l.size; i++) {
		this->push(l.at(i));
	}
};

template <class T>
List<T> & List<T>::operator=(const List & l) {
	// empty current list
	while (size > 0) {
		removeAtIndex(size);
	}

	// push given one's
	for (int i = 1; i <= l.size; i++) {
		push(l.at(i));
	}
	return *this;
};

template <class T>
List<T>::~List() {
	while (size > 0) {
		removeAtIndex(size);
	}
}

template <class T>
void List<T>::push(T * v) {
	if (start == NULL) {
		start = new Node(v);
	} else {
		// handle edge case
		Node * curs = start;

		while (curs->next() != NULL) { // move until we either find the end of the list
			curs = curs->next();
		}

		// insert node
		Node * n = new Node(v);
		n->next(NULL);
		curs->next(n);
	}
	size++; // don't forget to update the size of our list
}

template <class T>
void List<T>::push_back(T * v) {
	Node * s = start;

	start = new Node(v);
	start->next(s);

	size++;
}

template <class T>
int List<T>::indexOf(const T v) const {

	if (size == 0) { return -1; }

	if (start->next() == NULL) { return *(start->content()) == v ? 1 : -1; }

	Node * curs = start;
	int idx		= 1;
	bool found	= false;

	while (curs != NULL && !found) { // move until we match or get to the end of the list
		if (*(curs->content()) == v) {
			found = true;
		} else {
			idx++;
			curs = curs->next();
		}
	}

	if (found) {
		return idx;
	} else {
		return -1;
	}
}

template <class T>
T * List<T>::pop() {
	if (size == 0) { return NULL; }

	T * ret = at(size); // get the item
	removeAtIndex(size);
	return ret;
}

template <class T>
T * List<T>::pop(int index) {
	T * ret = at(index); // get the item
	removeAtIndex(index);
	return ret;
}

template <class T>
T * List<T>::at(int index) const {
	assert(index > 0);
	assert(index <= size);

	Node * curs = start;
	int ctr		= 1;

	while (ctr < index) {
		curs = curs->next();
		ctr++;
	}

	return curs->content();
}

template <class T>
int List<T>::getSize() const {
	return size;
}

template <class T>
void List<T>::deleteContents() {
	while (size > 0) {
		delete pop(size);
	}
}

#endif
