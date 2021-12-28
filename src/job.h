#include <functional>
#include <iostream>

class Job 
{
public:
    Job(std::function<void()> func);
    bool finished() const;
	void run();

private:
    int m_id;
    std::function<void()> m_func;
	bool m_finished = false;
};
