#ifndef editDistance
#define editDistance

#include "../my_string.h"
#include "../entry.h"

unsigned long getEdit(bud::string& f, bud::string& s, unsigned int tolerance);

static int Edistance(Entry* a, Entry* b, unsigned int tolerance)
{
	return int(getEdit(a->first, b->first, tolerance));
}

#endif // !editDistance
