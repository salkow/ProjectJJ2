#ifndef FILE_READER_H
#define FILE_READER_H

#include <cstddef>

#include "core.h"
#include "vector.h"
#include "my_string.h"

namespace bud
{

using size_type = std::size_t;

const size_type SAME_WORDS_PER_DOCUMENT_FACTOR = 15;

class file_reader
{
public:
	explicit file_reader(const char* file_path);

	~file_reader();

	bud::vector<bud::string> read_unique_words();

	bud::vector<bud::vector<bud::string*>> read_queries();

private:
	size_type calculate_file_size();

	[[nodiscard]] size_type get_apr_num_of_words() const;

	[[nodiscard]] size_type get_apr_num_of_queries() const;

	[[nodiscard]] size_type get_apr_num_of_words_per_query() const;

	[[nodiscard]] size_type get_apr_num_of_unique_words() const;

	FILE* m_file;
	size_type m_file_size;
};

} // namespace bud

#endif // FILE_READER_H
