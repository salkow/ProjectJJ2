
// #include "stringBreaker.h"

// bud::vector<bud::string> stringBreaker(const char *input)
// {
// 	char *working_string = new char[strlen(input) + 1];
// 	strcpy(working_string, input);
// 	size_t orig_len = strlen(working_string);
// 	bud::vector<bud::string> output;

// 	// char *cursor = working_string;
// 	unsigned int curs, start = curs = 0;

// 	//replace all ' ' with '\0'
// 	while (curs < orig_len)
// 	{
// 		if (working_string[curs] == ' ')
// 		{
// 			working_string[curs] = '\0';
// 			output.emplace_back(working_string + start);
// 			start = curs + 1;
// 		}
// 		curs++;
// 	}
// 	output.emplace_back(working_string + start);

// 	delete[] working_string;

// 	return output;
// }
