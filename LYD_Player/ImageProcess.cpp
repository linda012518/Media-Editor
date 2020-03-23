#include "ImageProcess.h"
#include "YUV420Shader.h"
#include "ExportShader.h"

#include "ContrastShader.h"

extern "C"
{
#include "libavutil/frame.h"
}

namespace lyd
{
	ImageProcess::ImageProcess()
	{
		_yuvTex			=	0;
		_width			=	0;
		_height			=	0;
		_winWidth		=	0;
		_winHeight		=	0;

		_yuvData		=	nullptr;
		_decodeShader	=	nullptr;

		_isExport		=	false;
		_update			=	false;

		_exportShader	=	new ExportShader();
		_exportShader->begin();
		_exportShader->setInt("tex", 0);
		_exportShader->end();
	}

	ImageProcess::~ImageProcess()
	{
		if (_exportShader)
		{
			delete _exportShader;
			_exportShader = nullptr;
		}
		close();
	}

	bool ImageProcess::open(int w, int h)
	{
		_width		=	w;
		_height		=	h;
		_videoRatio = (float)_width / (float)_height;

		_winWidth	=	w;
		_winHeight	=	h;

		_yuvData	=	new uint8_t[_width * (_height + _height / 2)];

		_projection =	glm::ortho(0.0f, (float)_width, 0.0f, (float)_height);

		_decodeShader = new YUV420Shader();
		_decodeShader->begin();
		_decodeShader->setMat4("projection", _projection);
		_decodeShader->setMat4("model", glm::mat4(1));
		_decodeShader->setInt("texYUV", 0);
		_decodeShader->end();

		glGenTextures(1, &_yuvTex);

		glBindTexture(GL_TEXTURE_2D, _yuvTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _width, _height + _height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		_update = false;
		_changeSpeed = false;

		return true;
	}

	void ImageProcess::close()
	{
		if (_yuvData)
		{
			delete _yuvData;
			_yuvData = nullptr;
		}
		if (_yuvTex)
		{
			glDeleteTextures(1, &_yuvTex);
			_yuvTex = 0;
		}
		for (auto itr = _shaderList.begin(); itr != _shaderList.end(); itr++)
		{
			Shader* shader = *itr;
			delete shader;
			shader = nullptr;
		}
		_shaderList.clear();
		if (_decodeShader)
		{
			delete _decodeShader;
			_decodeShader = nullptr;
		}

		clearExportBuffer();
	}

	void ImageProcess::deleteGPU_Buffer()
	{
		if (YUV_VBO)
		{
			glDeleteBuffers(1, &YUV_VBO);
			YUV_VBO = 0;
		}
		if (YUV_VAO)
		{
			glDeleteVertexArrays(1, &YUV_VAO);
			YUV_VAO = 0;
		}
		if (VBO)
		{
			glDeleteBuffers(1, &VBO);
			VBO = 0;
		}
		if (VAO)
		{
			glDeleteVertexArrays(1, &VAO);
			VAO = 0;
		}
		if (FBO1)
		{
			glDeleteFramebuffers(1, &FBO1);
			FBO1 = 0;
		}
		if (ColorBuffer1)
		{
			glDeleteTextures(1, &ColorBuffer1);
			ColorBuffer1 = 0;
		}
		if (RenderBuffer1)
		{
			glDeleteRenderbuffers(1, &RenderBuffer1);
			RenderBuffer1 = 0;
		}
		if (FBO2)
		{
			glDeleteFramebuffers(1, &FBO2);
			FBO2 = 0;
		}
		if (ColorBuffer2)
		{
			glDeleteTextures(1, &ColorBuffer2);
			ColorBuffer2 = 0;
		}
		if (RenderBuffer2)
		{
			glDeleteRenderbuffers(1, &RenderBuffer2);
			RenderBuffer2 = 0;
		}
	}

	void ImageProcess::updateImage(GLubyte* dst, int x, int y, int w, int h, void* data)
	{
		int         pitch = _width;
		GLubyte*    dst1 = dst + y * pitch + x;
		GLubyte*    src = (GLubyte*)(data);

		int         size = w;

		for (int i = 0; i < h; ++i)
		{
			memcpy(dst1, src, w);
			dst1 += pitch;
			src += w;
		}
	}

	void ImageProcess::updateTexture(AVFrame* frame)
	{
		if (!frame)
		{
			return;
		}

		if (_width == frame->linesize[0])
		{
			memcpy(_yuvData, frame->data[0], _width * _height);
			updateImage(_yuvData, 0, _height, _width / 2, _height / 2, frame->data[1]);
			updateImage(_yuvData, _width / 2, _height, _width / 2, _height / 2, frame->data[2]);
		}
		else
		{
			for (size_t i = 0; i < _height; i++)
			{
				memcpy(_yuvData + _width * i, frame->data[0] + frame->linesize[0] * i, _width);
			}
			for (size_t i = 0; i < _height / 2; i++)
			{
				memcpy(_yuvData + _width * _height + _width * i, frame->data[1] + frame->linesize[1] * i, _width / 2);

				memcpy(_yuvData + _width * _height + _width * i + _width / 2, frame->data[2] + frame->linesize[2] * i, _width / 2);
			}
		}

		av_frame_free(&frame);

		if (!_update) _update = true;

		pushExportFrame();
	}

	void ImageProcess::renderEffect()
	{
		glBindVertexArray(VAO);
		glViewport(0, 0, _width, _height);

		for (auto itr = _shaderList.begin(); itr != _shaderList.end(); itr++)
		{
			switchFBO();

			glBindFramebuffer(GL_FRAMEBUFFER, _currentFBO);

			glClearColor(0.6, 0.6, 0.6, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空深度和颜色缓冲
			glBindTexture(GL_TEXTURE_2D, _currentColorBuffer);

			Shader* shader = *itr;
			shader->begin();
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			shader->end();
		}

		if (_changeSpeed) pushRgb();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		switchFBO();

		glBindVertexArray(0);
	}

	void ImageProcess::switchFBO()
	{
		if (_currentFBO == FBO1)
		{
			_currentFBO = FBO2;
			_currentColorBuffer = ColorBuffer1;
		}
		else
		{
			_currentFBO = FBO1;
			_currentColorBuffer = ColorBuffer2;
		}
		//_currentFBO = _currentFBO == FBO1 ? FBO2 : FBO1;
	}

	void ImageProcess::renderExport()
	{
		if (_changeSpeed) return;
		if (!_isExport && _exportFramelist.empty()) return;

		while (true)
		{
			uint8_t* frame = popExportFrame();
			if (!frame) break;

			glBindVertexArray(YUV_VAO);

			glBindTexture(GL_TEXTURE_2D, _yuvTex);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height + _height / 2, GL_RED, GL_UNSIGNED_BYTE, frame);
			delete frame;

			glViewport(0, 0, _width, _height);
			glBindFramebuffer(GL_FRAMEBUFFER, _currentFBO);

			glClearColor(0.6, 0.6, 0.6, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空深度和颜色缓冲

			if (_decodeShader)
			{
				_decodeShader->begin();
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				_decodeShader->end();
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);


			glBindVertexArray(VAO);
			glViewport(0, 0, _width, _height);

			for (auto itr = _shaderList.begin(); itr != _shaderList.end(); itr++)
			{
				switchFBO();

				glBindFramebuffer(GL_FRAMEBUFFER, _currentFBO);

				glClearColor(0.6, 0.6, 0.6, 1);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空深度和颜色缓冲
				glBindTexture(GL_TEXTURE_2D, _currentColorBuffer);

				Shader* shader = *itr;
				shader->begin();
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				shader->end();
			}
			pushRgb();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			switchFBO();

			glBindVertexArray(0);
		}
	}

	void ImageProcess::render()
	{
		if (!_update) return;
		glBindVertexArray(YUV_VAO);

		glBindTexture(GL_TEXTURE_2D, _yuvTex);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height + _height / 2, GL_RED, GL_UNSIGNED_BYTE, _yuvData);

		glViewport(0, 0, _width, _height);
		glBindFramebuffer(GL_FRAMEBUFFER, _currentFBO);

		glClearColor(0.6, 0.6, 0.6, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空深度和颜色缓冲

		if (_decodeShader)
		{
			_decodeShader->begin();
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			_decodeShader->end();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		//switchFBO();

		renderEffect();

		glViewport(0, 0, _winWidth, _winHeight);
		glClearColor(0.6, 0.6, 0.6, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空深度和颜色缓冲

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, _currentColorBuffer);
		_exportShader->begin();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		_exportShader->end();
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);

		renderExport();
	}

	void ImageProcess::addShader(Shader* shader)
	{
		if (!shader) return;

		for (auto itr = _shaderList.begin(); itr != _shaderList.end(); ++itr)
		{
			if (shader == *itr) return;
		}
		_shaderList.push_back(shader);

		shader->begin();
		shader->setMat4("projection", glm::ortho(0.0f, (float)_width, (float)_height, 0.0f));
		shader->setMat4("model", glm::mat4(1));
		shader->setInt("tex", 0);
		shader->end();
	}

	void ImageProcess::removeShader(Shader* shader)
	{
		if (!shader) return;

		for (auto itr = _shaderList.begin(); itr != _shaderList.end(); ++itr)
		{
			if (shader == *itr)
			{
				delete shader;
				shader = nullptr;
				_shaderList.erase(itr);
				break;
			}
		}
	}

	void ImageProcess::setupMesh()
	{
		deleteGPU_Buffer();

		struct  Vertex
		{
			glm::vec2  pos;
			glm::vec2  uvY;
			glm::vec2  uvU;
			glm::vec2  uvV;
		};

		float x = 0;
		float y = 0;

		float Yv = (float)_height / (float)(_height + _height / 2);

		float   Uu0 = 0;
		float   Uv0 = Yv;
		float   Uu1 = 0.5f;
		float   Uv1 = 1.0f;

		float   Vu0 = 0.5f;
		float   Vv0 = Yv;
		float   Vu1 = 1.0f;
		float   Vv1 = 1.0f;

		Vertex  vertex[] =
		{
			glm::vec2(x,			y),				glm::vec2(0,0),     glm::vec2(Uu0,Uv0),   glm::vec2(Vu0,Vv0),
			glm::vec2(x + _width,	y),				glm::vec2(1,0),     glm::vec2(Uu1,Uv0),   glm::vec2(Vu1,Vv0),
			glm::vec2(x,			y + _height),	glm::vec2(0,Yv),    glm::vec2(Uu0,Uv1),   glm::vec2(Vu0,Vv1),
			glm::vec2(x + _width,	y + _height),	glm::vec2(1,Yv),    glm::vec2(Uu1,Uv1),   glm::vec2(Vu1,Vv1),
		};

		glGenVertexArrays(1, &YUV_VAO);
		glGenBuffers(1, &YUV_VBO);

		glBindVertexArray(YUV_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, YUV_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(4 * sizeof(float)));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		genFrameBuffer();

		genExportVAO();
	}

	void ImageProcess::genFrameBuffer()
	{
		glGenFramebuffers(1, &FBO1);
		
		glGenTextures(1, &ColorBuffer1);
		glBindTexture(GL_TEXTURE_2D, ColorBuffer1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		
		glGenRenderbuffers(1, &RenderBuffer1);
		glBindRenderbuffer(GL_RENDERBUFFER, RenderBuffer1);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);

		glBindFramebuffer(GL_FRAMEBUFFER, FBO1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorBuffer1, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RenderBuffer1);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("Framebuffer not complete! \n");
			printf("vbo=%d,  vao=%d \n", YUV_VBO, YUV_VAO);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);


		glGenFramebuffers(1, &FBO2);

		glGenTextures(1, &ColorBuffer2);
		glBindTexture(GL_TEXTURE_2D, ColorBuffer2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glGenRenderbuffers(1, &RenderBuffer2);
		glBindRenderbuffer(GL_RENDERBUFFER, RenderBuffer2);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);

		glBindFramebuffer(GL_FRAMEBUFFER, FBO2);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorBuffer2, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RenderBuffer2);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("Framebuffer not complete! \n");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		_currentFBO = FBO1;
	}

