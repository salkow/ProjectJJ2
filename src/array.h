#ifndef ARRAY_H
#define ARRAY_H

#include <type_traits>
#include <utility>
#include <iterator>
#include <exception>

#include "iterator.h"

namespace bud
{

template <class T, size_type N>
struct array
{
	using value_type = T;
	using iterator = T*;
	using const_iterator = const T*;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using size_type = std::size_t;

	reference operator[](size_type pos) { return m_elements[pos]; }
	const_reference operator[](size_type pos) const { return m_elements[pos]; }

	[[nodiscard]] reference at(size_type pos)
	{
		if (pos < N)
			return m_elements[pos];

		throw std::out_of_range("Index out of range");
	}

	[[nodiscard]] const_reference at(size_type pos) const
	{
		if (pos < N)
			return m_elements[pos];

		throw std::out_of_range("Index out of range");
	}

	[[nodiscard]] iterator begin() noexcept { return m_elements; }
	[[nodiscard]] const_iterator begin() const noexcept { return m_elements; }
	[[nodiscard]] const_iterator cbegin() const noexcept { return m_elements; }

	[[nodiscard]] iterator end() noexcept { return m_elements + N; }
	[[nodiscard]] const_iterator end() const noexcept { return m_elements + N; }
	[[nodiscard]] const_iterator cend() const noexcept { return m_elements + N; }

	[[nodiscard]] reference front() { return m_elements[0]; }
	[[nodiscard]] const_reference front() const { return m_elements[0]; }

	[[nodiscard]] reference back() { return m_elements[N - 1]; }
	[[nodiscard]] const_reference back() const { return m_elements[N - 1]; }

	[[nodiscard]] T* data() noexcept { return m_elements; }
	[[nodiscard]] const T* data() const noexcept { return m_elements; }

	[[nodiscard]] bool empty() const { return false; }
	[[nodiscard]] size_type size() const { return N; }

	void fill(const T& value)
	{
		for (auto& i : *this)
			i = value;
	}

	T m_elements[N];
};

template <class T>
struct array<T, 0>
{
	using value_type = T;
	using iterator = T*;
	using const_iterator = const T*;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using size_type = std::size_t;

	reference operator[](size_type /*pos*/) { return m_elements[0]; }
	const_reference operator[](size_type /*pos*/) const { return m_elements[0]; }

	[[nodiscard]] reference at(size_type /*pos*/) { throw std::out_of_range("Index out of range"); }

	[[nodiscard]] const_reference at(size_type /*pos*/) const
	{
		throw std::out_of_range("Index out of range");
	}

	[[nodiscard]] iterator begin() noexcept { return m_elements; }
	[[nodiscard]] const_iterator begin() const noexcept { return m_elements; }
	[[nodiscard]] const_iterator cbegin() const noexcept { return m_elements; }

	[[nodiscard]] iterator end() noexcept { return m_elements; }
	[[nodiscard]] const_iterator end() const noexcept { return m_elements; }
	[[nodiscard]] const_iterator cend() const noexcept { return m_elements; }

	[[nodiscard]] reference front() { return m_elements[0]; }
	[[nodiscard]] const_reference front() const { return m_elements[0]; }

	[[nodiscard]] reference back() { return m_elements[0]; }
	[[nodiscard]] const_reference back() const { return m_elements[0]; }

	[[nodiscard]] T* data() noexcept { return nullptr; }
	[[nodiscard]] const T* data() const noexcept { return nullptr; }

	[[nodiscard]] bool empty() const { return true; }
	[[nodiscard]] size_type size() const { return 0; }

	void fill(const T& /*value*/) const {}

	T m_elements[1];
};

} // namespace bud

#endif // ARRAY_H
