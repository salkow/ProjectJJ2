#ifndef _UTIL_
#define _UTIL_

#define HASH_MAX 50

namespace Util {
	int stringHashFunction(char * str); // takes a string as an argument and returns a number
										// between 0 and (HASH_MAX-1)
};

#endif
