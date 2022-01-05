#include "mutex.h"

void bud::cond_variable::signal(){
	pthread_cond_signal(&m_cond);
}

void bud::cond_variable::broadcast(){
	pthread_cond_broadcast(&m_cond);
}

void bud::cond_variable::wait(bud::mutex &mtx)
{
	pthread_cond_wait(&m_cond, &mtx.m_mtx);
}
