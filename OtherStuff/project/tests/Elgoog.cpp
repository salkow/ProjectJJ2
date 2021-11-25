#include "../acutest.h"

#define macro TEST_NO_MAIN

#include "../classes/elgoog.hpp"
#include "../classes/core.hpp"

#include <cstring>

namespace ELGOOG {

	static void delete_entry_list(Elgoog::entry_list * l) {
		for (unsigned int i = 1; i <= l->get_number_entries(); i++) {
			delete l->get(i);
		}
	}
	static void entry_getWord(void) {
		char word[MAX_WORD_LENGTH] = "hello!";

		Elgoog::entry e(word);

		TEST_CHECK(strcmp(word, e.getWord()) == 0);
	};

	static void entry_list_add(void) {
		char word[3][MAX_WORD_LENGTH] = {"hello!", "how are", "you?"};
		Elgoog::entry e1(word[0]);
		Elgoog::entry e2(word[1]);
		Elgoog::entry e3(word[2]);

		Elgoog::entry_list list;

		Elgoog::error_code error;
		error = list.add_entry(&e1);
		TEST_CHECK(error == Elgoog::error_code::NO_ERROR);
		error = list.add_entry(&e2);
		TEST_CHECK(error == Elgoog::error_code::NO_ERROR);
		error = list.add_entry(&e3);
		TEST_CHECK(error == Elgoog::error_code::NO_ERROR);
	}

	static void entry_list_get_number_entries(void) {
		char word[3][MAX_WORD_LENGTH] = {"hello!", "how are", "you?"};
		Elgoog::entry e1(word[0]);
		Elgoog::entry e2(word[1]);
		Elgoog::entry e3(word[2]);

		Elgoog::entry_list list;

		list.add_entry(&e1);
		TEST_CHECK(list.get_number_entries() == 1);
		list.add_entry(&e2);
		TEST_CHECK(list.get_number_entries() == 2);
		list.add_entry(&e3);
		TEST_CHECK(list.get_number_entries() == 3);
	}

	static void entry_list_get(void) {
		char word[3][MAX_WORD_LENGTH] = {"hello!", "how are", "you?"};
		Elgoog::entry e1(word[0]);
		Elgoog::entry e2(word[1]);
		Elgoog::entry e3(word[2]);

		Elgoog::entry_list list;
		list.add_entry(&e1);
		list.add_entry(&e2);
		list.add_entry(&e3);

		TEST_CHECK(list.get(1) == &e1);
		TEST_CHECK(list.get(2) == &e2);
		TEST_CHECK(list.get(3) == &e3);

		TEST_EXCEPTION(list.get(4), Elgoog::error_code);
	}

	static void entry_index_build(void) {
		char words[3][MAX_WORD_LENGTH] = {"five", "cute", "pups"};

		Elgoog::entry_list list;

		list.add_entry(new Elgoog::entry(words[0]));
		list.add_entry(new Elgoog::entry(words[1]));
		list.add_entry(new Elgoog::entry(words[2]));

		Elgoog::entry_index idx(&list, Elgoog::match_type::EDIT);
		Elgoog::entry_index idx2(&list, Elgoog::match_type::HAMMING);
		Elgoog::entry_index idx3(&list, Elgoog::match_type::EXACT);

		delete_entry_list(&list);
	}

	static void entry_index_lookup(void) {
		char words[6][MAX_WORD_LENGTH] = {"hell", "help", "fall", "felt", "fell", "melt"};

		Elgoog::entry_list list;

		for (int i = 0; i < 6; i++) {
			list.add_entry(new Elgoog::entry(words[i]));
		}

		Elgoog::entry_index idx(&list, Elgoog::match_type::EDIT);
		Elgoog::entry_index idx2(&list, Elgoog::match_type::HAMMING);
		Elgoog::entry_index idx3(&list, Elgoog::match_type::EXACT);

		/* chech edit for word that exists */
		char query1[MAX_WORD_LENGTH] = "hell";
		Elgoog::entry_list * results = idx.lookup(query1, 0);
		TEST_CHECK(results->get_number_entries() == 1);
		TEST_MSG("num entries %d", results->get_number_entries());
		delete_entry_list(results);
		delete results;

		/* check hamming for word that exists */
		results = idx2.lookup(query1, 0);
		TEST_CHECK(results->get_number_entries() == 1);
		TEST_MSG("num entries %d", results->get_number_entries());
		delete_entry_list(results);
		delete results;

		/* chech exact */
		results = idx3.lookup(query1, 0);
		TEST_CHECK(results->get_number_entries() == 1);
		TEST_MSG("num entries %d", results->get_number_entries());
		delete_entry_list(results);
		delete results;

		/* check edit for word that's similar */
		char query2[MAX_WORD_LENGTH] = "hel1";
		results						 = idx.lookup(query2, 1);
		TEST_CHECK(results->get_number_entries() == 2);
		TEST_MSG("num entries %d", results->get_number_entries());
		delete_entry_list(results);
		delete results;

		/* chech hamming for word that's similar */
		results = idx2.lookup(query2, 1);
		TEST_CHECK(results->get_number_entries() == 2);
		TEST_MSG("num entries %d", results->get_number_entries());
		delete_entry_list(results);
		delete results;

		/* check edit for word that's similar but has different size*/
		char query3[MAX_WORD_LENGTH] = "el";
		results						 = idx.lookup(query3, 2);
		TEST_CHECK(results->get_number_entries() == 5);
		TEST_MSG("num entries %d", results->get_number_entries());
		delete_entry_list(results);
		delete results;

		delete_entry_list(&list);
	}

	static TEST_STRUCT tests[] = {{"entry_getWord", entry_getWord},
								  {"entry_list_add", entry_list_add},
								  {"entry_list_get_number_entries", entry_list_get_number_entries},
								  {"entry_list_get", entry_list_get},
								  {"entry_index_build", entry_index_build},
								  {"entry_index_lookup", entry_index_lookup},
								  {NULL, NULL}};
	TEST_STRUCT * getTests() { return tests; };
} // namespace ELGOOG
