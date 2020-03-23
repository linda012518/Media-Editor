#ifndef _Lyd_Thread_
#define _Lyd_Thread_

#include<thread>
#include "Semaphore.h"

namespace lyd
{
	class LydThread
	{
	private:
		typedef std::function<void(LydThread*)> EventCall;

		EventCall	_onCreate;
		EventCall	_onRun;
		EventCall	_onDestory;
		std::mutex	_mutex;
		//�ź���		�����̵߳���ֹ���˳�
		Semaphore	_sem;
		//�߳��Ƿ�����������
		bool		_isRun = false;

	public:
		static void sleep(time_t dt);

		void start(EventCall onCreate = nullptr, EventCall onRun = nullptr, EventCall onDestory = nullptr);
		//�����ر��߳�
		void close();
		//�����˳�	�ڹ����������˳�
		void exit();

		bool isRun();

	protected:
		void onWork();
	};
}

#endif // !_Lyd_Thread_


