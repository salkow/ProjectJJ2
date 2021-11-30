#ifndef VECTOR_H
#define VECTOR_H

#include <type_traits>
#include <utility>
#include <exception>

#include "iterator.h"

namespace bud
{

template <class T>
class vector
{
	const std::size_t CAPACITY_INCREASE_FACTOR = 2;

public:
	using value_type = T;
	using iterator = random_access_iterator<T>;
	using const_iterator = random_access_iterator<T, true>;
	using pointer = T*;
	using const_pointer = T const*;
	using reference = value_type&;
	using const_reference = value_type const&;
	using size_type = std::size_t;

	vector() = default;

	vector(size_type num_of_elements, const T& value)
	{
		fit(num_of_elements);
		for (size_type i = 0; i < num_of_elements; ++i)
			emplace_back(value);
	}

	explicit vector(size_type num_of_elements)
	{
		fit(num_of_elements);
		for (size_type i = 0; i < num_of_elements; ++i)
			emplace_back(T());
	}

	vector(vector&& other) noexcept :
		m_elements(std::exchange(other.m_elements, nullptr)),
		m_capacity(std::exchange(other.m_capacity, 0)), m_size(std::exchange(other.m_size, 0))
	{
	}

	vector(vector const& other) :
		m_elements(allocate_new_blocks(other.m_size)), m_capacity(other.m_size),
		m_size(other.m_size)
	{
		for (size_type i = 0; i < m_size; ++i)
			new (&m_elements[i]) T(other.m_elements[i]);
	}

	~vector()
	{
		destruct_elements();
		::operator delete(m_elements);
	}

	vector& operator=(vector other)
	{
		other.swap(*this);
		return *this;
	}

	bool operator==(vector const& other) const
	{
		if (m_size != other.m_size)
			return false;

		auto this_it = this->begin();
		auto other_it = other.begin();

		for (; this_it != this->end(); ++this_it, ++other_it)
		{
			if (*this_it != *other_it)
				return false;
		}

		return true;
	}

	bool operator!=(vector const& other) const { return !(*this == other); }

	reference operator[](size_type pos) noexcept { return m_elements[pos]; }
	const_reference operator[](size_type pos) const noexcept { return m_elements[pos]; }

	[[nodiscard]] reference at(size_type pos)
	{
		if (pos < m_size)
			return m_elements[pos];

		throw std::out_of_range("Index out of range");
	}

	[[nodiscard]] const_reference at(size_type pos) const
	{
		if (pos < m_size)
			return m_elements[pos];

		throw std::out_of_range("Index out of range");
	}

	void swap(vector& other) noexcept
	{
		T* tmp_m_elements = m_elements;
		m_elements = other.m_elements;
		other.m_elements = tmp_m_elements;

		size_type tmp_size_type = m_capacity;
		m_capacity = other.m_capacity;
		other.m_capacity = tmp_size_type;

		tmp_size_type = m_size;
		m_size = other.m_size;
		other.m_size = tmp_size_type;
	}

	friend void swap(vector& a, vector& b) noexcept { a.swap(b); }

	void reserve(size_type new_cap)
	{
		if (m_capacity < new_cap)
			reallocate(new_cap);
	}

	void push_back(const T& value) { emplace_back(value); }

	void push_back(T&& value) { emplace_back(std::move(value)); }

	template <class... Args>
	reference emplace_back(Args&&... args)
	{
		if (m_size == m_capacity)
			reallocate(get_increased_capacity());

		new (&m_elements[m_size]) T(std::forward<Args>(args)...);

		return m_elements[m_size++];
	}

	void insert(iterator first, iterator last)
	{
		size_type new_capacity = static_cast<size_type>(last - first) + m_size;

		reserve(new_capacity);

		for (; first != last; first++)
			emplace_back(*first);
	}

	void pop_back() noexcept
	{
		--m_size;
		m_elements[m_size].~T();
	}

	void clear() noexcept
	{
		destruct_elements();
		m_size = 0;
	}

	iterator erase(const_iterator pos)
	{
		auto offset = static_cast<size_t>(pos - begin());

		move_items_in_block(offset, m_size - 1);

		pop_back();

		return begin() + offset;
	}

	[[nodiscard]] iterator begin() const noexcept { return iterator(m_elements); }
	[[nodiscard]] const_iterator cbegin() const noexcept { return const_iterator(m_elements); }

	[[nodiscard]] iterator end() const noexcept { return iterator(m_elements + m_size); }
	[[nodiscard]] const_iterator cend() const noexcept
	{
		return const_iterator(m_elements + m_size);
	}

	[[nodiscard]] reference front() noexcept { return m_elements[0]; }
	[[nodiscard]] const_reference front() const noexcept { return m_elements[0]; }

	[[nodiscard]] reference back() noexcept { return m_elements[m_size - 1]; }
	[[nodiscard]] const_reference back() const noexcept { return m_elements[m_size - 1]; }

	[[nodiscard]] pointer data() noexcept { return m_elements; }
	[[nodiscard]] const_pointer data() const noexcept { return m_elements; }

	[[nodiscard]] size_type size() const noexcept { return m_size; }
	[[nodiscard]] size_type capacity() const noexcept { return m_capacity; }
	[[nodiscard]] size_type empty() const noexcept { return m_size == 0; }

private:
	void reallocate(size_type new_cap)
	{
		T* new_block = allocate_new_blocks(new_cap);
		transfer_items_to_new_block(new_block);

		destruct_elements();
		::operator delete(m_elements);

		m_elements = new_block;
		m_capacity = new_cap;
	}

	void fit(size_type new_cap)
	{
		m_elements = allocate_new_blocks(new_cap);
		m_capacity = new_cap;
	}

	void transfer_items_to_new_block(T* new_block)
	{
		if constexpr (std::is_nothrow_move_constructible_v<T>)
		{
			for (size_type i = 0; i < m_size; ++i)
				new (&new_block[i]) T(std::move(m_elements[i]));
		}

		else
		{
			for (size_type i = 0; i < m_size; ++i)
				new (&new_block[i]) T(m_elements[i]);
		}
	}

	void move_items_in_block(size_type start, size_type end)
	{
		if constexpr (std::is_nothrow_move_constructible_v<T>)
		{
			for (size_type i = start; i < end; ++i)
			{
				m_elements[i].~T();
				new (&m_elements[i]) T(std::move(m_elements[i + 1]));
			}
		}

		else
		{
			for (size_type i = start; i < end; ++i)
			{
				m_elements[i].~T();
				new (&m_elements[i]) T(m_elements[i + 1]);
			}
		}
	}

	void destruct_elements() const
	{
		if constexpr (!std::is_trivially_destructible_v<T>)
		{
			for (auto& i : *this)
				i.~T();
		}
	}

	[[nodiscard]] size_type get_increased_capacity() const
	{
		return m_capacity * CAPACITY_INCREASE_FACTOR + 1;
	}

	[[nodiscard]] T* allocate_new_blocks(size_type size) const
	{
		return static_cast<T*>(::operator new(sizeof(T) * size));
	}

	T* m_elements = nullptr;
	size_type m_capacity = 0;
	size_type m_size = 0;
};

} // namespace bud

#endif // VECTOR_H
