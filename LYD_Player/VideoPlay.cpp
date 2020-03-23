#include "VideoPlay.h"

namespace lyd
{
	VideoPlay::VideoPlay()
	{
		
	}

	VideoPlay::~VideoPlay()
	{
		close();
	}

	bool VideoPlay::open(AVCodecParameters* para, int w, int h)
	{
		close();

		_pts		=	0;
		_isEnd		=	false;
		_isPause	=	false;
		_isFlush	=	false;

		bool ret = _decode.open(para);
		if (!ret) return false;

		ret = _image.open(w, h);
		if (!ret) return false;

		return true;
	}

	void VideoPlay::close()
	{
		_thread.close();

		_decode.close();
		_image.close();
		_pts = 0;
	}

	void VideoPlay::clear()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_decode.clear();
	}

	void VideoPlay::setPause(bool pause)
	{
		_isPause = pause;
	}

	void VideoPlay::start()
	{
		_thread.start(nullptr, [this](LydThread* thread) { run(); }, nullptr);
	}

	bool VideoPlay::push(AVPacket* pkt)
	{
		return _decode.push(pkt);
	}

	bool VideoPlay::seekPts(AVPacket* pkt, long long seekpts)
	{
		bool ret = _decode.send(pkt);
		if (!ret) return false;

		AVFrame* frame = _decode.receive();
		if (!frame) return false;

		//std::lock_guard<std::mutex> lock(_mutex);
		//_image.updateTexture(frame);
		//return true;

		if (_decode.getPts() >= seekpts)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_image.updateTexture(frame);
			return true;
		}
		_decode.freeFrame(&frame);
		return false;
	}

	void VideoPlay::run()
	{
		AVPacket* pkt = nullptr;
		AVFrame* frame = nullptr;
		bool ret = false;

		while (_thread.isRun())
		{
			if (_isPause)
			{
				LydThread::sleep(5);
				continue;
			}

			if (_pts < _decode.getPts())
			{
				LydThread::sleep(1);
				continue;
			}

			if (_isFlush)
			{
				LydThread::sleep(5);
				continue;
			}

			std::lock_guard<std::mutex> lock(_mutex);

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
				_image.updateTexture(frame);
			}
			if (_isFlush) _image.exportOver();
		}

	}

	void VideoPlay::render()
	{
		_image.render();
	}
	void VideoPlay::setRect(int w, int h)
	{
		_image.setRect(w, h);
	}
	void VideoPlay::setupMesh()
	{
		_image.setupMesh();
	}
	void VideoPlay::exportVideo()
	{
		_image.exportVideo();
	}
}