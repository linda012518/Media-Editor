#ifndef _YUV_420_Shader_
#define _YUV_420_Shader_

#include "Shader.h"

namespace lyd
{
	class YUV420Shader : public Shader
	{
	public:
		YUV420Shader();
	protected:
		virtual void initialize();
	};
}

#endif // !_YUV_420_Shader_


