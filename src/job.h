#ifndef JOB_H
#define JOB_H

#include <functional>
#include <iostream>

class Job{
public:
	explicit Job(std::function<void()> func);

	bool finished() const;

	void run();

private:
	int m_id;
	std::function<void()> m_func;
	bool m_finished = false;
	static inline int m_idCounter = 0;
};

#endif
