#include "elgoog.hpp"

#include <cstring>
#include <iostream>

#include "sigmod.hpp"
#include "util.hpp"

// wraps EditDistance for use as a distance function in the BK tree
static int EditDistanceWrapper(char * a, char * b) {
	return EditDistance(a, strlen(a), b, strlen(b));
}

// wraps HammingDistance for use as a distance function in the BK tree
static int HammingDistanceWrapper(char * a, char * b) {
	return HammingDistance(a, strlen(a), b, strlen(b));
}

// sum string's letters and mod with EXACT_HT_SIZE
static int ExactHashFunction(Elgoog::entry ent) { return Util::stringHashFunction(ent.getWord()); }

Elgoog::entry::entry_payload::entry_payload(){};

Elgoog::entry::entry_payload::~entry_payload(){};

// copy given word to entry
Elgoog::entry::entry(char * word) {
	this->word = (char *) malloc(sizeof(char) * MAX_WORD_LENGTH);
	strcpy(this->word, word);
};

Elgoog::entry::entry(const entry & ent) {
	this->word = (char *) malloc(sizeof(char) * MAX_WORD_LENGTH);
	strcpy(this->word, ent.word);
}

// free dynamically allocated word
Elgoog::entry::~entry() { free(word); };

char * Elgoog::entry::getWord() const { return word; }

bool Elgoog::entry::operator==(Elgoog::entry & right) const {
	return strcmp(word, right.word) == 0;
}

Elgoog::entry_list::entry_list(){};

Elgoog::entry_list::~entry_list(){};

unsigned int Elgoog::entry_list::get_number_entries() const { return list.getSize(); }

Elgoog::error_code Elgoog::entry_list::add_entry(entry * e) {
	// make sure given entry is not null
	if (e == NULL) return error_code::NULL_POINTER;
	list.push(e); // add to list
	return error_code::NO_ERROR;
}

Elgoog::entry * Elgoog::entry_list::get(int index) {
	// assert index value
	if (index <= 0 || index > list.getSize()) throw error_code::INVALID_INDEX;
	return list.at(index);
}

Elgoog::entry_index::entry_index(entry_list * el, match_type type) : type(type), list(el) {
	// assert list to exist
	if (list == NULL) throw error_code::NULL_POINTER;

	// no reason to create an index with an empty list
	if (list->get_number_entries() == 0) throw error_code::EMPTY_LIST;

	if (type == match_type::EXACT) {
		ht = new HashTable<entry, entry>(1, HASH_MAX, ExactHashFunction);
	} else {
		// pointer to distance function
		static int (*distanceFunction)(char *, char *);

		switch (type) {
		case match_type::EDIT: distanceFunction = EditDistanceWrapper; break;
		case match_type::HAMMING: distanceFunction = HammingDistanceWrapper; break;
		case match_type::EXACT: break;
		};

		tree = new BK<char>(distanceFunction);
	}

	// add each entry of the list to the tree
	for (size_t i = 1; i <= list->get_number_entries(); i++) {
		entry * ent = list->get(i);
		if (type == match_type::EXACT) {
			ht->insert(*ent, *ent);
		} else
			tree->insert(ent->getWord());
	}
};

Elgoog::entry_index::~entry_index() {
	if (type == match_type::EXACT)
		delete ht;
	else
		delete tree;
};

//
Elgoog::entry_list * Elgoog::entry_index::lookup(char * word, int threshold) {
	if (word == NULL) throw error_code::NULL_POINTER;
	if (type == match_type::EXACT) threshold = 0;
	entry_list * el = new entry_list(); // create entry list to store results

	if (type == match_type::EXACT) {
		entry * ent = ht->get(entry(word));
		if (ent != NULL) el->add_entry(new entry(*ent));
	} else {
		List<char> results = tree->search(word, threshold);
		// add each result to the entry list
		for (int i = 1; i <= results.getSize(); i++) {
			el->add_entry(new entry(results.at(i)));
		};
	}
	return el;
};
