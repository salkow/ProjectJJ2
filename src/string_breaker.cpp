#include "string_breaker.h"

bud::unordered_set<bud::string> string_breaker(const char* input)
{
	// if the input is an empty string return immediately
	if (strlen(input) == 0)
		return bud::unordered_set<bud::string>();

	// copy string
	char* working_string = new char[strlen(input) + 1];
	strcpy(working_string, input);
	size_t orig_len = strlen(working_string);
	bud::unordered_set<bud::string> output;

	unsigned int curs, start = curs = 0;

	// replace all ' ' with '\0' and add to vector
	while (curs < orig_len)
	{
		if (working_string[curs] == ' ')
		{
			working_string[curs] = '\0';
			output.insert(working_string + start);
			start = curs + 1;
		}
		curs++;
	}

	// we assume there's at least one word
	output.insert(working_string + start);

	delete[] working_string;

	return output;
}
