#include "Semaphore.h"

namespace lyd
{
	void Semaphore::wait()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		if (--_wait < 0)
		{
			//×èÈûµÈ´ý
			_cv.wait(lock, [this]()->bool {
				return _wakeup > 0;
			});
			--_wakeup;
		}
	}

	void Semaphore::wakeup()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (++_wait <= 0)
		{
			++_wakeup;
			_cv.notify_one();
		}
	}
}