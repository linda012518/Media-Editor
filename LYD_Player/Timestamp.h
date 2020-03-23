#ifndef _Timestamp_
#define _Timestamp_

#include <chrono>

namespace lyd
{
	class Timestamp
	{
	public:
		Timestamp();
		virtual ~Timestamp();

		void update();

		//获取当前秒
		double getElapsedSecond();
		//获取毫秒
		double getElapsedTimeInMilliSec();
		//获取微妙
		long long getElapsedTimeInMicroSec();

	protected:
		std::chrono::time_point<std::chrono::high_resolution_clock> _begin;
	};
}

#endif // !_Timestamp_


