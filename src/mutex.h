#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>
namespace bud
{
	class mutex
	{
		pthread_mutex_t m_mtx = PTHREAD_MUTEX_INITIALIZER;

	public:
		void lock();
		void unlock();
	};
}
#endif
