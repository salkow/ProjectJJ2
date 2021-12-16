#include "my_string.h"

bud::string::string() : m_elem(create_string(STARTING_CAPACITY)) {}

bud::string::string(bud::size_type count, char ch) :
	m_size(count), m_elem(create_string(count)), m_capacity(count)
{
	memset(m_elem, ch, count);
}

bud::string::string(const bud::string& other) :
	m_size(other.m_size), m_elem(create_string(other.m_size)), m_capacity(m_size)
{
	std::memcpy(m_elem, other.m_elem, m_size);
}

bud::string::string(bud::string&& other) noexcept
{
	m_size = std::exchange(other.m_size, 0);
	m_elem = std::exchange(other.m_elem, nullptr);
	m_capacity = std::exchange(other.m_capacity, 0);
}

bud::string::string(const char* other) :
	m_size(strlen(other)), m_elem(create_string(m_size)), m_capacity(m_size)
{
	std::memcpy(m_elem, other, m_size);
}

bud::string::string(const char* other, size_type count) :
	m_size(count), m_elem(create_string(count)), m_capacity(count)
{
	std::memcpy(m_elem, other, count);
}

bud::string& bud::string::operator=(bud::string other)
{
	other.swap(*this);
	return *this;
}

bud::string& bud::string::operator=(const char* other)
{
	m_size = strlen(other);
	fit(m_size);
	std::memcpy(m_elem, other, m_size);
	m_elem[m_size] = '\0';

	return *this;
}

bud::string::~string() { delete_string(); }

bool bud::string::empty() const { return m_size == 0; }

bud::size_type bud::string::size() const { return m_size; }

bud::size_type bud::string::length() const { return m_size; }

bud::string::iterator bud::string::begin() const noexcept { return iterator(m_elem); }

bud::string::iterator bud::string::end() const noexcept { return iterator(m_elem + m_size); }

bud::string::value_type& bud::string::front() noexcept { return m_elem[0]; }

const bud::string::value_type& bud::string::front() const noexcept { return m_elem[0]; }

bud::string::value_type& bud::string::back() noexcept { return m_elem[m_size - 1]; }

const bud::string::value_type& bud::string::back() const noexcept { return m_elem[m_size - 1]; }

bud::string::pointer bud::string::data() noexcept { return m_elem; }

bud::string::pointer bud::string::data() const noexcept { return m_elem; }

bool bud::string::operator==(const bud::string& other) const
{
	if (m_size != other.m_size)
		return false;

	return !(std::memcmp(m_elem, other.m_elem, m_size));
}

bool bud::string::operator==(const char* other) const { return !(std::strcmp(m_elem, other)); }

bool bud::string::operator!=(const bud::string& other) const { return !(*this == other); }

bool bud::string::operator!=(const char* other) const { return !(this->m_elem == other); }

bud::string::value_type& bud::string::operator[](bud::size_type pos) noexcept
{
	return m_elem[pos];
}

const bud::string::value_type& bud::string::operator[](bud::size_type pos) const noexcept
{
	return m_elem[pos];
}

bud::string::value_type& bud::string::at(bud::size_type pos)
{
	if (pos < m_size)
		return m_elem[pos];

	throw std::out_of_range("Index out of range");
}

const bud::string::value_type& bud::string::at(bud::size_type pos) const
{
	if (pos < m_size)
		return m_elem[pos];

	throw std::out_of_range("Index out of range");
}

void bud::string::reserve(bud::size_type new_cap)
{
	if (m_capacity < new_cap)
		reallocate(new_cap);
}

bud::size_type bud::string::capacity() const { return m_capacity; }

void bud::string::clear() noexcept
{
	m_elem[0] = '\0';
	m_size = 0;
}

const char* bud::string::c_str() const { return m_elem; }

void bud::string::push_back(char ch)
{
	if (m_size == m_capacity)
		reallocate(get_increased_capacity());

	m_elem[m_size] = ch;
	m_size++;
	m_elem[m_size] = '\0';
}

void bud::string::pop_back()
{
	m_elem[m_size - 1] = '\0';
	m_size--;
}

void bud::string::reallocate(bud::size_type new_cap)
{
	auto* tmp_string = create_string(new_cap);
	std::memcpy(tmp_string, m_elem, m_size);

	delete_string();

	m_elem = tmp_string;
	m_elem[m_size] = '\0';
	m_capacity = new_cap;
}

void bud::string::fit(bud::size_type new_cap)
{
	if (m_capacity < new_cap)
	{
		delete_string();
		m_elem = create_string(new_cap);
		m_capacity = new_cap;
	}
}

void bud::string::delete_string() { delete[] m_elem; }

char* bud::string::create_string(bud::size_type size)
{
	auto* tmp = new char[size + 1];
	tmp[size] = '\0';
	return tmp;
}

bud::size_type bud::string::get_increased_capacity() const
{
	return m_capacity * CAPACITY_INCREASE_FACTOR + 1;
}
