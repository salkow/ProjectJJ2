#include "util.hpp"

#include <cstring>

int Util::stringHashFunction(char * str) {
	int s = 0;

	for (long unsigned int i = 0; i < strlen(str); i++) {
		s += (int) str[i];
	}

	return s % HASH_MAX;
}
