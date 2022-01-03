#include "thread.h"
#include "job.h"
// #include "vector_deque.h"
#include <list>
#include "vector.h"
#include "semaphore.h"
#include "unique_ptr.h"
#include "mutex.h"

constexpr int NUM_OF_THREADS = 1;

class JobManager
{
public:
	JobManager();
	void addJob(Job &&j);
	void waitFinishAllJobs();

private:
	// We might need to also pass the thread id.
	static void*run_forever(void*t_job_manager);

	// bud::vector_deque<Job> m_jobs;
	std::list<Job> m_jobs;
	bud::vector<bud::thread> m_threads;
	bud::mutex m_mtx_jobs;
	bud::cond_variable m_cond_jobs_empty;

	bud::mutex m_mtx_terminated;
	bool terminated = false;
};
