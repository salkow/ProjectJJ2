#ifndef _HASHTABLE_T_CPP_
#define _HASHTABLE_T_CPP_

#include "hashtable_T.hpp"
#include <algorithm>

template <typename K, typename V>
HashTable<K, V>::Item::Item(K key, V value) : key(key), value(value){};

template <typename K, typename V>
HashTable<K, V>::Item::Item(){};

template <typename K, typename V>
HashTable<K, V>::Bucket::Bucket(int maxNumOfEntries) {
	// bucket has an array of values
	values		 = new Item *[maxNumOfEntries];
	numOfEntries = 0;
}

template <typename K, typename V>
HashTable<K, V>::Bucket::~Bucket() {
	// delete all items
	for (int i = 0; i < numOfEntries; i++) {
		delete values[i];
	}

	delete[] values;
}

template <typename K, typename V>
HashTable<K, V>::HashTable(int bucketSize, int indexCount, int (*hashFunction)(K)) :
	bucketSize(bucketSize), indexCount(indexCount), hashFunction(hashFunction) {
	// the table is an array of lists of buckets
	table = new List<Bucket> *[indexCount];
	for (int i = 0; i < indexCount; i++) {
		table[i] = new List<Bucket>;
	}
};

template <typename K, typename V>
HashTable<K, V>::HashTable(int bucketSize, int indexCount) :
	HashTable(bucketSize, indexCount, HashTable::modHashFunction){};

template <typename K, typename V>
HashTable<K, V>::~HashTable() {
	for (int i = 0; i < indexCount; i++) {
		List<Bucket> * b = table[i];
		for (int j = 1; j <= b->getSize(); j++) {
			delete b->at(j);
		}
		delete b;
	}
	delete[] table;
}

template <typename K, typename V>
int HashTable<K, V>::modHashFunction(K key) const {
	return key % indexCount;
}

template <typename K, typename V>
void HashTable<K, V>::insert(K key, V value) {
	int idx = hashFunction(key);
	// calc the max number of entries
	int maxNumOfEntries = (bucketSize - sizeof(Bucket::numOfEntries)) / sizeof(Item);
	maxNumOfEntries		= std::max(maxNumOfEntries, 1);

	Item * i = new Item(key, value); // item to be inserted

	if (table[idx]->getSize() == 0) {
		// case where it's the first time an entry is inserted on this index
		Bucket * nb = new Bucket(maxNumOfEntries);
		table[idx]->push(nb);
	}

	Bucket * b = table[idx]->at(table[idx]->getSize()); // get the last bucket

	if (b->numOfEntries >= maxNumOfEntries) {
		// case when the bucket is full, make a new bucket
		b = new Bucket(maxNumOfEntries);
		table[idx]->push(b);
	}

	b->values[b->numOfEntries] = i;
	b->numOfEntries++;
}

template <typename K, typename V>
V * HashTable<K, V>::get(K key) const {
	int idx				   = hashFunction(key);
	List<Bucket> * buckets = table[idx];

	Bucket * b;

	bool found = false;
	V * value  = NULL;
	// iterate the bucket list until we find an entry with the given key
	for (int bucketsIndex = 1; bucketsIndex <= buckets->getSize(); bucketsIndex++) {

		b = buckets->at(bucketsIndex);

		// iterate the bucket value array
		for (int i = 0; i < b->numOfEntries; i++) {
			if (key == b->values[i]->key) {
				found = true;
				value = &(b->values[i]->value);
				break;
			}
		}

		if (found) break;
	}

	return value;
}

template <typename K, typename V>
bool HashTable<K, V>::exists(K key) const {
	return get(key) != NULL;
}

template <typename K, typename V>
List<typename HashTable<K, V>::Item> * HashTable<K, V>::all() const {
	List<Item> * l = new List<Item>();
	List<Bucket> * buckets;

	Bucket * b;

	// iterate over all the entries in the hashtable and add them to the list
	for (int x = 0; x < indexCount; x++) {
		buckets = table[x];
		for (int bucketsIndex = 1; bucketsIndex <= buckets->getSize(); bucketsIndex++) {
			b = buckets->at(bucketsIndex);
			for (int i = 0; i < b->numOfEntries; i++) {
				l->push(b->values[i]);
			}
		}
	}

	return l;
}

#endif
