#ifndef _Contrast_Shader_
#define _Contrast_Shader_

#include "Shader.h"

namespace lyd
{
	class ContrastShader : public Shader
	{
	public:
		ContrastShader();
	protected:
		virtual void initialize();
	};
}

#endif // !_Contrast_Shader_



