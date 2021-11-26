#ifndef MY_STRING_H
#define MY_STRING_H

#include <cstring>
#include <utility>
#include <exception>
#include <iostream>

#include "iterator.h"

namespace bud
{
using size_type = std::size_t;
constexpr std::size_t CAPACITY_INCREASE_FACTOR = 2;

class string
{
public:
	using value_type = char;
	using iterator = random_access_iterator<value_type>;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;

	string() : m_elem(create_string(0)) {}

	string(size_type count, char ch) :
		m_size(count), m_elem(create_string(count)), m_capacity(count)
	{
		memset(m_elem, ch, count);
	}

	string(const string& other) :
		m_size(other.m_size), m_elem(create_string(other.m_size)), m_capacity(m_size)
	{
		std::memcpy(m_elem, other.m_elem, m_size);
	}

	string(string&& other) noexcept
	{
		m_size = std::exchange(other.m_size, 0);
		m_elem = std::exchange(other.m_elem, nullptr);
		m_capacity = std::exchange(other.m_capacity, 0);
	}

	string(const char* other) :
		m_size(strlen(other)), m_elem(create_string(m_size)), m_capacity(m_size)
	{
		std::memcpy(m_elem, other, m_size);
	}

	constexpr string(std::nullptr_t) = delete;

	string& operator=(string other)
	{
		other.swap(*this);
		return *this;
	}

	string& operator=(const char* other)
	{
		m_size = strlen(other);
		fit(m_size);
		std::memcpy(m_elem, other, m_size);
		m_elem[m_size] = '\0';

		return *this;
	}

	constexpr string& operator=(std::nullptr_t) = delete;

	~string() { delete_string(); }

	[[nodiscard]] constexpr bool empty() const { return m_size == 0; }

	[[nodiscard]] constexpr size_type size() const { return m_size; }
	[[nodiscard]] constexpr size_type length() const { return m_size; }

	[[nodiscard]] iterator begin() const noexcept { return iterator(m_elem); }

	[[nodiscard]] iterator end() const noexcept { return iterator(m_elem + m_size); }

	[[nodiscard]] constexpr reference front() noexcept { return m_elem[0]; }
	[[nodiscard]] constexpr const_reference front() const noexcept { return m_elem[0]; }

	[[nodiscard]] constexpr reference back() noexcept { return m_elem[m_size - 1]; }
	[[nodiscard]] constexpr const_reference back() const noexcept { return m_elem[m_size - 1]; }

	[[nodiscard]] constexpr pointer data() noexcept { return m_elem; }
	[[nodiscard]] constexpr pointer data() const noexcept { return m_elem; }

	void swap(string& other) noexcept
	{
		size_type size_tmp = m_size;
		m_size = other.m_size;
		other.m_size = size_tmp;

		char* elem_tmp = m_elem;
		m_elem = other.m_elem;
		other.m_elem = elem_tmp;

		size_tmp = m_capacity;
		m_capacity = other.m_capacity;
		other.m_capacity = size_tmp;
	}

	bool operator==(const string& other) const
	{
		if (m_size != other.m_size)
			return false;

		return !(std::memcmp(m_elem, other.m_elem, m_size));
	}

	bool operator==(const char* other) const { return !(std::strcmp(m_elem, other)); }

	bool operator!=(const string& other) const { return !(*this == other); }
	bool operator!=(const char* other) const { return !(this->m_elem == other); }

	constexpr reference operator[](size_type pos) noexcept { return m_elem[pos]; }
	constexpr const_reference operator[](size_type pos) const noexcept { return m_elem[pos]; }

	[[nodiscard]] constexpr reference at(size_type pos)
	{
		if (pos < m_size)
			return m_elem[pos];

		throw std::out_of_range("Index out of range");
	}

	[[nodiscard]] constexpr const_reference at(size_type pos) const
	{
		if (pos < m_size)
			return m_elem[pos];

		throw std::out_of_range("Index out of range");
	}

	friend void swap(string& first, string& second) noexcept { first.swap(second); }

	constexpr void reserve(size_type new_cap)
	{
		if (m_capacity < new_cap)
			reallocate(new_cap);
	}

	[[nodiscard]] constexpr size_type capacity() const { return m_capacity; }

	void clear() noexcept
	{
		m_elem[0] = '\0';
		m_size = 0;
	}

	[[nodiscard]] const char* c_str() const { return m_elem; }

	void push_back(char ch)
	{
		if (m_size == m_capacity)
			reallocate(get_increased_capacity());

		m_elem[m_size] = ch;
		m_size++;
		m_elem[m_size] = '\0';
	}

	void pop_back()
	{
		m_elem[m_size - 1] = '\0';
		m_size--;
	}

	friend std::ostream& operator<<(std::ostream& os, const string& str)
	{
		os << str.m_elem;
		return os;
	}

private:
	void reallocate(size_type new_cap)
	{
		auto* tmp_string = create_string(new_cap);
		std::memcpy(tmp_string, m_elem, m_size);

		delete_string();

		m_elem = tmp_string;
		m_elem[m_size] = '\0';
		m_capacity = new_cap;
	}

	void fit(size_type new_cap)
	{
		if (m_capacity < new_cap)
		{
			delete_string();
			m_elem = create_string(new_cap);
			m_capacity = new_cap;
		}
	}

	void delete_string() { delete[] m_elem; }

	[[nodiscard]] static char* create_string(size_type size)
	{
		auto* tmp = new char[size + 1];
		tmp[size] = '\0';
		return tmp;
	}

	[[nodiscard]] constexpr size_type get_increased_capacity() const
	{
		return m_capacity * CAPACITY_INCREASE_FACTOR + 1;
	}

	size_type m_size = 0;
	char* m_elem;
	size_type m_capacity = 0;
};

} // namespace bud

#endif // MY_STRING_H
