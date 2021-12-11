#ifndef _BK_ENTRY_
#define _BK_ENTRY_

#include "BK_T.h"
#include "pair.h"
#include "my_string.h"
#include "unordered_set.h"
#include "query.h"
#include "entry.h"

class BK_Entry : public BK<Entry>
{
	bud::vector<bud::pair<Entry *, int>> recSearch(Node *parent, bud::string query,
												   int maxDistance) const; // recursively searches the tree for words that
																		   // have <= maxDistance to the given query

public:
	BK_Entry(int (*distanceFunction)(Entry *,
									 Entry *, unsigned int)); // the distance function finds the distance between two "T"s

	Entry *get(bud::string);
	bud::vector<bud::pair<Entry *, int>> search(bud::string query, int maxDistance) const; // searches the tree for words that
																						   // have <= maxDistance to the given query
};

#endif
