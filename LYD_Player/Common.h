#ifndef _lyd_Common_
#define _lyd_Common_

namespace lyd
{
	struct RecordFrame {
		~RecordFrame()
		{
			if (data)
			{
				delete data;
				data = nullptr;
			}
			pts = 0;
		}
		char* data;
		long long pts;
	};
}

#endif // !_lyd_Common_
