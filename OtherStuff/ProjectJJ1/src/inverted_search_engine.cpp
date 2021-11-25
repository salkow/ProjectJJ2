#include "inverted_search_engine.h"

#include "bkTrees/bkTree.h"
#include "my_string.h"
#include "vector.h"
#include "exact_matching_engine.h"
#include "unordered_map.h"

using bud::pair;
using bud::string;
using bud::vector;

using query_vector = bud::vector<bud::string*>;

inverted_search_engine::inverted_search_engine(const vector<query_vector>& queries)
{
	if (queries.empty())
		throw std::invalid_argument("Queries vector can't by empty.");

	std::size_t num_of_words = count_words_in_queries(queries);

	m_words_from_all_queries.reserve(num_of_words);

	m_hash_map = bud::unique_ptr(
		new bud::unordered_map<bud::string*, bud::vector<int>, HashFunction>(num_of_words));

	add_queries_to_containers(queries);
}

inverted_search_engine::~inverted_search_engine()
{
	for (auto* word : m_words_from_all_queries)
		delete word;
}

bud::vector<int> inverted_search_engine::find(bud::string& word) const { return find(word, 0); }

void inverted_search_engine::add_queries_to_containers(const bud::vector<query_vector>& queries)
{
	int query_index = 0;

	for (const auto& query : queries)
	{
		for (auto* word : query)
		{
			vector<int>* queries_with_that_word = (*m_hash_map)[word];

			if (queries_with_that_word == nullptr)
			{
				m_words_from_all_queries.emplace_back(word);

				vector<int> new_queries_with_that_word;
				new_queries_with_that_word.emplace_back(query_index);
				m_hash_map->insert(
					pair<string* const, vector<int>>(word, std::move(new_queries_with_that_word)));
			}

			else
			{
				queries_with_that_word->emplace_back(query_index);
				delete word;
			}
		}

		query_index++;
	}
}

inverted_search_engine*
inverted_search_engine::search_engine_factory(bud::vector<query_vector>& queries, match_type type)
{
	if (type == match_type::EXACT)
		return new exact_matching_engine(queries);

	return new bkTree(queries, type);
}

std::size_t inverted_search_engine::count_words_in_queries(const vector<query_vector>& queries)
{
	std::size_t sum = 0;

	for (const auto& query : queries)
	{
		sum += query.size();
	}

	return sum;
}
