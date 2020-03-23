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
		//�����ȴ�-��������
		std::condition_variable _cv;
		//�ȴ�����
		int						_wait	= 0;
		//���Ѽ���
		int						_wakeup = 0;
	};
}

#endif // !_Semaphore_



