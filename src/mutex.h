#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

class mutex
{
	pthread_mutex_t *mtx;

public:
	mutex();
	~mutex();

	void lock();
	void unlock();
};

#endif
