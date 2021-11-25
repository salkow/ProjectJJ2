#ifndef _LIST_T_
#define _LIST_T_

#include <iostream>

template <class T>
class List {
	struct Node { // node class, contains T * as data, only accessible to the
				  // List class
		Node(T *&);
		void next(Node *);	 // set next node
		Node * next() const; // get next node
		void content(T *&);	 // set content
		T * content() const; // get content

	  private:
		T * _content;
		Node * _next;
	};

	Node * start; // start of list
	int size;

	void removeAtIndex(const int); // deletes a node at the specified index

  public:
	List();
	List(const List &);
	List & operator=(const List &);

	~List();

	void push(T *);		 // inserts a T at the end of the list
	void push_back(T *); // inserts a T at the start of the list

	int indexOf(const T) const; // searches for the item v in the list and if found, it
								// returns it's index, if not, it returns -1

	T * pop(); // removes and returns the last item of the list

	T * pop(int n); // removes and returns the n'th item of the list

	T * at(int n) const; // returns the n'th item of the list

	int getSize() const; // returns the size of the list

	void deleteContents(); // removes and calls `delete` for every item on the list, handy for
						   // destructors
};

#include "list_T.cpp"

#endif
