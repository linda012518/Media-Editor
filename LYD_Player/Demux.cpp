#include "Demux.h"
#include "FFmpegUtilFunc.h"
#include "OpenglWidget.h"
#include <iostream>

namespace lyd
{
	Demux::Demux()
	{
		av_register_all();
		avformat_network_init();
		avcodec_register_all();

		_recordPlay.initDevice(_audioPlay.getDevice(), _videoPlay.getDevice());
	}

	Demux::~Demux()
	{
		close();
	}

	bool Demux::open(const char* url)
	{
		close();

		_pts		=	0;
		_isEnd		=	false;
		_isPause	=	false;

		int ret = avformat_open_input(&_context, url, 0, 0);
		if (ret != 0)
		{
			printError(ret);
			return false;
		}
		ret = avformat_find_stream_info(_context, 0);
		if (ret < 0)
		{
			printError(ret);
			return false;
		}

		_totalMs = _context->duration / (AV_TIME_BASE / 1000);

		av_dump_format(_context, 0, url, 0);

		std::cout << "------------------------------------------" << std::endl;

		bool videoOpen = false;
		bool audioOpen = false;

		_videoStream = av_find_best_stream(_context, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
		if (AVERROR_STREAM_NOT_FOUND != _videoStream)
		{
			AVStream* vs = _context->streams[_videoStream];
			videoOpen = _videoPlay.open(vs->codecpar, vs->codecpar->width, vs->codecpar->height);
			if(videoOpen) videoOpen = AV_CODEC_ID_H264 == vs->codecpar->codec_id;
			if (!videoOpen) _videoStream = -1;
			_recordPlay.setVideoSize(vs->codecpar->width, vs->codecpar->height);
		}

		_audioStream = av_find_best_stream(_context, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
		if (AVERROR_STREAM_NOT_FOUND != _audioStream)
		{
			AVStream* as = _context->streams[_audioStream];
			audioOpen = _audioPlay.open(as->codecpar);
			if (!audioOpen) _audioStream = -1;
			_recordPlay.setAudioPar(as->codecpar->sample_rate, as->codecpar->channels);
		}

		return videoOpen;
	}

	void Demux::close()
	{
		_thread.close();
		if (_context)
			avformat_close_input(&_context);

		if (_gl) _gl->close();
		_videoPlay.close();
		_audioPlay.close();
	}

	void Demux::clear()
	{
		_videoPlay.clear();
		_audioPlay.clear();

		if (!_context) return;
		std::lock_guard<std::mutex> lock(_mutex);
		avformat_flush(_context);
	}

	void Demux::start()
	{
		_thread.start(nullptr, [this](LydThread* thread) { run(); }, nullptr);
		_videoPlay.start();
		_audioPlay.start();
		if (_gl) _gl->start();

	}

	bool Demux::exportVideo(const char * url) {
		bool ret = _recordPlay.open(url);
		if (!ret) return false;
		seek(0);
		_videoPlay.exportVideo();
		_audioPlay.exportAudio();
		setPause(false);
		_recordPlay.start();
		return true;
	}

	AVPacket* Demux::read()
	{
		AVPacket* pkt = av_packet_alloc();

		if (!_context) return nullptr;

		std::lock_guard<std::mutex> lock(_mutex);

		int ret = av_read_frame(_context, pkt);

		if (ret == AVERROR_EOF)
		{
			_isEnd = true;
			_audioPlay.setOver();
			_videoPlay.setOver();
		}

		if (ret != 0)
		{
			av_packet_free(&pkt);
			return nullptr;
		}
		//printf("%d \n", pkt->pts);
		//pts转为毫秒
		pkt->pts = pkt->pts * (r2d(_context->streams[pkt->stream_index]->time_base) * 1000);
		pkt->dts = pkt->dts * (r2d(_context->streams[pkt->stream_index]->time_base) * 1000);

		return pkt;
	}

	void Demux::setPause(bool pause)
	{
		_isPause = pause;
		_videoPlay.setPause(_isPause);
		_audioPlay.setPause(_isPause);
	}

	AVPacket* Demux::readVideo()
	{
		if (!_context)
		{
			return nullptr;
		}

		AVPacket* pkt = NULL;
		//防止阻塞，认定20帧内要读到数据
		for (size_t i = 0; i < 20; i++)
		{
			pkt = read();
			if (pkt == NULL) break;
			if (pkt->stream_index == _videoStream)
			{
				break;
			}
			av_packet_free(&pkt);
		}

		return pkt;
	}

	bool Demux::seek(double pos)
	{
		bool state = _isPause;
		_isPause = true;
		_videoPlay.setPause(true);
		//setPause(true);

		if (!_context) return false;

		{
			std::lock_guard<std::mutex> lock(_mutex);

			//duration可能没有，用time_base，time_base也可能没有用AV_TIME_BASE
			int seekpos = _context->streams[_videoStream]->duration * pos;
			//long long seekpos = ms * (r2d(context->streams[pkt->stream_index]->time_base) * 1000);

			//用视频，因为有I帧的关系
			//AVSEEK_FLAG_BACKWARD		往后找一帧
			//AVSEEK_FLAG_FRAME			I帧
			int ret = av_seek_frame(_context, _videoStream, seekpos, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);
			if (ret < 0) return false;
		}
		
		clear();

		long long seekPts = _totalMs * pos;
		while (_thread.isRun())
		{
			AVPacket* pkt = readVideo();
			if (!pkt) break;
			if (_videoPlay.seekPts(pkt, seekPts))
			{
				_pts = seekPts;
				break;
			}
		}
		//setPause(state);
		_isPause = state;
		_videoPlay.setPause(state);

		if (_isEnd)
		{
			_isEnd = false;
			_audioPlay.setOver2Play();
			_videoPlay.setOver2Play();
		}
		return true;
	}

	void Demux::run()
	{
		while (_thread.isRun())
		{
			if (_isPause)
			{
				LydThread::sleep(5);
				continue;
			}

			_pts = _audioPlay.getPts();
			_videoPlay.setPts(_pts);

			if (_isEnd)
			{
				LydThread::sleep(5);
				continue;
			}

			AVPacket* pkt = read();
			if (!pkt)
			{
				LydThread::sleep(2);
				continue;
			}

			if (pkt->stream_index == _audioStream)
			{
				while (_thread.isRun())
				{
					bool ret = _audioPlay.push(pkt);
					if (ret) break;
				}
			}
			else if (pkt->stream_index == _videoStream)
			{
				while (_thread.isRun())
				{
					bool ret = _videoPlay.push(pkt);
					if (ret) break;
				}
			}
			else
			{
				av_packet_free(&pkt);
			}
		}
	}

	void Demux::setVolume(float rate)
	{
		_audioPlay.setVolume(rate);
	}

	void Demux::setGL(OpenglWidget* gl)
	{
		_gl = gl;
		if (_gl)
		{
			_gl->setVideoPlay(&_videoPlay);
		}
	}

}