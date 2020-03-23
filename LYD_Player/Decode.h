#ifndef _Audio_Decode_
#define _Audio_Decode_

#include <list>
#include <mutex>

struct AVFrame;
struct AVPacket;
struct AVCodecContext;
struct AVCodecParameters;

namespace lyd
{
	class Decode
	{
	public:
		Decode();
		virtual ~Decode();

		bool open(AVCodecParameters* para);
		void close();
		void clear();

		bool send(AVPacket* pkt);
		bool sendNull();
		AVFrame* receive();

		bool push(AVPacket* pkt);
		AVPacket* pop();

		void freeFrame(AVFrame** frame);

		long long getPts() const { return _pts; }

	private:
		std::mutex				_listMutex;
		std::mutex				_contextMutex;
		std::list<AVPacket*>	_packets;
		long long				_pts;
		AVCodecContext*			_context = nullptr;
		int						_maxList = 100;
	};
}

#endif // !_Audio_Decode_



