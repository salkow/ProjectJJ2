#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#include "thread.h"
#include "job.h"
#include "vector.h"
#include "mutex.h"
#include "list.h"

constexpr int NUM_OF_THREADS = 2;

class JobManager{
public:
	JobManager();


	void addJob(Job&& j);

	void waitFinishAllJobs();

	void terminate();

private:
	// We might need to also pass the thread id.
	static void*run_forever(void*t_job_manager);

	static bool should_terminate(JobManager*t_job_manager);

	// bud::vector_deque<Job> m_jobs;
	bud::list<bud::unique_ptr<Job>> m_jobs;
	bud::vector<bud::thread> m_threads;
	bud::mutex m_mtx_jobs;
	bud::cond_variable m_cond_jobs_empty;      //there are no jobs in the m_jobs list
	bud::cond_variable m_cond_jobs_not_empty; //there are jobs in the m_jobs list

	bud::mutex m_mtx_terminated;
	bool terminated = false;
	int m_num_of_running_jobs = 0;
	bud::mutex m_mtx_running_jobs;
	bud::cond_variable m_cond_running_jobs; //no running jobs
};

#endif