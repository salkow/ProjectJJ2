#include "../lib/include/catch2/catch.hpp"

#include "../src/my_string.h"
#include "../src/bkTrees/bkTree.h"
#include "../src/file_reader.h"
#include <iostream>
#include <stdexcept>

using bud::file_reader;
using bud::string;
using bud::vector;

TEST_CASE("simple bk test", "[bk_tests2]")
{
	file_reader query_file_reader("../queries/query_5.txt");

	vector<vector<string*>> queries = query_file_reader.read_queries();

	try
	{
		inverted_search_engine* engine =
			inverted_search_engine::search_engine_factory(queries, match_type::EDIT_DISTANCE);
		// vector<string*> p = temp.find(string("ops"), 2);
		bud::string ss("ops");
		vector<int> p = engine->find(ss, 2);

		REQUIRE(p.size() == 2);

		delete engine;
	}
	catch (const std::invalid_argument& ia)
	{
		std::cerr << ia.what() << std::endl;
	}
}

TEST_CASE("more bk test", "[bk_tests3]")
{
	file_reader query_file_reader("../queries/query_5.txt");

	vector<vector<string*>> queries = query_file_reader.read_queries();

	try
	{
		inverted_search_engine* engine =
			inverted_search_engine::search_engine_factory(queries, match_type::EDIT_DISTANCE);
		bud::string ss("helt");
		vector<int> p = engine->find(ss, 2);

		REQUIRE(p.size() == 6);

		delete engine;
	}
	catch (const std::invalid_argument& ia)
	{
		std::cerr << ia.what() << std::endl;
	}
}

TEST_CASE("hamming simple bk test", "[bk_testsΗ2]")
{
	file_reader query_file_reader("../queries/query_6.txt");

	vector<vector<string*>> queries = query_file_reader.read_queries();

	try
	{
		inverted_search_engine* engine =
			inverted_search_engine::search_engine_factory(queries, match_type::HAMMING_DISTANCE);
		bud::string ss("opss");
		vector<int> p = engine->find(ss, 2);
		REQUIRE(p.size() == 1);

		delete engine;
	}
	catch (const std::invalid_argument& ia)
	{
		std::cerr << ia.what() << std::endl;
	}
}

TEST_CASE("hamming more bk test", "[bk_testsΗ3]")
{
	file_reader query_file_reader("../queries/query_6.txt");

	vector<vector<string*>> queries = query_file_reader.read_queries();

	try
	{
		inverted_search_engine* engine =
			inverted_search_engine::search_engine_factory(queries, match_type::HAMMING_DISTANCE);
		bud::string ss("helt");
		vector<int> p = engine->find(ss, 2);
		REQUIRE(p.size() == 5);

		delete engine;
	}
	catch (const std::invalid_argument& ia)
	{
		std::cerr << ia.what() << std::endl;
	}
}


TEST_CASE("bk test sigmod data", "[bk_tests4]")
{
	file_reader query_file_reader("../queries/query_7.txt");
	vector<vector<string*>> queries = query_file_reader.read_queries();

	file_reader document_file_reader("../documents/document_5.txt");
	vector<string> document_words = document_file_reader.read_unique_words();

	long unsigned int sum = 0;
	try
	{
		inverted_search_engine* engine =
			inverted_search_engine::search_engine_factory(queries, match_type::EDIT_DISTANCE);

		for(auto x: document_words){
			vector<int> p = engine->find(x, 2);
			sum+= p.size();
		}


		REQUIRE(sum == 6302);

		delete engine;
	}
	catch (const std::invalid_argument& ia)
	{
		std::cerr << ia.what() << std::endl;
	}
}

TEST_CASE("hamming bk test sigmod data", "[bk_testsΗ4]")
{
	file_reader query_file_reader("../queries/query_7.txt");
	vector<vector<string*>> queries = query_file_reader.read_queries();

	file_reader document_file_reader("../documents/document_5.txt");
	vector<string> document_words = document_file_reader.read_unique_words();

	long unsigned int sum = 0;

	try
	{
		inverted_search_engine* engine =
			inverted_search_engine::search_engine_factory(queries, match_type::HAMMING_DISTANCE);
		
		for(auto x: document_words){
			vector<int> p = engine->find(x, 2);
			sum+= p.size();
		}

		REQUIRE(sum == 31815);

		delete engine;
	}
	catch (const std::invalid_argument& ia)
	{
		std::cerr << ia.what() << std::endl;
	}
}