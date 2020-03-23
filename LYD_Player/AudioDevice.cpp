#include "AudioDevice.h"
#include "qaudioformat.h"
#include "qaudiooutput.h"
#include "qaudioinput.h"

#include "LydThread.h"

extern "C"
{
#include "libavcodec\avcodec.h"
}

namespace lyd
{
	AudioDevice::AudioDevice()
	{
	}

	AudioDevice::~AudioDevice()
	{
		close();
	}

	bool AudioDevice::open()
	{
		close();
		std::lock_guard<std::mutex> lock(_mutex);
		QAudioFormat fmt;
		fmt.setSampleRate(_sampleRate/* * _speed*/);
		fmt.setSampleSize(_sampleSize);
		fmt.setChannelCount(_channels);
		fmt.setCodec("audio/pcm");
		fmt.setByteOrder(QAudioFormat::LittleEndian);//小端模式
		fmt.setSampleType(QAudioFormat::UnSignedInt);//short16位

		_byteConvert.open(_sampleSize);

		_soundTouch.setSampleRate(_sampleRate);
		_soundTouch.setChannels(_channels);

		_soundTouch.setTempoChange(0);
		_soundTouch.setPitchSemiTones(0);
		_soundTouch.setRateChange(0);

		_soundTouch.setSetting(SETTING_USE_QUICKSEEK, 0);
		_soundTouch.setSetting(SETTING_USE_AA_FILTER, !0);

		_output = new QAudioOutput(fmt);
		if (!_output)
		{
			return false;
		}

		_outio = _output->start();
		if (!_outio)
		{
			return false;
		}

		return true;
	}

	void AudioDevice::close()
	{
		clear();

		std::lock_guard<std::mutex> lock(_mutex);

		if (_outio)
		{
			_outio->close();
			_outio = nullptr;
		}
		if (_output)
		{
			_output->stop();
			delete _output;
			_output = 0;
		}
		_lastPos = 0;
		_byteConvert.close();
	}

	void AudioDevice::clear()
	{
		if (_outio)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_outio->reset();
		}
		clearExportBuffer();
	}

	void AudioDevice::clearExportBuffer()
	{
		std::lock_guard<std::mutex> lock(_mutexPcm);
		while (!_pcmList.empty())
		{
			RecordFrame* pcm = _pcmList.front();
			_pcmList.pop_front();
			delete pcm;
			pcm = nullptr;
		}
	}

	void AudioDevice::setSampleRate(int sampleRate)
	{
		_sampleRate = sampleRate;
	}

	void AudioDevice::changeVolume(const char* pcm, int size)
	{
		for (size_t i = 0; i < size; i += 2)
		{
			short* volume = (short*)(pcm + i);
			long filter = *volume * _volume;
			if (filter < -0x8000)
			{
				*volume = -0x8000;
			}
			else if (filter > 0x7FFF)
			{
				*volume = 0x7FFF;
			}
			else
			{
				*volume = filter;
			}
		}
	}

	bool AudioDevice::write(char* pcm, int size)
	{
		if (!pcm || size <= 0 || !_outio) return false;

		//qint64 wLen = 0;
		//int remain = size;
		changeVolume(pcm, size);

		int num = _byteConvert.read(_sampleBuffer, BUFF_SIZE, pcm, size);
		int nSamples = num / _channels;
		_soundTouch.putSamples(_sampleBuffer, nSamples);

		std::lock_guard<std::mutex> lock(_mutex);
		do
		{
			if (_output->bytesFree() < size)
			{
				LydThread::sleep(1);
				continue;
			}

			int buffSizeSamples = BUFF_SIZE / _channels;
			nSamples = _soundTouch.receiveSamples(_sampleBuffer, buffSizeSamples);
			int size = 0;
			void* pcm_convert = _byteConvert.write(_sampleBuffer, nSamples * _channels, &size);

			if (nSamples != 0 && pcm_convert)
			{
				_outio->write((char*)pcm_convert, size);
				push((char*)pcm_convert, size);
			}
			
		} while (nSamples != 0);

		//_outio->write((char*)pcm, size);
		//while (wLen < size)
		//{
		//	qint64 temp = _io->write((char*)pcm + wLen, remain);
		//	wLen += temp;
		//	remain = size - wLen;
		//}
		return true;
	}

	int AudioDevice::getFree()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (!_output) return -1;
		return _output->bytesFree();
	}

	long long AudioDevice::getNotPlayMs()
	{
		double size = 0;
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (!_output) return 0;
			size = _output->bufferSize() - _output->bytesFree();
		}

		long long pts = 0;

		//一秒音频字节大小
		double secSize = _sampleRate * (_sampleSize / 8) * _channels;

		if (secSize > 0)
		{
			//还要多少毫秒播放完这些数据
			pts = (size / secSize) * 1000;
		}
		return pts;
	}

	void AudioDevice::setPause(bool pause)
	{
		if (!_output) return;
		std::lock_guard<std::mutex> lock(_mutex);
		if (pause)
		{
			_output->suspend();
		}
		else
		{
			_output->resume();
		}
	}

	void AudioDevice::push(char* buf, int size)
	{
		if (!_isExport) return;
		std::lock_guard<std::mutex> lock(_mutexPcm);
		if (!_isExport) return;

		memcpy(_pcmBuffer + _lastPos, buf, size);
		_lastPos += size;
		if (_lastPos >= PCM_SIZE)
		{
			char* pcm = new char[PCM_SIZE];
			memcpy(pcm, _pcmBuffer, PCM_SIZE);

			RecordFrame* frame = new RecordFrame();
			frame->data = pcm;
			_pcmList.push_back(frame);

			_lastPos -= PCM_SIZE;
			memcpy(_pcmBuffer, _pcmBuffer + PCM_SIZE, _lastPos);
		}
	}

	RecordFrame* AudioDevice::popPcm()
	{
		if (!_isExport) return nullptr;
		if (_pcmList.empty()) return nullptr;

		std::lock_guard<std::mutex> lock(_mutexPcm);
		RecordFrame* pcm = _pcmList.front();
		_pcmList.pop_front();

		return pcm;
	}
}