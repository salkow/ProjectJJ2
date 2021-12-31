#include "job_manager.h"

JobManager::JobManager()
{
	// m_jobs.reserve(NUM_OF_THREADS);
	m_threads.reserve(NUM_OF_THREADS);

	for (auto &thread : m_threads)
	{
		pthread_create(&thread.nativeHandle, NULL, &JobManager::run_forever, this);
	}
}

void JobManager::addJob(Job &&j)
{
	m_jobs.emplace_back(std::move(j));
}

void JobManager::runAllJobs()
{
	for (auto &job : m_jobs)
		job.run();
}
