#ifndef UNORDERED_SET_H
#define UNORDERED_SET_H

#include <cstddef>
#include <cassert>
#include <utility>

#include "vector.h"
#include "../include/core.h"
#include "pair.h"
#include "hash_function.h"
#include "util.h"

namespace bud
{

template <class Key, class Hash = hash<Key>>
class unordered_set
{
	constexpr static std::size_t DEFAULT_SIZE = 13;
	constexpr static std::size_t DEFAULT_BUCKET_COUNT_INCREASE_FACTOR = 2;
	constexpr static std::size_t DEFAULT_ITEMS_PER_BUCKET = 4;
	constexpr static float DEFAULT_MAX_LOAD_FACTOR = 0.75;

public:
	using key_type = const Key;
	using value_type = key_type;
	using pointer = key_type*;
	using reference = key_type&;
	using const_reference = reference;
	using hasher = Hash;
	using size_type = std::size_t;

	explicit unordered_set(size_type bucket_count = DEFAULT_SIZE, const Hash& hash = Hash()) :
		m_buckets(vector<vector<Key>>(bucket_count)), m_hash_function(hash)
	{
		reserve_buckets();
	}

	~unordered_set() = default;
	unordered_set(unordered_set&& u) noexcept = default;

	unordered_set(const unordered_set& p) = delete;
	unordered_set& operator=(const unordered_set& u) = delete;

	unordered_set& operator=(unordered_set&& u) noexcept
	{
		m_size = std::exchange(u.m_size, 0);
		m_buckets = std::move(u.m_buckets);
		m_hash_function = u.m_hash_function;

		return *this;
	}

	pointer operator[](const_reference key) const
	{
		size_type index = m_get_hash(key);

		const vector<Key>* bucket_with_key = &(m_buckets[index]);

		auto it = find_if(bucket_with_key->begin(), bucket_with_key->end(),
						  [&key](const key_type& item) { return compare_values(item, key); });

		if (it == bucket_with_key->end())
			return nullptr;

		return &(*it);
	}

	pair<pointer, bool> insert(const_reference key) { return x_insert(key); }
	pair<pointer, bool> insert(Key&& key) { return x_insert(std::move(key)); }

	size_type erase(const_reference key)
	{
		size_type index = m_get_hash(key);

		vector<Key>* bucket_with_key = &(m_buckets[index]);

		auto it = find_if(bucket_with_key->begin(), bucket_with_key->end(),
						  [&key](const key_type& item) { return compare_values(item, key); });

		if (it == bucket_with_key->end())
			return 0;

		bucket_with_key->erase(it);

		--m_size;

		return 1;
	}

	void rehash(size_type count)
	{
		vector<vector<Key>> old_buckets = std::move(m_buckets);

		m_buckets = vector<vector<Key>>(count);
		reserve_buckets();

		for (auto& bucket : old_buckets)
		{
			for (auto&& item : bucket)
				insert_after_rehash(std::forward<key_type>(item));
		}
	}

	[[nodiscard]] size_type size() const { return m_size; }
	[[nodiscard]] size_type bucket_count() const { return m_buckets.size(); }
	[[nodiscard]] size_type empty() const { return m_size == 0; }
	[[nodiscard]] float load_factor() const { return m_size / m_buckets.size(); }

	[[nodiscard]] vector<vector<Key>>& data() { return m_buckets; }

private:
	template <class KeyType>
	pair<key_type*, bool> x_insert(KeyType&& key)
	{
		size_type index = m_get_hash(key);

		vector<Key>* bucket_with_key = &(m_buckets[index]);

		auto it = find_if(bucket_with_key->begin(), bucket_with_key->end(),
						  [&key](const key_type& item) { return compare_values(item, key); });

		if (it != bucket_with_key->end())
			return pair(&(*it), false);

		++m_size;

		if (load_factor() > DEFAULT_MAX_LOAD_FACTOR)
		{
			rehash(m_buckets.size() * DEFAULT_BUCKET_COUNT_INCREASE_FACTOR);
			index = m_get_hash(key);
			bucket_with_key = &(m_buckets[index]);
		}

		key_type* return_value = &(bucket_with_key->emplace_back(std::forward<KeyType>(key)));

		return pair(return_value, true);
	}

	void insert_after_rehash(key_type&& value)
	{
		size_type index = m_get_hash(value);

		vector<Key>* bucket_with_key = &(m_buckets[index]);

		bucket_with_key->emplace_back(std::move(value));
	}

	void reserve_buckets()
	{
		for (auto& bucket : m_buckets)
			bucket.reserve(DEFAULT_ITEMS_PER_BUCKET);
	}

	size_type m_get_hash(const_reference key) const
	{
		return m_hash_function(key) % m_buckets.size();
	}

	size_type m_size = 0;
	vector<vector<Key>> m_buckets;
	hasher m_hash_function;
};

} // namespace bud

#endif // UNORDERED_SET_H
