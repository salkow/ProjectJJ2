// #include "classes/dummy.hpp"
// #include "classes/BK_T.hpp"
#include "classes/core.hpp"
// #include "classes/sigmod.hpp"
// #include "classes/list_T.hpp"
// #include "classes/hashtable_T.hpp"

#include "classes/dedup.hpp"
#include "classes/elgoog.hpp"

#include <cstring>
#include <iostream>
#include <fstream>

#define THRESHOLD 2
#define DOC_FILE "tiny_test.txt"
#define MATCHTYPE Elgoog::match_type::EXACT
#define HAMMING_SIZE 5

// merges two entry_lists
void entry_list_merge(Elgoog::entry_list * dest, Elgoog::entry_list * src) {
	for (unsigned int i = 1; i <= src->get_number_entries(); i++) {
		dest->add_entry(src->get(i));
	}
};

// delete's all items of the given entry list
void entry_list_free_contents(Elgoog::entry_list * el) {
	for (unsigned int i = 1; i <= el->get_number_entries(); i++) {
		delete el->get(i);
	}
};

int main() {
	/* read query */
	int matched_lines = 0;
	std::ifstream queries;
	queries.open("queries.txt");
	char line[MAX_QUERY_LENGTH];
	queries.getline(line, MAX_QUERY_LENGTH);
	queries.close();

	// std::cout << line << std::endl;
	/* remove duplicates and split into words */
	char * queryStr = dedup(line);
	// std::cout << queryStr << std::endl;

	Elgoog::entry_list list;

	char * word = strtok(queryStr, " ");
	while (word != NULL) {
		// only add words of the given size to the index
		if (MATCHTYPE != Elgoog::match_type::HAMMING ||
			(MATCHTYPE == Elgoog::match_type::HAMMING && strlen(word) == HAMMING_SIZE))
			list.add_entry(new Elgoog::entry(word));
		word = strtok(NULL, " ");
	}

	/* generate index */
	Elgoog::entry_index idx(&list, MATCHTYPE);

	Elgoog::entry_list results;

	/* read documents and search in index */
	std::ifstream input;
	input.open(DOC_FILE);
	char input_line[MAX_DOC_LENGTH];

	int linenum = 1;
	while (input.getline(input_line, MAX_DOC_LENGTH)) {
		/* for each line, remove duplicates and split into words */
		char *input_dedup = dedup(input_line), *backup_for_free = input_dedup;
		bool match = false;
		std::cout << linenum << ": ";

		/* lookup each word in the index */
		char * w = strtok(input_dedup, " ");
		while (w != NULL) {
			// only lookup words of the same size to the index
			if (MATCHTYPE != Elgoog::match_type::HAMMING ||
				(MATCHTYPE == Elgoog::match_type::HAMMING && strlen(w) == HAMMING_SIZE)) {

				Elgoog::entry_list * r = idx.lookup(w, THRESHOLD);
				if (r->get_number_entries() > 0) {
					// append results of this word to the line-result list
					entry_list_merge(&results, r);
					match = true;
					for (unsigned int i = 1; i <= r->get_number_entries();
						 i++) { // print found words
						std::cout << r->get(i)->getWord() << " ";
					}
				}

				delete r; // cleanup
			}
			w = strtok(NULL, " "); // next line
		}
		free(backup_for_free);
		std::cout << std::endl;
		if (match) { matched_lines++; }
		linenum++;
	}

	std::cout << "Matched lines: " << matched_lines << "/" << linenum << std::endl;

	input.close();

	entry_list_free_contents(&list);
	entry_list_free_contents(&results);

	free(queryStr);

	return 0;
}
