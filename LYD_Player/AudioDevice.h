#ifndef _Audio_Device_
#define _Audio_Device_

#include <mutex>
#include <list>
#include "soundtouch\SoundTouch.h"
#include "soundtouch\ByteConvertContext.hpp"
#include "Common.h"

#define BUFF_SIZE           2048
#define PCM_SIZE			4096

struct AVCodecParameters;
class QAudioOutput;
class QAudioInput;
class QIODevice;

namespace lyd
{
	class AudioDevice
	{
	public:
		AudioDevice();
		virtual ~AudioDevice();

		bool open();

		void close();

		void clear();

		void setSampleRate(int sampleRate);

		bool write(char* pcm, int size);

		int getFree();

		long long getNotPlayMs();

		void setPause(bool pause);

		RecordFrame* popPcm();

		inline void setVolume(float rate) {
			_volume = rate;
		}

		inline void setTempo(float tempo) {
			_soundTouch.setTempoChange(tempo);
		}
		inline void setPitch(float pitch) {
			_soundTouch.setPitchSemiTones(pitch);
		}
		inline void setRate(float rate) {
			_soundTouch.setRateChange(rate);
		}

		inline void exportAudio() {
			_isExport = true;
		}
		void exportOver();
		inline bool getExportFinished() {
			return !_isExport;
		}

		void clearExportBuffer();

	private:
		void changeVolume(const char* pcm, int size);

		void push(char* buf, int size);

	private:
		soundtouch::SoundTouch	_soundTouch;
		soundtouch::SAMPLETYPE	_sampleBuffer[BUFF_SIZE];
		ByteConvertContext		_byteConvert;
		std::mutex				_mutex;
		std::mutex				_mutexPcm;
		std::list<RecordFrame*>	_pcmList;
		char					_pcmBuffer[PCM_SIZE*4];
		int						_lastPos	= 0;
		QAudioOutput*			_output		= nullptr;
		QIODevice*				_outio		= nullptr;
		int						_sampleRate = 44100;
		int						_sampleSize = 16;//16Î»2×Ö½Ú
		int						_channels	= 2;
		float					_volume		= 1;
		bool					_isExport	= false;
		//float					_speed		= 1;
	};
}

#endif // !_Audio_Device_



