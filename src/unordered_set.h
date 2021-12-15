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

template <typename Key, bool Const = false>
class unordered_set_iterator
{
	using vec_iter = typename vector<Key>::iterator;
	using bucket_iter = typename vector<vector<Key>>::iterator;

public:
	using value_type = Key;
	using reference = typename std::conditional_t<Const, value_type const&, value_type&>;
	using pointer = typename std::conditional_t<Const, value_type const*, value_type*>;

	unordered_set_iterator() = default;
	unordered_set_iterator(const vector<vector<value_type>>* buckets, const bucket_iter& pos) :
		m_buckets(buckets), m_bucket_it(pos), m_vec_it(m_buckets->begin()->begin())
	{
		if (m_bucket_it == m_buckets->end())
			return;

		if (m_vec_it != m_buckets->begin()->end())
			return;

		set_next_it();
	}

	template <bool Const_ = Const, class = std::enable_if_t<Const_>>
	unordered_set_iterator(const unordered_set_iterator<value_type, false>& rhs) :
		m_buckets(rhs.m_buckets), m_bucket_it(rhs.m_bucket_it), m_vec_it(rhs.m_vec_it)
	{
	}

	unordered_set_iterator operator++()
	{
		++m_vec_it;
		return set_next_it();
	};

	unordered_set_iterator operator++(int)
	{
		unordered_set_iterator it(*this);
		++*this;
		return it;
	}

	template <bool Const_ = Const>
	std::enable_if_t<Const_, reference> operator*() const
	{
		return *m_vec_it;
	}

	template <bool Const_ = Const>
	std::enable_if_t<!Const_, reference> operator*()
	{
		return *m_vec_it;
	}

	template <bool Const_ = Const>
	std::enable_if_t<Const_, pointer> operator->() const
	{
		return m_vec_it;
	}

	template <bool Const_ = Const>
	std::enable_if_t<!Const_, pointer> operator->()
	{
		return m_vec_it;
	}

	friend bool operator==(const unordered_set_iterator& lhs, const unordered_set_iterator& rhs)
	{
		return lhs.m_buckets == rhs.m_buckets && lhs.m_bucket_it == rhs.m_bucket_it;
	}

	friend bool operator!=(const unordered_set_iterator& lhs, const unordered_set_iterator& rhs)
	{
		return !(lhs == rhs);
	}

private:
	unordered_set_iterator set_next_it()
	{
		while (true)
		{
			if (m_vec_it == m_bucket_it->end())
			{
				++m_bucket_it;

				if (m_bucket_it == m_buckets->end())
					return *this;

				m_vec_it = m_bucket_it->begin();
			}

			else
				return *this;
		}
	}

	const vector<vector<value_type>>* m_buckets;
	bucket_iter m_bucket_it;
	vec_iter m_vec_it;
};

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
	using iterator = unordered_set_iterator<Key>;
	using const_iterator = unordered_set_iterator<Key, true>;

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

	void clear()
	{
		for (auto& bucket: m_buckets)
			bucket.clear();

		m_size = 0;
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

	void merge(const unordered_set<Key, Hash>& other)
	{
		for (const auto& key : other)
			insert(key);
	}

	void merge(unordered_set<Key, Hash>&& other)
	{
		for (auto&& key : other)
			insert(std::forward<Key>(key));
	}

	[[nodiscard]] size_type size() const { return m_size; }
	[[nodiscard]] size_type bucket_count() const { return m_buckets.size(); }
	[[nodiscard]] size_type empty() const { return m_size == 0; }
	[[nodiscard]] float load_factor() const { return m_size / m_buckets.size(); }

	[[nodiscard]] iterator begin() const noexcept
	{
		return iterator(&m_buckets, m_buckets.begin());
	}

	[[nodiscard]] const_iterator cbegin() const noexcept
	{
		return const_iterator(&m_buckets, m_buckets.begin());
	}

	[[nodiscard]] iterator end() const noexcept { return iterator(&m_buckets, m_buckets.end()); }

	[[nodiscard]] const_iterator cend() const noexcept
	{
		return const_iterator(&m_buckets, m_buckets.end());
	}

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
