#ifndef _BK_ENTRY_
#define _BK_ENTRY_

#include "BK_T.h"
#include "pair.h"
#include "my_string.h"
#include "vector.h"

using Entry = bud::pair<bud::string, bud::vector<int>>;

class BK_Entry : public BK<Entry>
{
public:
	BK_Entry(int (*distanceFunction)(Entry *,
									 Entry *)); // the distance function finds the distance between two "T"s

	Entry *get(bud::string);
};

#endif
