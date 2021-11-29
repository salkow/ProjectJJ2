#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <cstddef>
#include <iterator>
#include <utility>

#include "unique_ptr.h"

namespace bud
{

using size_type = std::size_t;

template <class T>
struct forward_list_node
{
	template <typename... Args>
	explicit forward_list_node(unique_ptr<forward_list_node>&& next, Args&&... value) :
		m_next(std::move(next)), m_value(std::forward<Args>(value)...)
	{
	}

	unique_ptr<forward_list_node> m_next;
	T m_value;
};

template <typename T, bool Const = false>
class forward_list_iterator
{
public:
	using value_type = T;
	using reference = typename std::conditional_t<Const, value_type const&, value_type&>;
	using pointer = typename std::conditional_t<Const, value_type const*, value_type*>;
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;

	using node = forward_list_node<T>;
	using node_pointer = node*;

	friend class forward_list_iterator<T, true>;

	template <typename U>
	friend class forward_list;

	forward_list_iterator() = default;

	explicit forward_list_iterator(node_pointer ptr, bool before_begin = false) :
		m_ptr(ptr), m_before_begin(before_begin)
	{
	}

	template <bool Const_ = Const, class = std::enable_if_t<Const_>>
	forward_list_iterator(const forward_list_iterator<T, false>& rhs) :
		m_ptr(rhs.m_ptr), m_before_begin(rhs.m_before_begin)
	{
	}

	forward_list_iterator& operator++()
	{
		if (m_before_begin)
			m_before_begin = false;
		else
			m_ptr = m_ptr->m_next.get();

		return *this;
	}

	forward_list_iterator operator++(int)
	{
		forward_list_iterator it(*this);
		++*this;
		return it;
	}

	template <bool Const_ = Const>
	std::enable_if_t<Const_, reference> operator*() const
	{
		return m_ptr->m_value;
	}

	template <bool Const_ = Const>
	std::enable_if_t<!Const_, reference> operator*()
	{
		return m_ptr->m_value;
	}

	template <bool Const_ = Const>
	std::enable_if_t<Const_, pointer> operator->() const
	{
		return *(m_ptr->m_value);
	}

	template <bool Const_ = Const>
	std::enable_if_t<!Const_, pointer> operator->()
	{
		return *(m_ptr->m_value);
	}

	friend bool operator==(const forward_list_iterator& lhs, const forward_list_iterator& rhs)
	{
		return lhs.m_ptr == rhs.m_ptr && lhs.m_before_begin == rhs.m_before_begin;
	}

	friend bool operator!=(const forward_list_iterator& lhs, const forward_list_iterator& rhs)
	{
		return !(lhs == rhs);
	}

private:
	node_pointer m_ptr = nullptr;
	bool m_before_begin = false;
};

template <class T>
class forward_list
{
	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;
	using node = forward_list_node<T>;
	using iterator = forward_list_iterator<T>;
	using const_iterator = forward_list_iterator<T, true>;

public:
	forward_list() = default;

	forward_list(forward_list&& other) noexcept : m_head(std::move(other.m_head)) {}

	forward_list(const forward_list& other) = delete;

	~forward_list() { clear(); }

	forward_list& operator=(forward_list&& other) noexcept
	{
		m_head = std::move(other.m_head);
		return *this;
	}

	forward_list& operator=(const forward_list& other) = delete;

	void clear() noexcept { m_head.reset(); }

	void push_front(const_reference item) { emplace_front(item); }

	void push_front(T&& item) { emplace_front(std::move(item)); }

	template <typename... Args>
	reference emplace_front(Args&&... args)
	{
		m_head = make_unique<node>(std::move(m_head), std::forward<Args>(args)...);

		return m_head->m_value;
	}

	void pop_front()
	{
		unique_ptr tmp = std::move(m_head->m_next);
		m_head = std::move(std::move(tmp));
	}

	iterator insert_after(const_iterator pos, const T& value) { return emplace_after(pos, value); }

	iterator insert_after(const_iterator pos, T&& value)
	{
		return emplace_after(pos, std::move(value));
	}

	template <class... Args>
	iterator emplace_after(const_iterator pos, Args&&... args)
	{
		if (pos.m_before_begin)
		{
			emplace_front(std::forward<Args>(args)...);
			return begin();
		}

		pos.m_ptr->m_next =
			make_unique<node>(std::move(pos.m_ptr->m_next), std::forward<Args>(args)...);

		forward_list_iterator it(pos.m_ptr);
		return ++it;
	}

	[[nodiscard]] bool empty() const noexcept { return !static_cast<bool>(m_head); }

	[[nodiscard]] reference front() { return m_head->m_value; }
	[[nodiscard]] const_reference front() const { return m_head->m_value; }

	[[nodiscard]] iterator begin() const noexcept { return iterator(m_head.get()); }
	[[nodiscard]] const_iterator cbegin() const noexcept { return const_iterator(m_head.get()); }

	[[nodiscard]] iterator before_begin() const noexcept { return iterator(m_head.get(), true); }
	[[nodiscard]] const_iterator cbefore_begin() const noexcept
	{
		return const_iterator(m_head.get(), true);
	}

	[[nodiscard]] iterator end() const noexcept { return iterator(); }
	[[nodiscard]] const_iterator cend() const noexcept { return const_iterator(); }

private:
	unique_ptr<node> m_head = nullptr;
};

} // namespace bud

#endif // FORWARD_LIST_H
