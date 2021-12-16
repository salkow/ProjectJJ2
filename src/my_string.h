#ifndef MY_STRING_H
#define MY_STRING_H

#include <cstring>
#include <utility>
#include <exception>
#include <iostream>

#include "iterator.h"
#include "../include/core.h"

namespace bud
{

class string
{
	constexpr static std::size_t CAPACITY_INCREASE_FACTOR = 2;
	constexpr static std::size_t STARTING_CAPACITY = MAX_WORD_LENGTH;

public:
	using size_type = std::size_t;
	using value_type = char;
	using iterator = random_access_iterator<value_type>;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;

	string();

	string(size_type count, char ch);

	string(const string& other);

	string(string&& other) noexcept;

	string(const char* other);

	string(std::nullptr_t) = delete;

	string& operator=(string other);

	string& operator=(const char* other);

	string& operator=(std::nullptr_t) = delete;

	~string();

	[[nodiscard]] bool empty() const;

	[[nodiscard]] size_type size() const;
	[[nodiscard]] size_type length() const;

	[[nodiscard]] iterator begin() const noexcept;

	[[nodiscard]] iterator end() const noexcept;

	[[nodiscard]] reference front() noexcept;
	[[nodiscard]] const_reference front() const noexcept;

	[[nodiscard]] reference back() noexcept;
	[[nodiscard]] const_reference back() const noexcept;

	[[nodiscard]] pointer data() noexcept;
	[[nodiscard]] pointer data() const noexcept;

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

	bool operator==(const string& other) const;

	bool operator==(const char* other) const;

	bool operator!=(const string& other) const;
	bool operator!=(const char* other) const;

	reference operator[](size_type pos) noexcept;
	const_reference operator[](size_type pos) const noexcept;

	[[nodiscard]] reference at(size_type pos);

	[[nodiscard]] const_reference at(size_type pos) const;

	friend void swap(string& first, string& second) noexcept { first.swap(second); }

	void reserve(size_type new_cap);

	[[nodiscard]] size_type capacity() const;

	void clear() noexcept;

	[[nodiscard]] const char* c_str() const;

	void push_back(char ch);

	void pop_back();

	friend std::ostream& operator<<(std::ostream& os, const string& str)
	{
		os << str.m_elem;
		return os;
	}

private:
	void reallocate(size_type new_cap);

	void fit(size_type new_cap);

	void delete_string();

	[[nodiscard]] static char* create_string(size_type size);

	[[nodiscard]] size_type get_increased_capacity() const;

	size_type m_size = 0;
	char* m_elem;
	size_type m_capacity = STARTING_CAPACITY;
};

} // namespace bud

#endif // MY_STRING_H
