#include "Resample.h"
#include "FFmpegUtilFunc.h"

extern "C"
{
#include "libswresample\swresample.h"
#include "libavcodec\avcodec.h"
}

namespace lyd
{
	Resample::Resample()
	{
	}

	Resample::~Resample()
	{
		close();
	}

	void Resample::setPara(int sampleRate)
	{
		_sampleRate = sampleRate;
	}

	bool Resample::open(AVCodecParameters* para)
	{
		close();

		if (!para) return false;

		setPara(para->sample_rate);

		_context = swr_alloc_set_opts(_context,
			av_get_default_channel_layout(2), (AVSampleFormat)_format, _sampleRate,
			av_get_default_channel_layout(para->channels), (AVSampleFormat)para->format, para->sample_rate,
			0, 0);

		if (!_context) return false;

		int ret = swr_init(_context);
		if (ret != 0)
		{
			printError(ret);
			return false;
		}

		return true;
	}

	void Resample::close()
	{
		if (_context)
		{
			swr_free(&_context);
		}
	}

	int Resample::resample(AVFrame* indata, unsigned char* pcm)
	{
		if (!indata) return 0;
		if (!pcm)
		{
			av_frame_free(&indata);
			return 0;
		}
		if (!_context)
		{
			return 0;
		}

		uint8_t* data[2] = { 0 };
		data[0] = pcm;

		int ret = swr_convert(_context,
			data, indata->nb_samples,
			(const uint8_t**)indata->data, indata->nb_samples);

		int out = ret * indata->channels * av_get_bytes_per_sample((AVSampleFormat)_format);
		av_frame_free(&indata);
		return out;
	}

}