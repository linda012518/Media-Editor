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

		//��ȡ��ǰ��
		double getElapsedSecond();
		//��ȡ����
		double getElapsedTimeInMilliSec();
		//��ȡ΢��
		long long getElapsedTimeInMicroSec();

	protected:
		std::chrono::time_point<std::chrono::high_resolution_clock> _begin;
	};
}

#endif // !_Timestamp_


