#include "dedup.hpp"

#include <cstring>
#include <iostream>

#include "hashtable_T.hpp"
#include "list_T.hpp"
#include "util.hpp"

#include "core.hpp"

char * dedup(char * input) {

	// just a string-type with an == operator
	struct myString {
		char * str;
		myString(char * str) : str(str){};
		bool operator==(myString & right) const { return strcmp(str, right.str) == 0; }
	};

	// sums the string's chars and mods by HT_SIZE
	int (*hashFunction)(myString str) = [](myString str) {
		return Util::stringHashFunction(str.str);
	};

	// in order to optimise lookup, we're using a hashtable
	HashTable<myString, char *> ht(1, HASH_MAX, hashFunction);

	// split input string's words and add to the hashtable
	char * current_token = strtok(input, " ");

	while (current_token != NULL) {
		if (!ht.exists(current_token)) ht.insert(current_token, current_token);
		current_token = strtok(NULL, " ");
	}

	// since we're checking with `exists`, every item in the hashtable is unique
	List<HashTable<myString, char *>::Item> * all = ht.all();

	// allocate the output string
	char * ret = (char *) malloc(sizeof(char) * MAX_DOC_LENGTH);
	ret[0]	   = '\0';

	// copy each word to the output string and add spaces
	while (all->getSize() > 0) {
		HashTable<myString, char *>::Item * itm = all->pop();
		if (strlen(ret) > 0) strcat(ret, " ");
		strcat(ret, itm->value);
	}

	delete all; // free result list

	return ret;
}
