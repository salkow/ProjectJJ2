#include "thread.h"
#include "job.h"
// #include "vector_deque.h"
#include <list>
#include "vector.h"
#include "semaphore.h"
#include "unique_ptr.h"
#include "mutex.h"

constexpr int NUM_OF_THREADS = 4;

class JobManager
{
public:
	JobManager();
	void addJob(Job &&j);
	void waitFinishAllJobs();

private:
	// We might need to also pass the thread id.
	[[noreturn]] static void*run_forever(void*t_job_manager){
		JobManager*job_manager = static_cast<JobManager*>(t_job_manager);

		bud::unique_ptr<Job> job;

		// Instead of true, use a variable to stop the thread at the end.
		while(true){
			// Mutex here.
			job = nullptr;
			job_manager->m_mtx_jobs.lock();
			if(!job_manager->m_jobs.empty()){
				*job = job_manager->m_jobs.front();
				job_manager->m_jobs.pop_front();
			}else{
				job_manager->m_cond_jobs_empty.signal();
			}
			job_manager->m_mtx_jobs.unlock();
			if(job){
				job->run(); //only run job if it's a fresh one
			}
		}

		return 0;
	}

	// bud::vector_deque<Job> m_jobs;
	std::list<Job> m_jobs;
	bud::vector<bud::thread> m_threads;
	bud::mutex m_mtx_jobs;
	bud::cond_variable m_cond_jobs_empty;
};
