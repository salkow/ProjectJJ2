#include "mutex.h"

bud::mutex::mutex()
{
	pthread_mutex_init(&m_mtx, NULL);
}

bud::mutex::~mutex()
{
	pthread_mutex_destroy(&m_mtx);
}

void bud::mutex::lock()
{
	pthread_mutex_lock(&m_mtx);
}

void bud::mutex::unlock()
{
	pthread_mutex_unlock(&m_mtx);
}
