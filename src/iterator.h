#ifndef ITERATOR_H
#define ITERATOR_H

#include <iterator>
#include <cstddef>

namespace bud
{
using size_type = std::size_t;

template <typename T, bool Const = false>
class my_iterator
{
public:
	using value_type = T;
	using reference = typename std::conditional_t<Const, value_type const&, value_type&>;
	using pointer = typename std::conditional_t<Const, value_type const*, value_type*>;
	using difference_type = std::ptrdiff_t;

	friend class my_iterator<T, true>;

	my_iterator() = default;
	explicit my_iterator(T* ptr) : m_ptr(ptr) {}

	template <bool Const_ = Const, class = std::enable_if_t<Const_>>
	my_iterator(const my_iterator<T, false>& rhs) : m_ptr(rhs.m_ptr)
	{
	}

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

	template <bool Const_ = Const>
	std::enable_if_t<Const_, pointer> operator->() const
	{
		return m_ptr;
	}

	template <bool Const_ = Const>
	std::enable_if_t<!Const_, pointer> operator->()
	{
		return m_ptr;
	}

	template <bool Const_ = Const>
	std::enable_if_t<Const_, reference> operator*() const
	{
		return *m_ptr;
	}

	template <bool Const_ = Const>
	std::enable_if_t<!Const_, reference> operator*()
	{
		return *m_ptr;
	}

protected:
	T* m_ptr = nullptr;
};

template <typename T, bool Const = false>
class forward_iterator : public my_iterator<T, Const>
{
public:
	using pointer = typename my_iterator<T, Const>::pointer;
	friend class forward_iterator<T, true>;

	forward_iterator() = default;
	explicit forward_iterator(T* ptr) : my_iterator<T, Const>(ptr) {}

	template <bool Const_ = Const, class = std::enable_if_t<Const_>>
	forward_iterator(const forward_iterator<T, false>& rhs) : my_iterator<T, Const_>(rhs.m_ptr)
	{
	}
};

template <typename T, bool Const = false>
class random_access_iterator : public forward_iterator<T, Const>
{
public:
	using pointer = typename my_iterator<T, Const>::pointer;
	using reference = typename my_iterator<T, Const>::reference;
	using difference_type = typename my_iterator<T, Const>::difference_type;

	friend class random_access_iterator<T, true>;

	random_access_iterator() = default;
	explicit random_access_iterator(T* ptr) : forward_iterator<T, Const>(ptr) {}

	template <bool Const_ = Const, class = std::enable_if_t<Const_>>
	random_access_iterator(const random_access_iterator<T, false>& rhs) :
		forward_iterator<T, Const_>(rhs.m_ptr)
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
		return random_access_iterator(it.m_ptr + offset);
	}

	friend random_access_iterator operator-(const random_access_iterator& it, size_type offset)
	{
		return random_access_iterator(it.m_ptr - offset);
	}

	friend difference_type operator-(const random_access_iterator& lhs,
									 const random_access_iterator& rhs)
	{
		return lhs.m_ptr - rhs.m_ptr;
	}

	reference operator[](size_type index) { return *(this->m_ptr + index); }

	reference operator[](size_type index) const { return *(this->m_ptr + index); }
};

} // namespace bud

#endif // ITERATOR_H
