#include "AudioPlay.h"

extern "C"
{
#include "libavcodec\avcodec.h"
}

namespace lyd
{
	AudioPlay::AudioPlay()
	{

	}

	AudioPlay::~AudioPlay()
	{
		close();
	}

	bool AudioPlay::open(AVCodecParameters* para)
	{
		close();

		_isPause	=	false;
		_isEnd		=	false;
		_isFlush	=	false;
		_pts		=	0;

		bool ret = _decode.open(para);
		if (!ret) return false;

		ret = _resample.open(para);
		if (!ret) return false;

		ret = _device.open();
		if (!ret) return false;

		return true;
	}

	void AudioPlay::close()
	{
		_thread.close();

		_decode.close();
		_resample.close();
		_device.close();
		_pts = 0;
	}

	void AudioPlay::clear()
	{
		_decode.clear();
		_device.clear();
	}

	void AudioPlay::setPause(bool pause)
	{
		_isPause = pause;
		_device.setPause(_isPause);
	}

	bool AudioPlay::push(AVPacket* pkt)
	{
		return _decode.push(pkt);
	}

	void AudioPlay::start()
	{
		_thread.start(nullptr, [this](LydThread* thread) { run(); }, nullptr);
	}

	void AudioPlay::setVolume(float rate)
	{
		_device.setVolume(rate);
	}

	void AudioPlay::run()
	{
		unsigned char* pcm = new unsigned char[1024 * 1024 * 10];

		AVPacket* pkt = nullptr;
		AVFrame* frame = nullptr;
		bool ret = false;

		while (_thread.isRun())
		{
			if (_isPause || _isFlush)
			{
				LydThread::sleep(5);
				continue;
			}

			pkt = _decode.pop();
			if (_isEnd && !pkt)
			{
				ret = _decode.sendNull();
				_isFlush = true;
			}
			else
			{
				ret = _decode.send(pkt);
			}
			
			if (!ret)
			{
				LydThread::sleep(1);
				continue;
			}
			while (_thread.isRun())
			{
				frame = _decode.receive();
				if (!frame) break;

				_pts = _decode.getPts() - _device.getNotPlayMs();

				int len = _resample.resample(frame, pcm);
				if (len <= 0) break;

				while (_thread.isRun())
				{
					if (_device.getFree() < len)
					{
						LydThread::sleep(1);
						continue;
					}
					_device.write((char*)pcm, len);
					break;
				}
			}
			if (_isFlush) _device.exportOver();
		}
		delete pcm;
	}

}