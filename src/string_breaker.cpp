#include "string_breaker.h"

bud::unordered_set<bud::string> string_breaker(const char* input)
{
	// if the input is an empty string return immediately
	if (input[0] == '\0')
		return bud::unordered_set<bud::string>();

	bud::unordered_set<bud::string> output;

	unsigned int size = 0;

	while (input[size] != '\0')
	{
		if (input[size] == ' ')
		{
			output.insert(bud::string(input, size));

			input += size + 1;

			size = 0;
		}

		size++;
	}

	// we assume there's at least one word
	output.insert(input);

	return output;
}
