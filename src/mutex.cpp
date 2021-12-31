#include "mutex.h"

void bud::mutex::lock()
{
	pthread_mutex_lock(&m_mtx);
}

void bud::mutex::unlock()
{
	pthread_mutex_unlock(&m_mtx);
}
