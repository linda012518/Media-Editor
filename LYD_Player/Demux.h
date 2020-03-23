#ifndef _Demux_
#define _Demux_

#include "LydThread.h"
#include "AudioPlay.h"
#include "VideoPlay.h"
#include "RecordPlay.h"

#include <mutex>

struct AVFormatContext;
struct AVPacket;
class OpenglWidget;

namespace lyd
{
	enum VideoState {
		VideoNormal	=	1 << 0,
		VideoEnd	=	1 << 1,
		VideoFlush	=	1 << 2,
		VideoPause	=	1 << 3,
	};

	class Demux
	{
	public:
		Demux();
		virtual ~Demux();

		bool open(const char* url);
		void close();

		void clear();

		void start();

		void setPause(bool pause);
		bool getPause() const { return _isPause; }

		bool seek(double pos);

		void setGL(OpenglWidget* gl);

		void setVolume(float rate);

		inline void setTempo(float tempo) {
			_audioPlay.setTempo(tempo);
			_videoPlay.changeSpeed();
		}
		inline void setPitch(float pitch) {
			_audioPlay.setPitch(pitch);
		}
		inline void setRate(float rate) {
			_audioPlay.setRate(rate);
			_videoPlay.changeSpeed();
		}


		inline long long getTotalMs() const { return _totalMs; }
		inline long long getPts() const { return _pts; }

		inline void addShader(Shader* shader) {
			_videoPlay.addShader(shader);
		}
		bool exportVideo(const char* url);

	private:
		AVPacket* read();
		AVPacket* readVideo();
		void run();

	private:
		std::mutex			_mutex;
		LydThread			_thread;
		AudioPlay			_audioPlay;
		VideoPlay			_videoPlay;
		RecordPlay			_recordPlay;
		AVFormatContext*	_context		=	nullptr;
		OpenglWidget*		_gl				=	nullptr;
		long long			_totalMs		=	0;
		long long			_pts			=	0;
		int					_videoStream	=	-1;
		int					_audioStream	=	-1;
		bool				_isPause		=	false;
		bool				_isEnd			=	false;
		unsigned int		_state			=	VideoNormal;
	};
}

#endif // !_Demux_



