#ifndef _HASHTABLE_T_
#define _HASHTABLE_T_

#include "list_T.cpp"

template <typename Key, typename Value>
class HashTable {
	int bucketSize;
	int indexCount;

  protected:
	// protected so someone can override it to support more custom data types
	// virtual int hashFunction(Key) const;
	int (*hashFunction)(Key);
	int modHashFunction(Key) const;

  public:
	struct Item { // used as a return type in all() function
		Key key;
		Value value;
		Item(Key, Value);
		Item();
	};

	HashTable(int bucketSize, int indexCount);
	HashTable(int bucketSize, int indexCount, int (*hashFunction)(Key));
	virtual ~HashTable(); // compiler didn't like this destructor not beign virtual although it made
						  // no difference

	void insert(Key, Value); // inserts a key,value pair to the hashtable
	Value * get(Key) const;	 // get a value with the given key
	bool exists(Key) const;	 // check whether the given key exists in the hashtable
	List<Item> *
	all() const; // returns a list with all the items in the hashtable, the list is dynamically
				 // allocated and should be `delete`ed when deemed appropriate

  private:
	// the hashtable uses these to store values
	struct Bucket {
		Item ** values;	  // table of pointers to values
		int numOfEntries; // how full that above table is

		Bucket(int);
		~Bucket();
	};
	List<Bucket> ** table; // the hashtable table
};

#include "hashtable_T.cpp"

#endif
