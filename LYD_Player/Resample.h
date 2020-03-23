#ifndef _Resample_
#define _Resample_

struct SwrContext;
struct AVFrame;
struct AVCodecParameters;

namespace lyd
{
	class Resample
	{
	public:
		Resample();
		virtual ~Resample();

		bool open(AVCodecParameters* para);
		void close();
		//返回重采样后大小		释放Indata空间
		int resample(AVFrame* indata, unsigned char* pcm);

		void setPara(int sampleRate);

	private:
		SwrContext*	_context	= nullptr;
		int			_sampleRate = 44100;
		int			_format		= 1; //AVSampleFormat::AV_SAMPLE_FMT_S16;
	};
}

#endif // !_Resample_



