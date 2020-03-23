#ifndef _Audio_Play_
#define _Audio_Play_

#include "Resample.h"
#include "AudioDevice.h"
#include "Decode.h"
#include "LydThread.h"
#include <mutex>

struct AVCodecParameters;
struct AVPacket;

namespace lyd
{
	class AudioPlay
	{
	public:
		AudioPlay();
		virtual ~AudioPlay();

		bool open(AVCodecParameters* para);

		void close();

		void clear();

		void setPause(bool pause);

		void start();

		bool push(AVPacket* pkt);

		long long getPts() const { return _pts; }

		void setVolume(float rate);

		inline void setTempo(float tempo) {
			_device.setTempo(tempo);
		}
		inline void setPitch(float pitch) {
			_device.setPitch(pitch);
		}
		inline void setRate(float rate) {
			_device.setRate(rate);
		}

		inline void setOver() {
			_isEnd = true;
		}
		inline void setOver2Play() {
			_isEnd = false;
			_isFlush = false;
		}
		inline void exportAudio() {
			_device.exportAudio();
		}

		AudioDevice* getDevice() {
			return &_device;
		}

	private:
		void run();

	private:
		AudioDevice	_device;
		Decode		_decode;
		Resample	_resample;
		LydThread	_thread;
		long long	_pts;
		bool		_isPause;
		bool		_isEnd;
		bool		_isFlush;
	};
}

#endif // !_Audio_Play_



