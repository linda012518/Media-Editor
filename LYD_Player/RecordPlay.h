#ifndef _Record_Play_
#define _Record_Play_

#include "LydThread.h"

struct AVPacket;
struct AVFormatContext;
struct AVCodecContext;
struct AVStream;
struct SwsContext;
struct SwrContext;
struct AVFrame;

namespace lyd
{
	class AudioDevice;
	class ImageProcess;
}

namespace lyd
{
	class RecordPlay
	{
	public:
		RecordPlay();
		virtual ~RecordPlay();

		bool open(const char* url);

		void start();
		void stop();

		void initDevice(AudioDevice* ad, ImageProcess* vd);

		void setVideoSize(int w, int h) {
			inWidth = w;
			inHeight = h;
		}

		void setAudioPar(int sampleRate, int channels) {
			inSampleRate = sampleRate;
			inChannel = channels;
		}

	private:
		void close();

		bool addVideoStream();
		bool addAudioStream();

		AVPacket* encodeVideo(const char* rgb);
		AVPacket* encodeAudio(const char* d);
		void encodeVideoFlush();
		void encodeAudioFlush();

		bool writeVideo();
		bool writeAudio();

		bool writeHeader();
		bool writeAVPacket(AVPacket* pkt);
		bool writeEnd();

		bool isVideoBefore();

		void run();

	private:
		LydThread		_thread;
		AudioDevice*	_audioDevice	=	nullptr;
		ImageProcess*	_videoDevice	=	nullptr;
		bool			_isOver			=	false;


		AVFormatContext*	oc		= nullptr;//封装mp4输出上下文
		AVCodecContext*		vc		= nullptr;//视频编码器上下文
		AVCodecContext*		ac		= nullptr;//音频编码器上下文
		AVStream*			vs		= nullptr;//视频流
		AVStream*			as		= nullptr;//音频流
		SwsContext*			vsc		= nullptr;//像素格式转换上下文
		SwrContext*			asc		= nullptr;//音频格式转换上下文
		AVFrame*			yuv		= nullptr;//yuv输出
		AVFrame*			pcm		= nullptr;//pcm输出
		int					vpts	= 0;//视频pts
		int					apts	= 0;//音频pts

		//输出
		int vBitrate		= 4000000;
		int outWidth		= 960;
		int outHeight		= 540;
		int outfps			= 25;


		//输入
		int inWidth			= 1920;
		int inHeight		= 1080;
		int inPixformat		= 2;	//AVPixelFormat		AV_PIX_FMT_RGB24

		//输出
		int aBitrate		= 64000;
		int outChannels		= 2;
		int outSampleRate	= 44100;
		int outSampleType	= 8;//AVSampleFormat	AV_SAMPLE_FMT_FLTP
		int outNbsample		= 1024;
		//输入
		int inSampleRate	= 44100;
		int inChannel		= 2;
		int inSampleType	= 1;//AVSampleFormat	AV_SAMPLE_FMT_S16

		std::string filename;
	};
}

#endif // !_Record_Play_



