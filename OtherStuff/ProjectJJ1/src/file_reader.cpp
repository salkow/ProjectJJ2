#include "file_reader.h"

#include <cstdint>
#include <system_error>
#include <cstdio>

#include "unordered_map.h"

bud::file_reader::file_reader(const char* file_path) : m_file(fopen(file_path, "r"))
{
	if (m_file == nullptr)
		throw std::invalid_argument("File path is incorrect.");

	m_file_size = calculate_file_size();
}

bud::file_reader::~file_reader() { fclose(m_file); }

bud::vector<bud::string> bud::file_reader::read_unique_words()
{
	bud::vector<bud::string> unique_words;
	unique_words.reserve(get_apr_num_of_unique_words());

	bud::unordered_map<bud::string, bool, HashFunction> hash_map(get_apr_num_of_unique_words() + 1);

	char* buffer = nullptr;

	while (true)
	{
		size_t len = MAX_WORD_LENGTH + 1;

		ssize_t chars_read = getline(&buffer, &len, m_file);
		if (chars_read == -1)
			break;

		buffer[chars_read - 1] = '\0';

		const bud::string new_word(buffer);

		if (hash_map[new_word] == nullptr)
		{
			unique_words.emplace_back(buffer);
			hash_map.insert(bud::pair<const bud::string, bool>(new_word, true));
		}
	}

	free(buffer);

	return unique_words;
}

bud::vector<bud::vector<bud::string*>> bud::file_reader::read_queries()
{
	vector<vector<string*>> queries_vector;
	queries_vector.reserve(get_apr_num_of_queries());

	vector<string*> query;
	query.reserve(get_apr_num_of_words_per_query());

	char* buffer = nullptr;

	while (true)
	{
		size_t len = MAX_WORD_LENGTH + 1;

		ssize_t chars_read = getline(&buffer, &len, m_file);
		if (chars_read == -1)
		{
			// If empty, the file had no data.
			if (!query.empty())
				queries_vector.emplace_back(query);

			break;
		}

		if (buffer[0] != '\n')
			buffer[chars_read - 1] = '\0';

		// We found a new query.
		else
		{
			queries_vector.emplace_back(std::move(query));

			query = vector<string*>();
			query.reserve(get_apr_num_of_words_per_query());

			continue;
		}

		auto* new_string = new string(buffer);
		query.emplace_back(new_string);
	}

	free(buffer);

	return queries_vector;
}

bud::size_type bud::file_reader::calculate_file_size()
{
	int result = fseek(m_file, 0L, SEEK_END);
	if (result == -1)
		throw std::system_error();

	int64_t size = ftell(m_file);

	result = fseek(m_file, 0L, SEEK_SET);
	if (result == -1)
		throw std::system_error();

	return static_cast<size_t>(size);
}

bud::size_type bud::file_reader::get_apr_num_of_words() const
{
	return m_file_size / (MAX_WORD_LENGTH / 2);
}

bud::size_type bud::file_reader::get_apr_num_of_queries() const
{
	return (m_file_size / (MAX_QUERY_LENGTH / 2)) + 1;
}

bud::size_type bud::file_reader::get_apr_num_of_words_per_query() const
{
	return get_apr_num_of_words() / (get_apr_num_of_queries() + 1);
}

bud::size_type bud::file_reader::get_apr_num_of_unique_words() const
{
	return get_apr_num_of_words() / SAME_WORDS_PER_DOCUMENT_FACTOR;
}
