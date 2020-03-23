#ifndef _Video_Play_
#define _Video_Play_

#include "ImageProcess.h"
#include "Decode.h"
#include "LydThread.h"

#include <mutex>

struct AVCodecParameters;
struct AVPacket;

namespace lyd
{
	class VideoPlay
	{
	public:
		VideoPlay();
		virtual ~VideoPlay();

		bool open(AVCodecParameters* para, int w, int h);

		void close();

		void clear();

		void setPause(bool pause);

		void start();

		bool push(AVPacket* pkt);

		bool seekPts(AVPacket* pkt, long long seekpts);

		void render();
		void setRect(int w, int h);
		void setupMesh();
		void setPts(long long pts) { _pts = pts; }

		inline void addShader(Shader* shader) {
			_image.addShader(shader);
		}

		void exportVideo();

		inline void setOver() {
			_isEnd = true;
		}
		inline void setOver2Play() {
			_isEnd = false;
			_isFlush = false;
		}

		ImageProcess* getDevice() {
			return &_image;
		}

		void changeSpeed() {
			_image.changeSpeed();
		}

	private:
		void run();

	private:
		std::mutex		_mutex;
		Decode			_decode;
		ImageProcess	_image;
		LydThread		_thread;
		long long		_pts;
		bool			_isPause;
		bool			_isEnd;
		bool			_isFlush;
	};
}

#endif // !_Video_Play_



