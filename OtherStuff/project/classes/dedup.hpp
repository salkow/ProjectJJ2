#ifndef _DEDUP_
#define _DEDUP_

#include "core.hpp"

// takes a string of words separated by spaces and returns a string with the same words, separated
// by spaces but without the duplicates. Returns a char* with the size of [MAX_DOC_LENGTH] that must
// be free'd when you're done using it
char * dedup(char * input);

#endif
