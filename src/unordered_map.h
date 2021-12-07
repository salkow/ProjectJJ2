#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include <cstddef>
#include <cassert>

#include "vector.h"
#include "../include/core.h"
#include "pair.h"
#include "hash_function.h"
#include "util.h"

namespace bud
{

template <class Key, class T, class Hash = hash<Key>>
class unordered_map
{
	constexpr static std::size_t DEFAULT_SIZE = 13;
	constexpr static std::size_t DEFAULT_BUCKET_COUNT_INCREASE_FACTOR = 2;
	constexpr static std::size_t DEFAULT_ITEMS_PER_BUCKET = 4;
	constexpr static float DEFAULT_MAX_LOAD_FACTOR = 0.75;

public:
	using key_type = Key;
	using mapped_type = T;
	using value_type = pair<const key_type, mapped_type>;
	using reference = value_type&;
	using const_reference = const value_type&;
	using hasher = Hash;
	using size_type = std::size_t;

	explicit unordered_map(size_type bucket_count = DEFAULT_SIZE, const Hash& hash = Hash()) :
		m_buckets(vector<vector<value_type>>(bucket_count)), m_hash_function(hash)
	{
		reserve_buckets();
	}

	unordered_map(unordered_map&& u) noexcept :
		m_size(std::exchange(u.m_size, 0)), m_buckets(std::move(u.m_buckets)),
		m_hash_function(u.hash_function)
	{
	}

	unordered_map& operator=(unordered_map&& u) noexcept
	{
		m_size = std::exchange(u.m_size, 0);
		m_buckets = std::move(u.m_buckets);
		m_hash_function = u.m_hash_function;

		return *this;
	}

	~unordered_map() = default;
	unordered_map(const unordered_map& p) = delete;
	unordered_map& operator=(const unordered_map& u) = delete;

	T* operator[](const key_type& key) const
	{
		size_type index = m_get_hash(key);

		const vector<value_type>* bucket_with_key = &(m_buckets[index]);

		auto it =
			find_if(bucket_with_key->begin(), bucket_with_key->end(),
					[&key](const value_type& item) { return compare_values(item.first, key); });

		if (it == bucket_with_key->end())
			return nullptr;

		return &(it->second);
	}

	template <class... Args>
	pair<T*, bool> try_emplace(const Key& key, Args&&... args)
	{
		return x_try_emplace(key, std::forward<Args>(args)...);
	}

	template <class... Args>
	pair<T*, bool> try_emplace(Key&& key, Args&&... args)
	{
		return x_try_emplace(std::move(key), std::forward<Args>(args)...);
	}

	size_type erase(const key_type& key)
	{
		size_type index = m_get_hash(key);

		vector<value_type>* bucket_with_key = &(m_buckets[index]);

		auto it =
			find_if(bucket_with_key->begin(), bucket_with_key->end(),
					[&key](const value_type& item) { return compare_values(item.first, key); });

		if (it == bucket_with_key->end())
			return 0;

		bucket_with_key->erase(it);

		--m_size;

		return 1;
	}

	void rehash(size_type count)
	{
		vector<vector<value_type>> old_buckets = std::move(m_buckets);

		m_buckets = vector<vector<value_type>>(count);
		reserve_buckets();

		for (auto& bucket : old_buckets)
		{
			for (auto&& item : bucket)
				insert_after_rehash(std::forward<value_type>(item));
		}
	}

	[[nodiscard]] size_type size() const { return m_size; }
	[[nodiscard]] size_type bucket_count() const { return m_buckets.size(); }
	[[nodiscard]] size_type empty() const { return m_size == 0; }
	[[nodiscard]] float load_factor() const { return m_size / m_buckets.size(); }

	[[nodiscard]] vector<vector<value_type>>& data() { return m_buckets; }

private:
	template <class KeyType, class... Args>
	pair<T*, bool> x_try_emplace(KeyType&& key, Args&&... args)
	{
		size_type index = m_get_hash(key);

		vector<value_type>* bucket_with_key = &(m_buckets[index]);

		auto it =
			find_if(bucket_with_key->begin(), bucket_with_key->end(),
					[&key](const value_type& item) { return compare_values(item.first, key); });

		if (it != bucket_with_key->end())
			return pair(&(it->second), false);

		++m_size;

		if (load_factor() > DEFAULT_MAX_LOAD_FACTOR)
		{
			rehash(m_buckets.size() * DEFAULT_BUCKET_COUNT_INCREASE_FACTOR);
			index = m_get_hash(key);
			bucket_with_key = &(m_buckets[index]);
		}

		T* return_value =
			&(bucket_with_key->emplace_back(std::forward<KeyType>(key), std::forward<Args>(args)...)
				  .second);

		return pair(return_value, true);
	}

	void insert_after_rehash(value_type&& value)
	{
		size_type index = m_get_hash(value.first);

		vector<value_type>* bucket_with_key = &(m_buckets[index]);

		bucket_with_key->emplace_back(std::move(value));
	}

	void reserve_buckets()
	{
		for (auto& bucket : m_buckets)
			bucket.reserve(DEFAULT_ITEMS_PER_BUCKET);
	}

	size_type m_get_hash(key_type key) const { return m_hash_function(key) % m_buckets.size(); }
	size_type m_size = 0;

	vector<vector<value_type>> m_buckets;
	hasher m_hash_function;
};

} // namespace bud

#endif // UNORDERED_MAP_H
