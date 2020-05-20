#include "RecordPlay.h"
#include "ImageProcess.h"
#include "AudioDevice.h"
#include <iostream>

extern "C"
{
#include "libavformat\avformat.h"
#include "libswscale\swscale.h"
#include "libavutil\opt.h"
#include "libswresample\swresample.h"
}

using namespace std;

namespace lyd
{
	RecordPlay::RecordPlay()
	{
	}

	RecordPlay::~RecordPlay()
	{
		_thread.close();
		close();
	}

	bool RecordPlay::open(const char* url)
	{
		close();
		if (!url || strlen(url) <= 0)
		{
			return false;
		}
		vpts = 0;
		apts = 0;
		filename = url;
		avformat_alloc_output_context2(&oc, NULL, NULL, url);
		if (!oc)
		{
			cerr << "avformat_alloc_output_context2 failed" << endl;
			return false;
		}
		return true;
	}
	void RecordPlay::close()
	{
		if (oc)
		{
			//vs 在 ic 里边，跟随删除了
			avformat_close_input(&oc);
		}
		if (vc)
		{
			avcodec_close(vc);
			avcodec_free_context(&vc);
		}
		if (ac)
		{
			avcodec_close(ac);
			avcodec_free_context(&ac);
		}
		if (vsc)
		{
			sws_freeContext(vsc);
			vsc = nullptr;
		}
		if (asc)
		{
			swr_free(&asc);
		}
		if (yuv)
		{
			av_frame_free(&yuv);
		}
		if (pcm)
		{
			av_frame_free(&pcm);
		}
	}

	bool RecordPlay::addVideoStream()
	{
		if (!oc) return false;

		//1 创建视频编码器
		AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
		if (!codec)
		{
			cerr << "avcodec_find_encoder AV_CODEC_ID_H264 failed" << endl;
			return false;
		}
		vc = avcodec_alloc_context3(codec);
		if (!vc)
		{
			cerr << "avcodec_alloc_context3 failed" << endl;
			return false;
		}
		vc->bit_rate = vBitrate;//压缩后每秒大小 比特率
		vc->width = outWidth;
		vc->height = outHeight;

		vc->time_base = { 1, outfps };
		vc->framerate = { outfps, 1 };

		vc->gop_size = 50;
		vc->max_b_frames = 0;//不要b帧

		vc->pix_fmt = AV_PIX_FMT_YUV420P;
		vc->codec_id = AV_CODEC_ID_H264;
		//压缩方式 为快速
		av_opt_set(vc->priv_data, "preset", "superfast", 0);
		vc->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;//统一头 可以不用sps pps

		int ret = avcodec_open2(vc, codec, NULL);
		if (ret != 0)
		{
			cerr << "avcodec_open2 failed" << endl;
			return false;
		}
		cout << "avcodec_open2 success" << endl;

		//添加视频流到输出上下文
		vs = avformat_new_stream(oc, NULL);
		vs->codecpar->codec_tag = 0;//编码是独立的，不存编码信息
		//把编码器的信息考到封装流里  宽高啥啥的
		avcodec_parameters_from_context(vs->codecpar, vc);

		av_dump_format(oc, 0, filename.c_str(), 1);

		//像素转换上下文 RGB TO YUV
		vsc = sws_getCachedContext(vsc,
			inWidth, inHeight, (AVPixelFormat)inPixformat,
			outWidth, outHeight, AV_PIX_FMT_YUV420P,
			SWS_BICUBIC, NULL, NULL, NULL);
		if (!vsc)
		{
			cerr << "sws_getCachedContext failed" << endl;
			return false;
		}
		if (!yuv)
		{
			yuv = av_frame_alloc();
			yuv->format = AV_PIX_FMT_YUV420P;
			yuv->width = outWidth;
			yuv->height = outHeight;
			yuv->pts = 0;
			ret = av_frame_get_buffer(yuv, 24);//24位对齐，3字节
			if (ret != 0)
			{
				cerr << "av_frame_get_buffer failed" << endl;
				return false;
			}
		}

		return true;
	}
	bool RecordPlay::addAudioStream()
	{
		if (!oc) return false;
		//找到音频编码
		AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
		if (!codec)
		{
			cerr << "avcodec_find_encoder AV_CODEC_ID_AAC failed" << endl;
			return false;
		}
		//创建并打开音频编码器
		ac = avcodec_alloc_context3(codec);
		if (!ac)
		{
			cerr << "avcodec_alloc_context3 failed" << endl;
			return false;
		}
		//输出参数
		ac->bit_rate = aBitrate;
		ac->sample_rate = outSampleRate;
		ac->sample_fmt = (AVSampleFormat)outSampleType;
		ac->channels = outChannels;
		ac->channel_layout = av_get_default_channel_layout(outChannels);
		ac->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;//统一头 可以不用sps pps

		int ret = avcodec_open2(ac, codec, NULL);
		if (ret != 0)
		{
			avcodec_free_context(&ac);
			cerr << "avcodec_open2 failed" << endl;
			return false;
		}

		//添加音频流到输出上下文
		as = avformat_new_stream(oc, NULL);
		if (!as)
		{
			cerr << "avformat_new_stream failed" << endl;
			return false;
		}
		as->codecpar->codec_tag = 0;//编码是独立的，不存编码信息
									//把编码器的信息考到封装流里  采样率啥啥的
		avcodec_parameters_from_context(as->codecpar, ac);

		av_dump_format(oc, 0, filename.c_str(), 1);

		//音频重采样上下文创建
		//swr_alloc();
		asc = swr_alloc_set_opts(asc,
			ac->channel_layout, ac->sample_fmt, ac->sample_rate,
			av_get_default_channel_layout(inChannel), (AVSampleFormat)inSampleType, inSampleRate,
			0, 0);
		ret = swr_init(asc);
		if (ret != 0)
		{
			avcodec_free_context(&ac);
			cerr << "swr_init failed" << endl;
			return false;
		}
		//音频重采样后输出avframe
		if (!pcm)
		{
			pcm = av_frame_alloc();
			pcm->format = ac->sample_fmt;
			pcm->channels = ac->channels;
			pcm->channel_layout = ac->channel_layout;
			pcm->nb_samples = outNbsample;//单通道 一帧音频样本数量
			ret = av_frame_get_buffer(pcm, 0);//0不用字节对齐
			if (ret != 0)
			{
				cerr << "av_frame_get_buffer failed" << endl;
				return false;
			}
		}

		return true;
	}

