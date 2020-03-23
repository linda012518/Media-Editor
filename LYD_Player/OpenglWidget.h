#ifndef _Opengl_Widget_
#define _Opengl_Widget_

#include "qwidget.h"
#include "LydThread.h"
#include "GLContext.h"
#include "Timestamp.h"

#include <mutex>

struct AVCodecParameters;

namespace lyd
{
	class VideoPlay;
}


class OpenglWidget : public QWidget
{
	Q_OBJECT

public:
	OpenglWidget(QWidget* parent = 0);
	~OpenglWidget();

	void setVideoPlay(lyd::VideoPlay* play);

	bool start();
	void close();

protected:
	virtual void resizeEvent(QResizeEvent *event);

private:
	QPaintEngine *paintEngine() const Q_DECL_OVERRIDE;//屏蔽qt自带的绘制即可

	void render();

private:
	lyd::LydThread		_thread;
	lyd::GLContext		_contextRender;
	lyd::GLContext		_contextResources;
	lyd::Timestamp		_time;

	std::mutex			_mutex;
	lyd::VideoPlay*		_videoPlay	=	nullptr;
	bool				_isChange	=	false;
};

#endif // !_Opengl_Widget_



