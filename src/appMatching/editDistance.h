#ifndef editDistance
#define editDistance

#include "../my_string.h"

unsigned long getEdit(bud::string f, bud::string s, unsigned int tolerance);
static int Edistance(bud::string *a, bud::string *b, unsigned int tolerance)
{
	return int(getEdit(*a, *b, tolerance));
}

#endif // !editDistance
