#ifndef LIST_H
#define LIST_H

#include <cstddef>
#include <iterator>
#include <utility>

#include "unique_ptr.h"

namespace bud
{

using size_type = std::size_t;

template <class T>
struct list_node
{
	template <typename... Args>
	explicit list_node(list_node* t_prev, unique_ptr<list_node>&& t_next, Args&&... value) :
		prev(std::move(t_prev)), next(std::move(t_next)), m_value(std::forward<Args>(value)...)
	{
	}

	list_node* prev;
	unique_ptr<list_node> next;

	T m_value;
};

template <typename T, bool Const = false>
class list_iterator
{
public:
	using value_type = T;
	using reference = typename std::conditional_t<Const, value_type const&, value_type&>;
	using pointer = typename std::conditional_t<Const, value_type const*, value_type*>;

	using node = list_node<T>;
	using node_pointer = node*;

	friend class list_iterator<T, true>;

	template <typename U>
	friend class list;

	list_iterator() = default;

	explicit list_iterator(node_pointer ptr, node_pointer tail) : m_ptr(ptr), m_tail(tail) {}

	explicit list_iterator(node_pointer tail) : m_tail(tail) {}

	template <bool Const_ = Const, class = std::enable_if_t<Const_>>
	list_iterator(const list_iterator<T, false>& rhs) : m_ptr(rhs.m_ptr), m_tail(rhs.m_tail)
	{
	}

	list_iterator& operator++()
	{
		m_ptr = m_ptr->next.get();

		return *this;
	}

	list_iterator operator++(int)
	{
		list_iterator it(*this);
		++*this;
		return it;
	}

	list_iterator& operator--()
	{
		if (!m_ptr)
			m_ptr = m_tail;

		return *this;
	}

	list_iterator operator--(int)
	{
		list_iterator it(*this);
		--*this;
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
		return &m_ptr->m_value;
	}

	template <bool Const_ = Const>
	std::enable_if_t<!Const_, pointer> operator->()
	{
		return &m_ptr->m_value;
	}

	friend bool operator==(const list_iterator& lhs, const list_iterator& rhs)
	{
		return lhs.m_ptr == rhs.m_ptr;
	}

	friend bool operator!=(const list_iterator& lhs, const list_iterator& rhs)
	{
		return !(lhs == rhs);
	}

private:
	node_pointer m_ptr = nullptr;
	node_pointer m_tail = nullptr;
};

template <class T>
class list
{
public:
	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;
	using node = list_node<T>;
	using iterator = list_iterator<T>;
	using const_iterator = list_iterator<T, true>;
	using iterator_category = std::bidirectional_iterator_tag;

	list() = default;
	~list() = default;

	list(list&& other) noexcept = default;

	list(const list& other) = delete;
	list& operator=(const list& other) = delete;

	void clear() noexcept
	{
		m_size = 0;
		m_head.reset();
		m_tail = nullptr;
	}

	bool empty() const noexcept { return m_size == 0; }

	size_type size() const noexcept { return m_size; }

	void push_front(const_reference item) { emplace_front(item); }

	void push_front(T&& item) { emplace_front(std::move(item)); }

	template <typename... Args>
	reference emplace_front(Args&&... args)
	{
		m_head = make_unique<node>(nullptr, std::move(m_head), std::forward<Args>(args)...);

		if (!m_tail)
			m_tail = m_head.get();

		if (m_head->next)
			m_head->next->prev = m_head.get();

		++m_size;

		return m_head->m_value;
	}

	void pop_front()
	{
		m_head = std::move(m_head->next);

		--m_size;

		if (!m_head)
			m_tail = nullptr;
		else
			m_head->prev = nullptr;
	}

	void push_back(const_reference item) { emplace_back(item); }

	void push_back(T&& item) { emplace_back(std::move(item)); }

	template <typename... Args>
	reference emplace_back(Args&&... args)
	{
		unique_ptr<node> new_node = make_unique<node>(m_tail, nullptr, std::forward<Args>(args)...);

		if (m_tail)
		{
			m_tail->next = std::move(new_node);
			m_tail = m_tail->next.get();
		}

		else
		{
			m_head = std::move(new_node);
			m_tail = m_head.get();
		}

		++m_size;

		return m_tail->m_value;
	}

	void pop_back()
	{
		if (m_tail->prev)
		{
			// m_tail->prev->next.reset();
			m_tail = m_tail->prev;
			m_tail->next.reset();
			--m_size;
		}

		else
			clear();
	}

	iterator insert(const_iterator pos, const T& value) { return emplace(pos, value); }

	iterator insert(const_iterator pos, T&& value) { return emplace(pos, std::move(value)); }

	template <class... Args>
	iterator emplace(const_iterator pos, Args&&... args)
	{
		++m_size;

		if (!pos.m_ptr)
		{
			unique_ptr<node> new_value =
				make_unique<node>(m_tail, nullptr, std::forward<Args>(args)...);
			m_tail = new_value.get();

			if (!m_head)
			{
				m_head = std::move(new_value);

				return iterator(m_head.get());
			}

			else
			{
				m_tail->prev->next = std::move(new_value);

				return iterator(m_tail->prev->next.get());
			}
		}

		else if (pos.m_ptr->prev)
		{
			pos.m_ptr->prev->next = make_unique<node>(
				pos.m_ptr->prev, std::move(pos.m_ptr->prev->next), std::forward<Args>(args)...);

			return iterator(pos.m_ptr->prev->next.get());
		}

		else
		{
			m_head = make_unique<node>(nullptr, std::move(m_head), std::forward<Args>(args)...);
			m_head->next->prev = m_head.get();

			return iterator(m_head.get());
		}
	}

	iterator erase(const_iterator pos)
	{
		--m_size;

		if (!pos.m_ptr->prev)
		{
			m_head = std::move(m_head->next);

			if (m_head)
			{
				m_head->prev = nullptr;

				return iterator(m_head.get(), m_tail);
			}

			return iterator(m_tail);
		}

		else
		{
			node* prev_elem = pos.m_ptr->prev;
			pos.m_ptr->prev->next = std::move(pos.m_ptr->next);

			if (prev_elem->next)
			{
				prev_elem->next->prev = prev_elem;
				return iterator(prev_elem->next.get(), m_tail);
			}

			return iterator(m_tail);
		}
	}

	[[nodiscard]] reference front() { return m_head->m_value; }
	[[nodiscard]] const_reference front() const { return m_head->m_value; }

	[[nodiscard]] reference back() { return m_tail->m_value; }
	[[nodiscard]] const_reference back() const { return m_tail->m_value; }

	[[nodiscard]] iterator begin() const noexcept { return iterator(m_head.get(), m_tail); }
	[[nodiscard]] const_iterator cbegin() const noexcept
	{
		return const_iterator(m_head.get(), m_tail);
	}

	[[nodiscard]] iterator end() const noexcept { return iterator(m_tail); }
	[[nodiscard]] const_iterator cend() const noexcept { return const_iterator(m_tail); }

private:
	unique_ptr<node> m_head = nullptr;
	node* m_tail = nullptr;
	size_type m_size = 0;
};

} // namespace bud

#endif // LIST_H
