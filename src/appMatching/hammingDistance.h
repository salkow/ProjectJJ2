#ifndef hammingDistance
#define hammingDistance

#include "../my_string.h"
#include "../entry.h"

unsigned long getHamming(bud::string f, bud::string s, unsigned int tol);
static int Hdistance(Entry *a, Entry *b, unsigned int tolerance)
{
	return int(getHamming(a->first, b->first, tolerance));
}
#endif // !hammingDistance
