#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

namespace bud{

	class thread{
	public:
		thread(void*(*start_routine)(void*), void*arg);

		~thread();

		thread(thread&& other) noexcept;

		thread& operator=(thread&& other) noexcept;

		thread& operator=(const thread& other) = delete;

		thread(const thread& other) = delete;

		void join();

		pthread_t nativeHandle;

		void terminate();

	private:
		int m_id;

		bool m_joinable = true;

		static inline int m_idCounter = 0;
	};

}
#endif