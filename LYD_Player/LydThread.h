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
		//信号量		控制线程的终止、退出
		Semaphore	_sem;
		//线程是否启动运行中
		bool		_isRun = false;

	public:
		static void sleep(time_t dt);

		void start(EventCall onCreate = nullptr, EventCall onRun = nullptr, EventCall onDestory = nullptr);
		//正常关闭线程
		void close();
		//报错退出	在工作函数中退出
		void exit();

		bool isRun();

	protected:
		void onWork();
	};
}

#endif // !_Lyd_Thread_


