#ifndef ITERATOR_H
#define ITERATOR_H

#include <iterator>
#include <cstddef>

namespace bud
{
using size_type = std::size_t;

template <typename T>
class my_iterator
{
public:
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using difference_type = std::ptrdiff_t;

	my_iterator() = default;
	explicit my_iterator(pointer ptr) : m_ptr(ptr) {}

	my_iterator& operator++()
	{
		m_ptr++;
		return *this;
	}

	my_iterator operator++(int)
	{
		my_iterator it(*this);
		++m_ptr;
		return it;
	}

	friend bool operator==(const my_iterator& lhs, const my_iterator& rhs)
	{
		return lhs.m_ptr == rhs.m_ptr;
	}

	friend bool operator!=(const my_iterator& lhs, const my_iterator& rhs)
	{
		return lhs.m_ptr != rhs.m_ptr;
	}

	pointer operator->() { return m_ptr; }
	const_pointer operator->() const { return m_ptr; }

	reference operator*() { return *m_ptr; }
	const_reference operator*() const { return *m_ptr; }

protected:
	T* m_ptr = nullptr;
};

template <typename T>
class forward_iterator : public my_iterator<T>
{
public:
	forward_iterator() = default;
	explicit forward_iterator(typename my_iterator<T>::pointer ptr) : my_iterator<T>(ptr) {}
};

template <typename T>
class random_access_iterator : public forward_iterator<T>
{
public:
	random_access_iterator() = default;
	explicit random_access_iterator(typename my_iterator<T>::pointer ptr) : forward_iterator<T>(ptr)
	{
	}

	random_access_iterator& operator--()
	{
		--this->m_ptr;
		return *this;
	}

	random_access_iterator operator--(int)
	{
		random_access_iterator it(*this);
		--this->m_ptr;
		return it;
	}

	random_access_iterator& operator+=(size_type offset)
	{
		this->m_ptr += offset;
		return *this;
	}

	random_access_iterator& operator-=(size_type offset)
	{
		this->m_ptr -= offset;
		return *this;
	}

	friend bool operator<(const random_access_iterator& lhs, const random_access_iterator& rhs)
	{
		return lhs.m_ptr < rhs.m_ptr;
	}

	friend bool operator<=(const random_access_iterator& lhs, const random_access_iterator& rhs)
	{
		return lhs.m_ptr < rhs.m_ptr;
	}

	friend bool operator>(const random_access_iterator& lhs, const random_access_iterator& rhs)
	{
		return lhs.m_ptr < rhs.m_ptr;
	}

	friend bool operator>=(const random_access_iterator& lhs, const random_access_iterator& rhs)
	{
		return lhs.m_ptr < rhs.m_ptr;
	}

	friend random_access_iterator operator+(const random_access_iterator& it, size_type offset)
	{
		random_access_iterator new_it(it);
		return new_it += offset;
	}

	friend random_access_iterator operator-(const random_access_iterator& it, size_type offset)
	{
		random_access_iterator new_it(it);
		return new_it -= offset;
	}

	friend typename my_iterator<T>::difference_type operator-(const random_access_iterator& lhs,
															  const random_access_iterator& rhs)
	{
		return lhs.m_ptr - rhs.m_ptr;
	}

	typename my_iterator<T>::reference operator[](size_type index)
	{
		return *(this->m_ptr + index);
	}

	typename my_iterator<T>::const_reference operator[](size_type index) const
	{
		return *(this->m_ptr + index);
	}
};

} // namespace bud

#endif // ITERATOR_H
