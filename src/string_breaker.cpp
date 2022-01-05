#include "string_breaker.h"
#include "vector.h"

bud::vector<bud::string> string_breaker(const char*input){
	// if the input is an empty string return immediately
	if(input[0] == '\0')
		return bud::vector<bud::string>();

	bud::unordered_set<bud::string> output_set;
	bud::vector<bud::string> output_vec;

	unsigned int size = 0;

	while(input[size] != '\0'){
		if(input[size] == ' '){
			bud::string temp(input, size);
			if(!output_set[temp]){
				output_set.insert(temp);
				output_vec.emplace_back(std::move(temp));
			}

			input += size+1;

			size = 0;
		}

		size++;
	}

	// we assume there's at least one word
	bud::string temp(input, size);
	if(!output_set[temp]){
		output_vec.emplace_back(std::move(temp));
	}

	return output_vec;
}
