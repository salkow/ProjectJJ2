#include "mutex.h"

bud::mutex::mutex()
{
	pthread_mutex_init(mtx, NULL);
}

bud::mutex::~mutex()
{
	pthread_mutex_destroy(mtx);
}

void bud::mutex::lock()
{
	pthread_mutex_lock(mtx);
}

void bud::mutex::unlock()
{
	pthread_mutex_unlock(mtx);
}
