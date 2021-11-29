#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include <cstddef>

#include "forward_list.h"
#include "array.h"
#include "../include/core.h"
#include "pair.h"
#include "util.h"

namespace bud
{

template <class Key, class T, class Hash, std::size_t Size>
class unordered_map
{
public:
	using key_type = Key;
	using mapped_type = T;
	using value_type = pair<const key_type, mapped_type>;
	using reference = value_type&;
	using const_reference = const value_type&;
	using hasher = Hash;
	using size_type = std::size_t;

	unordered_map() = default;
	~unordered_map() = default;
	unordered_map(const unordered_map& p) = delete;
	unordered_map(unordered_map&& u) = delete;
	unordered_map& operator=(unordered_map&& u) = delete;

	T* operator[](const key_type& key) const
	{
		size_type index = m_get_hash(key);

		for (auto& item : m_items[index])
		{
			if (compare_values(item.first, key))
				return &(item.second);
		}

		return nullptr;
	}

	pair<T*, bool> insert(const_reference value)
	{
		size_type index = m_get_hash(value.first);

		forward_list<value_type>* bucket_ptr = &(m_items[index]);

		for (auto& item : *bucket_ptr)
		{
			if (compare_values(item.first, value.first))
				return pair(&item.second, false);
		}

		T* return_value = &(bucket_ptr->emplace_front(value).second);

		++m_size;

		return pair(return_value, true);
	}

	size_type erase(const Key& key)
	{
		size_type index = m_get_hash(key);

		forward_list<value_type>* bucket_ptr = &(m_items[index]);

		auto prev_it = bucket_ptr->cbefore_begin();

		for (auto it = bucket_ptr->begin(); it != bucket_ptr->end(); ++it)
		{
			if (compare_values(it->first, key))
			{
				// Used to check if we deleted an element of not.
				auto is_last_it = prev_it;
				++is_last_it;
				++is_last_it;

				auto return_it = bucket_ptr->erase_after(prev_it);

				if (return_it == bucket_ptr->end() && is_last_it != bucket_ptr->end())
					return 0;

				--m_size;
				return 1;
			}

			prev_it = it;
		}

		return 0;
	}

	[[nodiscard]] size_type size() const { return m_size; }
	[[nodiscard]] size_type empty() const { return m_size == 0; }

private:
	size_type m_get_hash(key_type key) const { return m_hash_function(key) % Size; }
	size_type m_size = 0;

	array<forward_list<value_type>, Size> m_items;
	Hash m_hash_function;
};

} // namespace bud

#endif // UNORDERED_MAP_H
