#include "job.h"

Job::Job(std::function<void()> func)
	: m_func(func)
{
	m_id = ++m_idCounter;
}

bool Job::finished() const
{
	return m_finished;
}

void Job::run()
{
	m_func();
	m_finished = true;
}
