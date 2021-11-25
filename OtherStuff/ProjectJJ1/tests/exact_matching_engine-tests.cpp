#include "../lib/include/catch2/catch.hpp"

#include "../src/exact_matching_engine.h"
#include "../src/vector.h"
#include "../src/my_string.h"
#include "../src/file_reader.h"
#include "../src/unique_ptr.h"

using bud::file_reader;
using bud::string;
using bud::unique_ptr;
using bud::vector;

TEST_CASE("Test word not in any query.", "[word_not_in_any_query]")
{
	file_reader query_file_reader("../queries/query_3.txt");

	vector<vector<string*>> queries = query_file_reader.read_queries();

	auto engine =
		unique_ptr(inverted_search_engine::search_engine_factory(queries, match_type::EXACT));

	file_reader document_file_reader("../documents/document_3.txt");

	vector<string> document_words = document_file_reader.read_unique_words();

	vector<int> found_queries = engine->find(document_words[0]);

	REQUIRE(found_queries.empty());
}

TEST_CASE("Test words in queries.", "[words_in_queries]")
{
	file_reader query_file_reader("../queries/query_4.txt");

	vector<vector<string*>> queries = query_file_reader.read_queries();

	auto engine =
		unique_ptr(inverted_search_engine::search_engine_factory(queries, match_type::EXACT));

	file_reader document_file_reader("../documents/document_4.txt");

	vector<string> document_words = document_file_reader.read_unique_words();

	vector<int> found_queries = engine->find(document_words[0]);
	REQUIRE(found_queries.size() == 2);
	REQUIRE(found_queries[0] == 0);
	REQUIRE(found_queries[1] == 4);

	found_queries = engine->find(document_words[1]);
	REQUIRE(found_queries.size() == 2);
	REQUIRE(found_queries[0] == 1);
	REQUIRE(found_queries[1] == 4);

	found_queries = engine->find(document_words[2]);
	REQUIRE(found_queries.size() == 2);
	REQUIRE(found_queries[0] == 2);
	REQUIRE(found_queries[1] == 4);

	found_queries = engine->find(document_words[3]);
	REQUIRE(found_queries.empty());
}

TEST_CASE("Test with a lot of words exact_matching.", "[lots_of_words_exact]")
{
	file_reader query_file_reader("../queries/query_7.txt");

	vector<vector<string*>> queries = query_file_reader.read_queries();

	auto engine =
		unique_ptr(inverted_search_engine::search_engine_factory(queries, match_type::EXACT));

	file_reader document_file_reader("../documents/document_5.txt");

	vector<string> document_words = document_file_reader.read_unique_words();

	std::size_t sum = 0;

	for (auto& word : document_words)
		sum += engine->find(word).size();

	REQUIRE(sum == 393);

	vector<int> matching_queries = engine->find(document_words[530]);
	REQUIRE(matching_queries.size() == 1);
	REQUIRE(matching_queries[0] == 1629);
}
