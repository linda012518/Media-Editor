#include "OpenglWidget.h"
#include "glew\glew.h"
#include "qevent.h"
#include "VideoPlay.h"

OpenglWidget::OpenglWidget(QWidget* parent) : QWidget(parent, Qt::MSWindowsOwnDC)//这个qwidget组件有自己的dc，因为Opengl绘制要有自己的dc
{
	setAttribute(Qt::WA_PaintOnScreen, true);//激活qt引擎的函数
	setAttribute(Qt::WA_NoSystemBackground);//窗口都有个背景，不需要了
	setAutoFillBackground(true);

	bool ret = _contextRender.setup((HWND)winId(), GetDC((HWND)winId()));
	if (!ret)
	{
		printf("_contextRender setup false! \n");
	}
	ret = _contextResources.setup((HWND)winId(), GetDC((HWND)winId()));
	if (!ret)
	{
		printf("_contextRender setup false! \n");
	}

	_contextRender.shareContext(_contextResources.getRc());

	_contextResources.makeCurrent();

	GLenum what = glewInit();
	printf("glewInit=%d \n\n\n", what);
}

OpenglWidget::~OpenglWidget() 
{ 
}

void OpenglWidget::setVideoPlay(lyd::VideoPlay* play)
{
	_videoPlay = play;
}

bool OpenglWidget::start()
{
	_thread.start(nullptr, [this](lyd::LydThread* thread) { render(); }, nullptr);
	return true;
}

void OpenglWidget::close()
{
	_thread.close();
}

void OpenglWidget::render()
{
	_contextRender.makeCurrent();
	glEnable(GL_DEPTH_TEST);

	if (!_videoPlay) return;

	_videoPlay->setupMesh();
	_time.update();

	while (_thread.isRun())
	{
		if (_time.getElapsedTimeInMilliSec() < 40)
		{
			lyd::LydThread::sleep(1);
			continue;
		}
		_time.update();

		if (_isChange)
		{
			glViewport(0, 0, width(), height());
			_videoPlay->setRect(width(), height());
			std::lock_guard<std::mutex> lock(_mutex);
			_isChange = false;
		}
		glClearColor(0.6, 0.6, 0.6, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		_videoPlay->render();
		_contextRender.swapBuffer();
	}
}

void OpenglWidget::resizeEvent(QResizeEvent *event)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_isChange = true;
}

QPaintEngine* OpenglWidget::paintEngine() const 
{
	return 0;//屏蔽qt自带的绘制即可
}
