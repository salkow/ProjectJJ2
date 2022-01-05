#include "thread.h"
#include "iostream"
bud::thread::thread(void* (*start_routine) (void*), void* arg)
{
	m_id = ++m_idCounter;
//	pthread_create(&nativeHandle, NULL, start_routine, arg);
}

bud::thread::~thread(){
	if(m_joinable)
		join();
}

bud::thread::thread(thread&& other) noexcept
	: nativeHandle(other.nativeHandle), m_id(other.m_id), m_joinable(other.m_joinable)
{
	other.m_joinable = false;
}

bud::thread& bud::thread::operator=(thread&& other) noexcept
{
	nativeHandle = other.nativeHandle;

	m_id = other.m_id;

	m_joinable = other.m_joinable;

	other.m_joinable = false;

	return *this;
}

void bud::thread::join(){
	m_joinable = false;

	pthread_join(nativeHandle, NULL);
}

void bud::thread::terminate(){
//	pthread_cancel(nativeHandle); //TODO make sure this is ok
}