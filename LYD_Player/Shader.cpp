#include "Shader.h"
#include <iostream>

namespace lyd
{
	Shader::Shader()
	{
		vs	= nullptr;
		tes	= nullptr;
		tcs = nullptr;
		gs	= nullptr;
		fs	= nullptr;
		cs	= nullptr;
	}

	Shader::~Shader()
	{
	}

	bool Shader::compileShader()
	{
		initialize();

		_program = glCreateProgram();

		if (vs != nullptr && fs != nullptr)
		{
			GLuint vsHandle = createShader(vs, SHADER_VERTEX);
			if (vsHandle == -1)
			{
				return false;
			}
			attachShader(vsHandle);

			GLuint fsHandle = createShader(fs, SHADER_FRAGMENT);
			if (fsHandle == -1)
			{
				return false;
			}
			attachShader(fsHandle);
		}

		if (tes != nullptr)
		{
			GLuint tesHandle = createShader(tes, SHADER_TES);
			if (tesHandle == -1)
			{
				return false;
			}
			attachShader(tesHandle);
		}

		if (tcs != nullptr)
		{
			GLuint tcsHandle = createShader(tcs, SHADER_TCS);
			if (tcsHandle == -1)
			{
				return false;
			}
			attachShader(tcsHandle);
		}

		if (gs != nullptr)
		{
			GLuint gsHandle = createShader(gs, SHADER_GEOMETRY);
			if (gsHandle == -1)
			{
				return false;
			}
			attachShader(gsHandle);
		}

		if (cs != nullptr)
		{
			GLuint csHandle = createShader(cs, SHADER_COMPUTE);
			if (csHandle == -1)
			{
				return false;
			}
			attachShader(csHandle);
		}

		linkProgram();

		return true;
	}

	void Shader::begin()
	{
		glUseProgram(_program);
	}

	void Shader::end()
	{
		glUseProgram(0);
	}

	void Shader::attachShader(GLuint shader)
	{
		glAttachShader(_program, shader);
	}

	void Shader::dettachShader(GLuint shader)
	{
		glDetachShader(_program, shader);
	}

	bool Shader::linkProgram()
	{
		GLint   status = 0;
		
		/// Á´½Ó³ÌÐò
		glLinkProgram(_program);
		glGetProgramiv(_program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			char    compileLog[1024] = { 0 };
			glGetProgramInfoLog(_program, sizeof(compileLog), 0, compileLog);
			std::cout << compileLog << std::endl;
			return  false;
		}
		return  true;
	}

	unsigned Shader::createShader(const char* source, ShaderType type)
	{
		unsigned    handle = 0;
		switch (type)
		{
		case SHADER_VERTEX:
			handle = glCreateShader(GL_VERTEX_SHADER);
			break;
		case SHADER_GEOMETRY:
			handle = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		case SHADER_FRAGMENT:
			handle = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case SHADER_TES:
			handle = glCreateShader(GL_TESS_EVALUATION_SHADER);
			break;
		case SHADER_TCS:
			handle = glCreateShader(GL_TESS_CONTROL_SHADER);
			break;
		case SHADER_COMPUTE:
			handle = glCreateShader(GL_COMPUTE_SHADER);
			break;
		}

		GLint   status = 0;

		glShaderSource(handle, 1, (const GLchar**)&source, 0);
		glCompileShader(handle);
		glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
		
		if (status == GL_FALSE)
		{
			char    compileLog[1024] = { 0 };
			glGetShaderInfoLog(handle, sizeof(compileLog), 0, compileLog);
			//assert(false && compileLog);
			std::cout << compileLog << std::endl;
			glDeleteShader(handle);
			return  -1;
		}
		return  handle;
	}
}