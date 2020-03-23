#include "LydThread.h"

namespace lyd
{
	void LydThread::sleep(time_t dt)
	{
		std::chrono::milliseconds t(dt);
		std::this_thread::sleep_for(t);
	}

	void LydThread::start(EventCall onCreate, EventCall onRun, EventCall onDestory)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (!_isRun)
		{
			_isRun = true;
			if (onCreate)
				_onCreate = onCreate;
			if (onRun)
				_onRun = onRun;
			if (onDestory)
				_onDestory = onDestory;
			std::thread t(std::mem_fn(&LydThread::onWork), this);
			t.detach();
		}
	}
	void LydThread::close()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (_isRun)
		{
			_isRun = false;
			_sem.wait();
		}
	}
	void LydThread::exit()
	{
		if (_isRun)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_isRun = false;
		}
	}
	bool LydThread::isRun()
	{
		return _isRun;
	}
	void LydThread::onWork()
	{
		if (_onCreate)
			_onCreate(this);
		if (_onRun)
			_onRun(this);
		if (_onDestory)
			_onDestory(this);

		_sem.wakeup();
		_isRun = false;
	}
}