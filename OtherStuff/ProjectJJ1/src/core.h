#ifndef CORE_H
#define CORE_H

/// Maximum document length in characters (4194304).
constexpr int MAX_DOC_LENGTH = (1 << 22);

/// Maximum word length in characters.
constexpr int MAX_WORD_LENGTH = 31;

/// Minimum word length in characters.
constexpr int MIN_WORD_LENGTH = 4;

/// Maximum number of words in a query.
constexpr int MAX_QUERY_WORDS = 5;

/// Maximum query length in characters.
constexpr int MAX_QUERY_LENGTH = ((MAX_WORD_LENGTH + 1) * MAX_QUERY_WORDS);

#endif // CORE_H
