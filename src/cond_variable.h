#ifndef COND_VARIABLE_H
#define COND_VARIABLE_H

#include <pthread.h>

namespace bud
{
	class mutex;

	class cond_variable
	{
		pthread_cond_t m_cond = PTHREAD_COND_INITIALIZER;

	public:
		void wait(bud::mutex &mtx);

		void signal();

		void broadcast();
	};

}
#include "mutex.h"

#endif //COND_VARIABLE_H
