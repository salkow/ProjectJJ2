#ifndef INVERTED_SEARCH_ENGINE_H
#define INVERTED_SEARCH_ENGINE_H

#include <cstdio>
#include <exception>
#include <cstdint>

#include "vector.h"
#include "unordered_map.h"
#include "my_string.h"
#include "unique_ptr.h"

enum class match_type
{
	EXACT,
	EDIT_DISTANCE,
	HAMMING_DISTANCE
};

class inverted_search_engine
{
public:
	explicit inverted_search_engine(const bud::vector<bud::vector<bud::string*>>& queries);

	virtual ~inverted_search_engine();

	static inverted_search_engine*
	search_engine_factory(bud::vector<bud::vector<bud::string*>>& queries, match_type match_type);

	[[nodiscard]] virtual bud::vector<int> find(bud::string& word, int threshold) const = 0;
	[[nodiscard]] virtual bud::vector<int> find(bud::string& word) const;

protected:
	void add_queries_to_containers(const bud::vector<bud::vector<bud::string*>>& queries);

	[[nodiscard]] static std::size_t
	count_words_in_queries(const bud::vector<bud::vector<bud::string*>>& queries);

	bud::vector<bud::string*> m_words_from_all_queries;

	bud::unique_ptr<bud::unordered_map<bud::string*, bud::vector<int>, HashFunction>> m_hash_map;
};

#endif // INVERTED_SEARCH_ENGINE_H
