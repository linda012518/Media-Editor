#include "ExportShader.h"

namespace lyd
{
	ExportShader::ExportShader()
	{
		compileShader();
	}

	void ExportShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;
			layout(location = 1) in vec2 inUV;

			uniform mat4 projection;
			uniform mat4 model;

			out vec2 uv;

			void main()
			{
				uv = inUV;

				gl_Position = projection * model * vec4(inPos, 1.0);
			}
		);
		fs = STR(
			#version 330 core\n

			in vec2 uv;
			uniform sampler2D tex;

			void main()
			{
				gl_FragColor = vec4(texture2D(tex, uv).rgb, 1.0);
			}
		);

	}
}