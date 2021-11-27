#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <utility>

namespace bud
{

template <typename T>
class unique_ptr
{
public:
	using pointer = T*;

	unique_ptr() noexcept = default;
	explicit unique_ptr(pointer p) : m_data(p) {}
	unique_ptr(std::nullptr_t) noexcept {}

	unique_ptr(unique_ptr&& u) noexcept : m_data(std::exchange(u.m_data, nullptr)) {}

	template <class U>
	unique_ptr(unique_ptr<U>&& u) noexcept
	{
		unique_ptr<T> tmp(u.release());

		m_data = std::exchange(tmp.m_data, nullptr);
	}

	unique_ptr& operator=(unique_ptr&& r) noexcept
	{
		reset(r.get());
		r.m_data = nullptr;
		return *this;
	}

	template <class U>
	unique_ptr& operator=(unique_ptr<U>&& r) noexcept
	{
		unique_ptr<T> tmp(r.release());
		reset(tmp.release());

		return *this;
	}

	unique_ptr& operator=(std::nullptr_t) noexcept
	{
		reset();
		return *this;
	}

	unique_ptr(const unique_ptr& p) = delete;
	unique_ptr& operator=(const unique_ptr& r) = delete;

	T& operator*() const noexcept { return *m_data; }
	pointer operator->() const noexcept { return m_data; }

	explicit operator bool() const noexcept { return m_data; }

	~unique_ptr() { delete m_data; }
    
    friend bool operator==(const unique_ptr& lhs, const unique_ptr& rhs)
    {
        return lhs.m_data == rhs.m_data;
    }

    friend bool operator==(const unique_ptr& lhs, std::nullptr_t) noexcept
    {
        return lhs.m_data == nullptr;
    }

    friend bool operator!=(const unique_ptr& lhs, const unique_ptr& rhs)
    {
        return lhs.m_data != rhs.m_data;
    }

    friend bool operator!=(const unique_ptr& lhs, std::nullptr_t) noexcept
    {
        return lhs.m_data != nullptr;
    }

	[[nodiscard]] pointer get() const noexcept { return m_data; }

	[[nodiscard]] pointer release() noexcept
	{
		pointer tmp = m_data;
		m_data = nullptr;
		return tmp;
	}

	void reset(pointer ptr = pointer()) noexcept
	{
		delete m_data;
		m_data = ptr;
	}

private:
	T* m_data = nullptr;
};

template <class T, class... Args>
unique_ptr<T> make_unique(Args&&... args)
{
	return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

} // namespace bud

#endif // UNIQUE_PTR_H
