#include "string_breaker.h"

bud::unordered_set<bud::string> string_breaker(const char* input)
{
	// if the input is an empty string return immediately
	if (input[0] == '\0')
		return bud::unordered_set<bud::string>();

	bud::unordered_set<bud::string> output;

	unsigned int curs = 0;
	unsigned int size = 0;

	while (input[curs] != '\0')
	{
		if (input[curs] == ' ')
		{
			output.insert(bud::string(input, size));

			size = 0;

			input += curs + 1;

			curs = 0;
		}

		size++;
		curs++;
	}

	// we assume there's at least one word
	output.insert(input);

	return output;
}