	AVPacket* RecordPlay::encodeVideo(const char* rgb)
	{
		if (!oc || !vsc || !yuv || !rgb) return nullptr;
		AVPacket* p = nullptr;
		uint8_t* indata[AV_NUM_DATA_POINTERS] = { 0 };
		indata[0] = (uint8_t*)rgb;

		int insize[AV_NUM_DATA_POINTERS] = { 0 };
		insize[0] = inWidth * 3;
		//rgb to yuv
		int h = sws_scale(vsc, indata, insize, 0, inHeight,
			yuv->data, yuv->linesize);
		if (h < 0)
		{
			return nullptr;
		}
		//cout << h << "||";
		yuv->pts = vpts;
		vpts++;
		//encode
		//前面几个失败，因为会有缓冲		编解码都一样		传null会把缓冲区的取出来，会造成结尾几帧没有
		int ret = avcodec_send_frame(vc, yuv);
		if (ret != 0)
		{
			return nullptr;
		}
		p = av_packet_alloc();

		ret = avcodec_receive_packet(vc, p);
		if (ret != 0 || p->size <= 0)
		{
			av_packet_free(&p);
			return nullptr;
		}

		//自己设置的时间基数和生成avframe的时间基数不一样，要转换
		av_packet_rescale_ts(p, vc->time_base, vs->time_base);
		p->stream_index = vs->index;
		return p;
	}
	AVPacket* RecordPlay::encodeAudio(const char* d)
	{
		if (!oc || !asc || !pcm || !d) return nullptr;
		//音频重采样
		const uint8_t* data[AV_NUM_DATA_POINTERS] = { 0 };
		data[0] = (uint8_t*)d;
		int ret = swr_convert(asc,
			pcm->data, pcm->nb_samples,
			data, pcm->nb_samples);
		//cout << ret << "*";

		//音频编码
		ret = avcodec_send_frame(ac, pcm);
		if (ret != 0)
		{
			return nullptr;
		}

		AVPacket* pkt = av_packet_alloc();
		av_init_packet(pkt);
		ret = avcodec_receive_packet(ac, pkt);
		if (ret != 0)
		{
			av_packet_free(&pkt);//释放自己和里面的数据
			return nullptr;
		}
		//cout << pkt->size << "*";
		pkt->stream_index = as->index;

		pkt->pts = apts;
		pkt->dts = pkt->pts;
		apts += av_rescale_q(pcm->nb_samples, { 1, ac->sample_rate }, ac->time_base);

		return pkt;
	}

	void RecordPlay::encodeVideoFlush()
	{
		int ret = avcodec_send_frame(vc, NULL);
		if (ret != 0)
		{
			return;
		}

		while (true)
		{
			AVPacket* p = av_packet_alloc();
			ret = avcodec_receive_packet(vc, p);
			if (ret != 0 || p->size <= 0)
			{
				av_packet_free(&p);
				return;
			}

			//自己设置的时间基数和生成avframe的时间基数不一样，要转换
			av_packet_rescale_ts(p, vc->time_base, vs->time_base);
			p->stream_index = vs->index;
			writeAVPacket(p);
		}
	}
	void RecordPlay::encodeAudioFlush()
	{
		int ret = avcodec_send_frame(ac, NULL);
		if (ret != 0)
		{
			return;
		}

		while (true)
		{
			AVPacket* pkt = av_packet_alloc();
			av_init_packet(pkt);
			ret = avcodec_receive_packet(ac, pkt);
			if (ret != 0)
			{
				av_packet_free(&pkt);//释放自己和里面的数据
				return;
			}
			//cout << pkt->size << "*";
			pkt->stream_index = as->index;

			pkt->pts = apts;
			pkt->dts = pkt->pts;
			apts += av_rescale_q(pcm->nb_samples, { 1, ac->sample_rate }, ac->time_base);

			writeAVPacket(pkt);
		}
	}
	
