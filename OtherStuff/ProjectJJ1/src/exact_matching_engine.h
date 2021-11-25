#ifndef EXACT_MATCHING_ENGINE_H
#define EXACT_MATCHING_ENGINE_H

#include "inverted_search_engine.h"

#include "vector.h"
#include "my_string.h"

class exact_matching_engine : public inverted_search_engine
{
public:
	explicit exact_matching_engine(const bud::vector<bud::vector<bud::string*>>& queries);

	bud::vector<int> find(bud::string& word) const override;

private:
	bud::vector<int> find(bud::string& word, int threshold) const override;
};
#endif // EXACT_MATCHING_ENGINE_H