	void ImageProcess::genExportVAO()
	{
		struct  Vertex
		{
			glm::vec2  pos;
			glm::vec2  uv;
		};

		Vertex  vertex[] =
		{
			glm::vec2(0,		0),			glm::vec2(0,1),
			glm::vec2(_width,	0),			glm::vec2(1,1),
			glm::vec2(0,		_height),	glm::vec2(0,0),
			glm::vec2(_width,	_height),	glm::vec2(1,0),
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glm::mat4 ImageProcess::getModelMat()
	{
		float winRatio = (float)_winWidth / (float)_winHeight;
		float scale = 0;

		glm::mat4 model = glm::mat4(1.0f);
		if (_videoRatio > winRatio)
		{
			scale = (float)_winWidth / (float)_width;
			model = glm::translate(model, glm::vec3(0, (_winHeight - scale * _height) / 2.0f, 0));
			model = glm::scale(model, glm::vec3(scale));
		}
		else
		{
			scale = (float)_winHeight / (float)_height;
			model = glm::translate(model, glm::vec3((_winWidth - scale * _width) / 2.0f, 0, 0));
			model = glm::scale(model, glm::vec3(scale));
		}
		return model;
	}

	void ImageProcess::setRect(int w, int h)
	{
		_projection		=	glm::ortho(0.0f, (float)w, 0.0f, (float)h);

		_winWidth		=	w;
		_winHeight		=	h;

		glm::mat4 model =	getModelMat();

		_exportShader->begin();
		_exportShader->setMat4("projection", _projection);
		_exportShader->setMat4("model", model);
		_exportShader->end();
	}

	void ImageProcess::pushExportFrame()
	{
		if (_changeSpeed) return;
		if (!_isExport) return;

		int size = _width * (_height + _height / 2);
		std::lock_guard<std::mutex> lock(_mutexExportFrame);
		uint8_t* f = new uint8_t[size];
		memcpy(f, _yuvData, size);
		_exportFramelist.push_back(f);
	}

	uint8_t* ImageProcess::popExportFrame()
	{
		if (_changeSpeed) return nullptr;
		if (!_isExport)
		{
			if(_exportFramelist.empty()) return nullptr;
		}

		if (_exportFramelist.empty()) return nullptr;
		std::lock_guard<std::mutex> lock(_mutexExportFrame);
		uint8_t* frame = _exportFramelist.front();
		_exportFramelist.pop_front();
		return frame;
	}

	void ImageProcess::pushRgb()
	{
		if (!_isExport) return;
		std::lock_guard<std::mutex> lock(_mutexRgb);
		if (!_isExport) return;
		if (_rgbList.size() < 10)
		{
			char* rgb = new char[_width * _height * 3];
			glReadPixels(0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, rgb);
			RecordFrame* frame = new RecordFrame();
			frame->data = rgb;
			_rgbList.push_back(frame);
		}
	}

	RecordFrame* ImageProcess::popRgb()
	{
		if (!_isExport) return nullptr;
		if (_rgbList.empty()) return nullptr;

		std::lock_guard<std::mutex> lock(_mutexRgb);
		RecordFrame* rgb = _rgbList.front();
		_rgbList.pop_front();
		return rgb;
	}

	void ImageProcess::clearExportBuffer()
	{
		std::lock_guard<std::mutex> lock(_mutexRgb);
		while (!_rgbList.empty())
		{
			RecordFrame* rgb = _rgbList.front();
			_rgbList.pop_front();
			delete rgb;
			rgb = nullptr;
		}

		std::lock_guard<std::mutex> lock2(_mutexExportFrame);
		while (!_exportFramelist.empty())
		{
			uint8_t* f = _exportFramelist.front();
			_exportFramelist.pop_front();
			delete f;
			f = nullptr;
		}
	}
}