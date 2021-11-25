#include "../acutest.h"
#define macro TEST_NO_MAIN

#include "../classes/core.hpp"
#include "../classes/sigmod.hpp"
#include "../classes/list_T.hpp"

#include "../classes/BK_T.hpp"

namespace BK_T {
	static int distance(char * a, char * b) {
		TEST_ASSERT(a != NULL);
		TEST_ASSERT(b != NULL);
		return EditDistance(a, strlen(a), b, strlen(b));
	}

	void insert(void) {
		BK<char> tree(&distance);
		char words[7][MAX_WORD_LENGTH] = {"hell", "help", "fall", "felt", "fell", "small", "melt"};
		for (int i = 0; i < 7; i++) {
			tree.insert(words[i]);
		}
	}

	void search(void) {
		BK<char> tree(&distance);
		char words[7][MAX_WORD_LENGTH] = {"hell", "help", "fall", "felt", "fell", "small", "melt"};
		for (int i = 0; i < 7; i++) {
			tree.insert(words[i]);
		}

		List<char> results;

		char query1[MAX_WORD_LENGTH] = "hell";
		results						 = tree.search(query1, 0);
		TEST_CHECK(strcmp(results.pop(), query1) == 0);

		char query2[MAX_WORD_LENGTH] = "hel1";
		results						 = tree.search(query2, 1);
		TEST_CHECK(results.getSize() == 2);

		char query3[MAX_WORD_LENGTH] = "el";
		results						 = tree.search(query3, 2);
		TEST_CHECK(results.getSize() == 5);
		TEST_MSG("EXPECTED:\nhell\nhelp\nfelt\nfell\nmelt\n");
		TEST_MSG("GOT:");
		while (results.getSize() > 0) {
			TEST_MSG("%s", results.pop());
		}
	}

	static TEST_STRUCT tests[] = {
		{"BK_tree_insert", insert}, {"BK_tree_search", search}, {NULL, NULL}};
	TEST_STRUCT * getTests() { return tests; };
} // namespace BK_T
