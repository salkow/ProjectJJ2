#ifndef editDistance
#define editDistance

#include "../my_string.h"
#include "../entry.h"

unsigned long getEdit(bud::string &f, bud::string &s, unsigned int tolerance);

static int Edistance(Entry *a, Entry *b, unsigned int tolerance)
{
	if (tolerance == 0)
	{
		return int(getEdit(a->first, b->first, MAX_WORD_LENGTH+1));
	}
	else
		return int(getEdit(a->first, b->first, tolerance));
}

#endif // !editDistance
