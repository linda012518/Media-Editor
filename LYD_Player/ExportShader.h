#ifndef _Export_Shader_
#define _Export_Shader_

#include "Shader.h"

namespace lyd
{
	class ExportShader : public Shader
	{
	public:
		ExportShader();
	protected:
		virtual void initialize();
	};
}

#endif // !_Export_Shader_


