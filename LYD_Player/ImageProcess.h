#ifndef _Image_Process_
#define _Image_Process_

#include <vector>
#include <mutex>
#include <list>
#include "Shader.h"
#include "Common.h"

struct AVFrame;

namespace lyd
{
	class ImageProcess
	{
	public:
		ImageProcess();
		virtual ~ImageProcess();

		bool open(int w, int h);
		void close();

		void render();

		void addShader(Shader* shader);

		void removeShader(Shader* shader);

		void updateTexture(AVFrame* frame);

		void setupMesh();

		void setRect(int w, int h);

		RecordFrame* popRgb();
		inline void exportVideo() {
			_isExport = true;
			_exportFinished = false;
		}
		inline void exportOver() {
			_isExport = false;
		}
		inline bool getExportFinished() {
			return _exportFinished;
		}

		void clearExportBuffer();

		void changeSpeed() {
			_changeSpeed = true;
		}

	private:
		void updateImage(GLubyte* dst, int x, int y, int w, int h, void* data);

		void genFrameBuffer();

		void genExportVAO();

		void renderEffect();

		void switchFBO();

		glm::mat4 getModelMat();

		void pushRgb();

		void pushExportFrame();
		uint8_t* popExportFrame();
		void renderExport();

		void deleteGPU_Buffer();

	private:
		std::vector<Shader*>	_shaderList;
		std::list<RecordFrame*>	_rgbList;
		std::mutex				_mutexRgb;
		std::list<uint8_t*>		_exportFramelist;
		std::mutex				_mutexExportFrame;
		Shader*		_decodeShader;
		Shader*		_exportShader;

		glm::mat4	_projection;

		int			_width;
		int			_height;
		float		_videoRatio;

		int			_winWidth;
		int			_winHeight;

		uint8_t*	_yuvData;

		GLuint		_yuvTex;

		GLuint		YUV_VAO = 0, YUV_VBO = 0;
		GLuint		VAO = 0, VBO = 0;
		GLuint		FBO1 = 0, ColorBuffer1 = 0, RenderBuffer1 = 0;
		GLuint		FBO2 = 0, ColorBuffer2 = 0, RenderBuffer2 = 0;
		GLuint		_currentFBO			= 0;
		GLuint		_currentColorBuffer = 0;

		bool		_update;
		bool		_isExport;
		bool		_exportFinished;
		bool		_changeSpeed;

	};
}

#endif // !_Image_Process_

