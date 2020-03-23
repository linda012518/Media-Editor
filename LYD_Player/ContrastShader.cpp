#include "ContrastShader.h"


namespace lyd
{
	ContrastShader::ContrastShader()
	{
		compileShader();
		begin();
		setFloat("contrast", 1);
		end();
	}

	void ContrastShader::initialize()
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
			uniform float contrast;

			vec3 lerp(vec3 left, vec3 right, float rate)
			{
				return left * (1 - rate) + right * rate;
			}

			void main()
			{
				vec3	avgColor	=	vec3(0.5, 0.5, 0.5);
				vec3	finalColor	=	lerp(avgColor, texture2D(tex, uv).rgb, contrast);
				gl_FragColor		=	vec4(finalColor, 1.0);
			}
		);

	}
}