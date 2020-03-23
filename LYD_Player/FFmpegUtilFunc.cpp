#include "FFmpegUtilFunc.h"

#include <iostream>

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "swresample.lib")
#pragma comment(lib, "swscale.lib")

namespace lyd
{
	double r2d(AVRational r)
	{
		return r.den == 0 ? 0 : (double)r.num / (double)r.den;
	}

	void printError(int ret)
	{
		char buf[1024] = { 0 };
		av_strerror(ret, buf, 1023);
		std::cout << buf << std::endl;
	}

}