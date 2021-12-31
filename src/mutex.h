#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>
#include "cond_variable.h"
namespace bud
{
	class mutex
	{
		pthread_mutex_t m_mtx = PTHREAD_MUTEX_INITIALIZER;
		friend void cond_variable::wait(mutex &);

	public:
		void lock();
		void unlock();
	};
}
#endif
