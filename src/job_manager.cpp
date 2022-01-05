#include "job_manager.h"

JobManager::JobManager()
{
	// m_jobs.reserve(NUM_OF_THREADS);
	m_threads.reserve(NUM_OF_THREADS);

	for (int i = 0; i < NUM_OF_THREADS; i++)
	{
		m_threads.emplace_back(bud::thread(&JobManager::run_forever, this));
	}
}

void JobManager::addJob(Job&& j){
	m_mtx_jobs.lock();
	m_jobs.emplace_back(new Job(j));
	m_mtx_jobs.unlock();
	m_cond_jobs_not_empty.signal();
}

void JobManager::waitFinishAllJobs(){
	m_mtx_jobs.lock();
	while(m_jobs.size() != 0){
		m_cond_jobs_empty.wait(m_mtx_jobs);
	}
	m_mtx_jobs.unlock();

	m_mtx_running_jobs.lock();
	while(m_num_of_running_jobs != 0){
		m_cond_jobs_empty.wait(m_mtx_running_jobs);
	}
	m_mtx_running_jobs.unlock();
}

bool JobManager::should_terminate(JobManager*t_job_manager){
	t_job_manager->m_mtx_terminated.lock();
	bool retval = t_job_manager->terminated;
	t_job_manager->m_mtx_terminated.unlock();
	return retval;
}

void *JobManager::run_forever(void *t_job_manager)
{
	JobManager *job_manager = static_cast<JobManager *>(t_job_manager);

	bud::unique_ptr<Job> job;

	// Instead of true, use a variable to stop the thread at the end.
	while (true)
	{
		// Mutex here.
		job = nullptr;
		job_manager->m_mtx_jobs.lock();
		if (!job_manager->m_jobs.empty())
		{
			job = std::move(job_manager->m_jobs.front());
			job_manager->m_jobs.pop_front();
		}
		else
		{
			job_manager->m_cond_jobs_empty.signal();
			//check if we have to terminate, if not wait for a job
			//each time we unblock, check again if we have to terminate
			bool terminated = should_terminate(job_manager);
			while (!terminated && job_manager->m_jobs.empty())
			{
				job_manager->m_cond_jobs_not_empty.wait(job_manager->m_mtx_jobs);
				terminated = should_terminate(job_manager);
			}

			if (terminated)
			{
				job_manager->m_mtx_jobs.unlock();
				return nullptr;
			}
		}
		job_manager->m_mtx_jobs.unlock();
		if (job)
		{
			job_manager->m_mtx_running_jobs.lock();
			job_manager->m_num_of_running_jobs++;
			job_manager->m_mtx_running_jobs.unlock();

			job->run(); //only run job if it's a fresh one
			job_manager->m_mtx_running_jobs.lock();
			job_manager->m_num_of_running_jobs--;

			if(job_manager->m_num_of_running_jobs == 0){
				job_manager->m_cond_running_jobs.signal();
			}
			job_manager->m_mtx_running_jobs.unlock();
		}
	}
}

void JobManager::terminate()
{
	m_mtx_terminated.lock();
	terminated = true;
	m_mtx_terminated.unlock();
	m_cond_jobs_not_empty.broadcast(); //unblock threads
}
