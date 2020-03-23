#ifndef _FFmpeg_Util_Func_
#define _FFmpeg_Util_Func_

extern "C"
{
#include "libavformat\avformat.h"
}

namespace lyd
{
	double r2d(AVRational r);

	void printError(int ret);
}

#endif // !_FFmpeg_Util_Func_
