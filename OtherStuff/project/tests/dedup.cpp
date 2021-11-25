#include "../acutest.h"

#define macro TEST_NO_MAIN

#include "../classes/dedup.hpp"

#include <cstring>

namespace DEDUP {
	static void wordsWithDups(void) {
		char input[MAX_DOC_LENGTH] = "a word another word a third word yey ! ! !";
		char * output			   = dedup(input);
		TEST_CHECK(strlen(output) == 26);
		free(output);
	};

	static void wordsWithoutDups(void) {
		char input[MAX_DOC_LENGTH] = "five very unique words !";
		size_t input_lenght		   = strlen(input);
		char * output			   = dedup(input);
		TEST_CHECK(strlen(output) == input_lenght);
		free(output);
	};

	static void emptyStr(void) {
		char input[MAX_DOC_LENGTH] = "";
		char * output			   = dedup(input);
		TEST_CHECK(strlen(output) == 0);
		free(output);
	};

	static TEST_STRUCT tests[] = {{"words_with_dups", wordsWithDups},
								  {"words_without_dups", wordsWithoutDups},
								  {"empty_string", emptyStr},
								  {NULL, NULL}};
	TEST_STRUCT * getTests() { return tests; };
} // namespace DEDUP
