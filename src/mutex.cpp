#include "mutex.h"

mutex::mutex()
{
	pthread_mutex_init(mtx, NULL);
}

mutex::~mutex()
{
	pthread_mutex_destroy(mtx);
}

void mutex::lock()
{
	pthread_mutex_lock(mtx);
}

void mutex::unlock()
{
	pthread_mutex_unlock(mtx);
}
