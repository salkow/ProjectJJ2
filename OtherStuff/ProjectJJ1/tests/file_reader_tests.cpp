#include "../lib/include/catch2/catch.hpp"

#include <cstdio>

#include "../src/file_reader.h"
#include "../src/vector.h"
#include "../src/my_string.h"

using bud::file_reader;
using bud::string;
using bud::vector;

void delete_vector_of_vectors_of_strings(const vector<vector<string*>>& my_vec);

void delete_vector_of_vectors_of_strings(const vector<vector<string*>>& my_vec)
{
	for (const auto& in_vec : my_vec)
	{
		for (auto* word : in_vec)
		{
			delete word;
		}
	}
}

TEST_CASE("Read empty file and convert contents to vector.", "[empty_file_to_vector]")
{
	file_reader my_file_reader("../queries/query_0.txt");

	vector<vector<string*>> queries = my_file_reader.read_queries();

	REQUIRE(queries.empty());
}

TEST_CASE("Read file with one item and convert contents to vector.",
		  "[file_with_one_item_to_vector]")
{
	file_reader my_file_reader("../queries/query_1.txt");

	vector<vector<string*>> queries = my_file_reader.read_queries();

	REQUIRE(queries.size() == 1);

	REQUIRE(*(queries[0][0]) == string("this"));

	delete_vector_of_vectors_of_strings(queries);
}

TEST_CASE("Read file query with multiple words and convert contents to vector.",
		  "[file_one_query_words_to_vector]")
{
	file_reader my_file_reader("../queries/query_2.txt");

	vector<vector<string*>> queries = my_file_reader.read_queries();

	REQUIRE(queries.size() == 2);

	REQUIRE(queries[0].size() == 3);
	REQUIRE(*(queries[0][0]) == string("these"));
	REQUIRE(*(queries[0][1]) == string("are"));
	REQUIRE(*(queries[0][2]) == string("some"));

	REQUIRE(queries[1].size() == 2);
	REQUIRE(*(queries[1][0]) == string("different"));
	REQUIRE(*(queries[1][1]) == string("words"));

	delete_vector_of_vectors_of_strings(queries);
}

TEST_CASE("Read empty document.", "[read_empty_document]")
{
	file_reader my_file_reader("../documents/document_0.txt");

	vector<string> document_words = my_file_reader.read_unique_words();

	REQUIRE(document_words.size() == 0);
}

TEST_CASE("Read document with no duplicate words.", "[read_document_no_duplicate]")
{
	file_reader my_file_reader("../documents/document_1.txt");

	vector<string> document_words = my_file_reader.read_unique_words();

	REQUIRE(document_words.size() == 3);

	REQUIRE(document_words[0] == bud::string("word"));
	REQUIRE(document_words[1] == bud::string("another"));
	REQUIRE(document_words[2] == bud::string("text"));
}

TEST_CASE("Read document with duplicate words.", "[read_document_duplicate]")
{
	file_reader my_file_reader("../documents/document_2.txt");

	vector<string> document_words = my_file_reader.read_unique_words();

	REQUIRE(document_words.size() == 6);

	REQUIRE(document_words[0] == bud::string("this"));
	REQUIRE(document_words[1] == bud::string("document"));
	REQUIRE(document_words[2] == bud::string("contains"));
	REQUIRE(document_words[3] == bud::string("many"));
	REQUIRE(document_words[4] == bud::string("duplicate"));
	REQUIRE(document_words[5] == bud::string("words"));
}
