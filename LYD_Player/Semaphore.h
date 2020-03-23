#ifndef _Semaphore_
#define _Semaphore_

#include <mutex>

namespace lyd
{
	class Semaphore
	{
	public:
		void wait();

		void wakeup();

	private:
		std::mutex				_mutex;
		//阻塞等待-条件变量
		std::condition_variable _cv;
		//等待计数
		int						_wait	= 0;
		//唤醒计数
		int						_wakeup = 0;
	};
}

#endif // !_Semaphore_



