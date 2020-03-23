#ifndef _Shader_
#define _Shader_

#include <string>
#include "glew\glew.h"
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#define STR(s)     #s

namespace lyd
{
	enum    ShaderType
	{
		SHADER_NONE		=	0,
		SHADER_VERTEX	=	GL_VERTEX_SHADER,			//顶点
		SHADER_TES		=	GL_TESS_EVALUATION_SHADER,	//曲面细分1
		SHADER_TCS		=	GL_TESS_CONTROL_SHADER,		//曲面细分2
		SHADER_GEOMETRY =	GL_GEOMETRY_SHADER,			//几何
		SHADER_FRAGMENT =	GL_FRAGMENT_SHADER,			//片元
		SHADER_COMPUTE	=	GL_COMPUTE_SHADER,			//计算
	};

	class Shader
	{
	public:
		typedef GLint uniform;
	public:
		GLint	_program = -1;

	protected:
		const char* vs;
		const char* tes;
		const char* tcs;
		const char* gs;
		const char* fs;
		const char* cs;

	protected:
		virtual void initialize() = 0;

		bool compileShader();

	public:
		Shader();
		virtual ~Shader();

		virtual void begin();

		virtual void end();

		virtual void attachShader(GLuint shader);

		virtual void dettachShader(GLuint shader);

		virtual bool linkProgram();

		unsigned createShader(const char* source, ShaderType type);

		inline void setBool(const std::string &name, bool value) const
		{
			glUniform1i(glGetUniformLocation(_program, name.c_str()), (int)value);
		}
		inline void setInt(const std::string &name, int value) const
		{
			glUniform1i(glGetUniformLocation(_program, name.c_str()), value);
		}
		inline void setFloat(const std::string &name, float value) const
		{
			glUniform1f(glGetUniformLocation(_program, name.c_str()), value);
		}
		inline void setVec2(const std::string &name, const glm::vec2 &value) const
		{
			glUniform2fv(glGetUniformLocation(_program, name.c_str()), 1, &value[0]);
		}
		inline void setVec2(const std::string &name, float x, float y) const
		{
			glUniform2f(glGetUniformLocation(_program, name.c_str()), x, y);
		}
		inline void setVec3(const std::string &name, const glm::vec3 &value) const
		{
			glUniform3fv(glGetUniformLocation(_program, name.c_str()), 1, &value[0]);
		}
		inline void setVec3(const std::string &name, float x, float y, float z) const
		{
			glUniform3f(glGetUniformLocation(_program, name.c_str()), x, y, z);
		}
		inline void setVec4(const std::string &name, const glm::vec4 &value) const
		{
			glUniform4fv(glGetUniformLocation(_program, name.c_str()), 1, &value[0]);
		}
		inline void setVec4(const std::string &name, float x, float y, float z, float w)
		{
			glUniform4f(glGetUniformLocation(_program, name.c_str()), x, y, z, w);
		}
		inline void setMat2(const std::string &name, const glm::mat2 &mat) const
		{
			glUniformMatrix2fv(glGetUniformLocation(_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}
		inline void setMat3(const std::string &name, const glm::mat3 &mat) const
		{
			glUniformMatrix3fv(glGetUniformLocation(_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}
		inline void setMat4(const std::string &name, const glm::mat4 &mat) const
		{
			glUniformMatrix4fv(glGetUniformLocation(_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}

	};
}

#endif // !_Shader_



