#include "exact_matching_engine.h"

#include "vector.h"
#include "my_string.h"

using bud::string;
using bud::vector;

exact_matching_engine::exact_matching_engine(
	const bud::vector<bud::vector<bud::string*>>& queries) :
	inverted_search_engine(queries)
{
}

vector<int> exact_matching_engine::find(string& word, int /*threshold*/) const
{
	vector<int> empty_vector;

	const vector<int>* matching_queries = m_hash_map->operator[](&word);

	return (matching_queries == nullptr) ? empty_vector : *matching_queries;
}

bud::vector<int> exact_matching_engine::find(string& word) const { return find(word, 0); }
