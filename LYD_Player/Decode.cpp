#include "Decode.h"
#include "FFmpegUtilFunc.h"
#include "LydThread.h"

extern "C"
{
#include "libavcodec\avcodec.h"
}

namespace lyd
{
	Decode::Decode()
	{
	}

	Decode::~Decode()
	{
		close();
	}

	bool Decode::open(AVCodecParameters* para)
	{
		if (!para) return false;
		close();

		_pts = 0;

		AVCodec* codec = avcodec_find_decoder(para->codec_id);
		if (!codec) return false;

		std::lock_guard<std::mutex> lock(_contextMutex);

		_context = avcodec_alloc_context3(codec);
		if (!_context) return false;

		int ret = avcodec_parameters_to_context(_context, para);
		if (ret < 0)
		{
			avcodec_free_context(&_context);
			printError(ret);
			return false;
		}

		_context->thread_count = 4;

		ret = avcodec_open2(_context, 0, 0);
		if (ret != 0)
		{
			avcodec_free_context(&_context);
			printError(ret);
			return false;
		}

		return true;
	}

	void Decode::close()
	{
		clear();

		std::lock_guard<std::mutex> lock(_contextMutex);
		if (_context)
		{
			avcodec_close(_context);
			avcodec_free_context(&_context);
		}

		_pts = 0;
	}

	void Decode::clear()
	{
		std::lock_guard<std::mutex> lock1(_contextMutex);
		if (_context)
		{
			avcodec_flush_buffers(_context);
		}
		std::lock_guard<std::mutex> lock2(_listMutex);
		while (!_packets.empty())
		{
			AVPacket* pkt = _packets.front();
			av_packet_free(&pkt);
			_packets.pop_front();
		}
		_packets.clear();
	}

	bool Decode::send(AVPacket* pkt)
	{
		if (!pkt || pkt->size <= 0 || !pkt->data) return false;

		if (!_context) return false;

		std::lock_guard<std::mutex> lock(_contextMutex);
		int ret = avcodec_send_packet(_context, pkt);

		av_packet_free(&pkt);

		return ret == 0;
	}

	bool Decode::sendNull()
	{
		std::lock_guard<std::mutex> lock(_contextMutex);
		return 0 == avcodec_send_packet(_context, NULL);
	}

	AVFrame* Decode::receive()
	{
		if (!_context) return nullptr;

		AVFrame* frame = av_frame_alloc();

		std::lock_guard<std::mutex> lock(_contextMutex);
		int ret = avcodec_receive_frame(_context, frame);
		if (ret != 0)
		{
			av_frame_free(&frame);
			return nullptr;
		}
		_pts = frame->pts;
		return frame;
	}

	void Decode::freeFrame(AVFrame** frame)
	{
		av_frame_free(frame);
	}

	bool Decode::push(AVPacket* pkt)
	{
		if (!pkt || pkt->size <= 0 || !pkt->data) return true;

		if (_packets.size() < _maxList)
		{
			std::lock_guard<std::mutex> lock(_listMutex);
			_packets.push_back(pkt);
			return true;
		}
		else
		{
			LydThread::sleep(1);
			return false;
		}
	}
	AVPacket* Decode::pop()
	{
		if (_packets.empty())
		{
			return nullptr;
		}

		std::lock_guard<std::mutex> lock(_listMutex);
		AVPacket* pkt = _packets.front();
		_packets.pop_front();

		return pkt;
	}
}