#include "YUV420Shader.h"

namespace lyd
{
	YUV420Shader::YUV420Shader()
	{
		compileShader();
	}
	
	void YUV420Shader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;
			layout(location = 1) in vec2 uvY;
			layout(location = 2) in vec2 uvU;
			layout(location = 3) in vec2 uvV;

			uniform mat4 projection;
			uniform mat4 model;

			out vec2 outY;
			out vec2 outU;
			out vec2 outV;

			void main()
			{
				outY = uvY;
				outU = uvU;
				outV = uvV;

				gl_Position = projection * model * vec4(inPos, 1.0);
			}
		);
		fs = STR(
			#version 330 core\n

			in vec2 outY;
			in vec2 outU;
			in vec2 outV;
			uniform sampler2D texYUV;

			void main()
			{
				vec3 yuv;
				vec3 rgb;

				yuv.x	=	texture2D(texYUV, outY).r;
				yuv.y	=	texture2D(texYUV, outU).r - 0.5;
				yuv.z	=	texture2D(texYUV, outV).r - 0.5;

				rgb		=	mat3(	1,		1,			1,
									0,		-0.39465,	2.03211,
									1.13983, -0.58060,	0
								) * yuv;

				gl_FragColor = vec4(rgb, 1.0);
			}
		);

	}
}