	bool RecordPlay::writeVideo()
	{
		if (!_videoDevice) return false;

		RecordFrame* rgb = _videoDevice->popRgb();
		if (!rgb) return false;

		AVPacket* pkt = encodeVideo(rgb->data);
		delete rgb;
		writeAVPacket(pkt);
		return true;
	}
	bool RecordPlay::writeAudio()
	{
		if (!_audioDevice) return false;

		RecordFrame* data = _audioDevice->popPcm();
		if (!data) return false;

		AVPacket* pkt = encodeAudio(data->data);
		delete data;
		writeAVPacket(pkt);
		return true;
	}

	bool RecordPlay::writeHeader()
	{
		if (!oc) return false;
		//打开io
		int ret = avio_open(&oc->pb, filename.c_str(), AVIO_FLAG_WRITE);
		if (ret != 0)
		{
			cerr << "avio_open failed" << endl;
			return false;
		}

		//写入封装头
		ret = avformat_write_header(oc, NULL);
		if (ret != 0)
		{
			cerr << "avformat_write_header failed" << endl;
			return false;
		}
		return true;
	}
	bool RecordPlay::writeAVPacket(AVPacket* pkt)
	{
		if (!oc || !pkt || pkt->size <= 0) return false;
		//av_write_frame
		int ret = av_interleaved_write_frame(oc, pkt);//会把帧做dts排序
		if (ret != 0)
		{
			return false;
		}

		return true;
	}
	bool RecordPlay::writeEnd()
	{
		if (!oc || !oc->pb) return false;

		//写入尾部视频帧信息索引，关键帧地填和ptd
		int ret = av_write_trailer(oc);
		if (ret != 0)
		{
			cerr << "av_write_trailer failed" << endl;
			return false;
		}

		//关闭io
		if (avio_closep(&oc->pb) != 0)
		{
			cerr << "avio_close failed" << endl;
			return false;
		}

		return true;
	}

	void RecordPlay::start()
	{
		bool ret = addVideoStream();
		if (!ret)
		{
			close();
			return;
		}
		ret = addAudioStream();
		if (!ret)
		{
			close();
			return;
		}
		ret = writeHeader();
		if (!ret)
		{
			close();
			return;
		}

		_isOver = false;

		_thread.start(nullptr, 
			[this](LydThread* thread) { run(); }, 
			[this](LydThread* thread) { writeEnd(); close(); }
		);
	}

	void RecordPlay::stop()
	{
		_isOver = true;
		//_thread.close();
	}

	bool RecordPlay::isVideoBefore()
	{
		if (!oc || !as || !vs) return false;

		int ret = av_compare_ts(vpts, vc->time_base, apts, ac->time_base);
		if (ret <= 0)
		{
			return true;
		}

		return false;
	}

	void RecordPlay::run()
	{
		bool vOver = false, aOver = false;

		while (_thread.isRun())
		{
			if (_videoDevice)
			{
				RecordFrame* rgb = _videoDevice->popRgb();
				if (rgb)
				{
					AVPacket* pkt = encodeVideo(rgb->data);
					delete rgb;
					writeAVPacket(pkt);
					//printf("-");
				}
				else
				{
					if (_isOver) vOver = true;
				}
			}

			//audio
			if (_audioDevice)
			{
				RecordFrame* data = _audioDevice->popPcm();
				if (data)
				{
					AVPacket* pkt = encodeAudio(data->data);
					delete data;
					writeAVPacket(pkt);
					//printf("|");
				}
				else
				{
					if (_isOver) aOver = true;
				}
			}

			if (_videoDevice->getExportFinished() && _audioDevice->getExportFinished()) stop();
			if (_isOver && vOver && aOver)
			{
				while (true)
				{
					if (!writeAudio()) break;
				}
				encodeAudioFlush();

				while (true)
				{
					if (!writeVideo()) break;
				}
				encodeVideoFlush();

				_videoDevice->clearExportBuffer();
				_audioDevice->clearExportBuffer();
				break;
			}
		}
	}

	void RecordPlay::initDevice(AudioDevice* ad, ImageProcess* vd)
	{
		_audioDevice = ad;
		_videoDevice = vd;
	}
}