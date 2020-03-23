#include "GLContext.h"
#include "glew\glew.h"

#pragma comment(lib, "opengl32.lib")

namespace lyd
{
	GLContext::GLContext()
	{
		_format =	0;
		_hwnd	=	0;
		_hdc	=	0;
		_hrc	=	0;
	}

	GLContext::~GLContext()
	{
		shutdown();
	}

	void GLContext::swapBuffer()
	{
		SwapBuffers(_hdc);
	}

	bool GLContext::setup(HWND hwnd, HDC hdc)
	{
		_hwnd = hwnd;
		_hdc = hdc;

		unsigned pixelFormat;
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			32,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			24,
			8,
			0,
			PFD_MAIN_PLANE,
			0,
			0,
			0,
			0
		};
		if (_format == 0)
		{
			pixelFormat = ChoosePixelFormat(_hdc, &pfd);
		}
		else
		{
			pixelFormat = _format;
		}
		if (!SetPixelFormat(_hdc, pixelFormat, &pfd))
		{
			return  false;
		}
		_hrc = wglCreateContext(_hdc);
		//if (!wglMakeCurrent(_hdc, _hrc))
		//{
		//	return  false;
		//}

		//glewInit();
		return  true;
	}

	void GLContext::shutdown()
	{
		if (_hrc != NULL)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(_hrc);
			_hrc = NULL;
		}
		if (_hdc != NULL)
		{
			ReleaseDC(_hwnd, _hdc);
			_hdc = NULL;
		}
	}

	HGLRC GLContext::getRc() const
	{
		return _hrc;
	}

	bool GLContext::makeCurrent()
	{
		if (!wglMakeCurrent(_hdc, _hrc))
		{
			return  false;
		}

		return true;
	}

	bool GLContext::shareContext(HGLRC rc)
	{
		return wglShareLists(rc, _hrc);

		//DWORD a = GetLastError();//ERROR_BUSY 170
	}
}