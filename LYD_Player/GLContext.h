#ifndef _GL_Context_
#define _GL_Context_

#include <Windows.h>

namespace lyd 
{
	class GLContext
	{
	protected:
		int		_format;
		HWND	_hwnd;
		HDC		_hdc;
		HGLRC	_hrc;
	public:
		GLContext();
		virtual ~GLContext();

		HGLRC getRc() const;

		bool setup(HWND hwnd, HDC hdc);

		void shutdown();

		void swapBuffer();

		bool makeCurrent();

		bool shareContext(HGLRC rc);
	};
}

#endif // !_GL_Context_


