#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

namespace bud{
	class mutex;

	class cond_variable{
		pthread_cond_t m_cond = PTHREAD_COND_INITIALIZER;

	public:
		void wait(bud::mutex& mtx);

		void signal();

		void broadcast();
	};

	class mutex{
		pthread_mutex_t m_mtx = PTHREAD_MUTEX_INITIALIZER;

		friend void cond_variable::wait(mutex&);

	public:
		void lock();

		void unlock();
	};
}

#endif
