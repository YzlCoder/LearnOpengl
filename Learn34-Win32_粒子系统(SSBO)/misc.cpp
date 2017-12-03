#include "misc.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>




GLuint CreateVAOWithVBOSettings(std::function<void()>settings)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	settings();
	glBindVertexArray(0);
	return vao;
}

GLuint CreateFramebufferObject(GLuint&colorBuffer, GLuint&depthBuffer,int width,int height, GLuint *colorBuffer2)
{
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//color buffer
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	if (colorBuffer2!=nullptr)
	{
		glGenTextures(1, colorBuffer2);
		glBindTexture(GL_TEXTURE_2D, *colorBuffer2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, *colorBuffer2, 0);
		GLenum buffers[] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
		glDrawBuffers(2, buffers);
	}
	//depth buffer
	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

	GLenum status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status!=GL_FRAMEBUFFER_COMPLETE)
	{
		printf("create framebuffer object fail\n");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return fbo;
}

char *LoadFileContent(const char*path)
{
	FILE*pFile = fopen(path, "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		int nLen = ftell(pFile);
		char*buffer = nullptr;
		if (nLen!=0)
		{
			buffer=new char[nLen + 1];
			rewind(pFile);
			fread(buffer, nLen, 1, pFile);
			buffer[nLen] = '\0';
		}
		else
		{
			printf("load file fail %s content len is 0\n", path);
		}
		fclose(pFile);
		return buffer;
	}
	else
	{
		printf("open file %s fail\n",path);
	}
	fclose(pFile);
	return nullptr;
}


GLuint CompileShader(GLenum shaderType, const char*shaderPath)
{
	GLuint shader = glCreateShader(shaderType);
	if (shader==0)
	{
		printf("glCreateShader fail\n");
		return 0;
	}
	const char* shaderCode = LoadFileContent(shaderPath);
	if (shaderCode==nullptr)
	{
		printf("load shader code from file : %s fail\n",shaderPath);
		glDeleteShader(shader);
		return 0;
	}
	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);
	GLint compileResult = GL_TRUE;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&compileResult);
	if (compileResult==GL_FALSE)
	{
		char szLog[1024] = {0};
		GLsizei logLen = 0;
		glGetShaderInfoLog(shader,1024,&logLen,szLog);
		printf("Compile Shader fail error log : %s \nshader code :\n%s\n",szLog,shaderCode);
		glDeleteShader(shader);
		shader = 0;
	}
	delete shaderCode;
	return shader;
}


GLuint CreateGPUProgram(const char*vsShaderPath, const char*fsShaderPath)
{
	GLuint vsShader = CompileShader(GL_VERTEX_SHADER, vsShaderPath);
	GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER,fsShaderPath);
	GLuint program = glCreateProgram();
	glAttachShader(program, vsShader);
	glAttachShader(program, fsShader);
	glLinkProgram(program);
	glDetachShader(program, vsShader);
	glDetachShader(program, fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);
	GLint linkResult = GL_TRUE;
	glGetProgramiv(program,GL_LINK_STATUS,&linkResult);
	if (linkResult==GL_FALSE)
	{
		char szLog[1024] = { 0 };
		GLsizei logLen = 0;
		glGetProgramInfoLog(program, 1024, &logLen, szLog);
		printf("link program fail error log : %s \n vs : %s\n fs : %s\n", szLog, vsShaderPath,fsShaderPath);
		glDeleteProgram(program);
		program = 0;
	}
	return program;
}

GLuint CreateComputeProgram(const char*computeShaderPath)
{
	GLuint computeShader = CompileShader(GL_COMPUTE_SHADER, computeShaderPath);
	GLuint program = glCreateProgram();
	glAttachShader(program, computeShader);
	glLinkProgram(program);
	glDetachShader(program, computeShader);
	glDeleteShader(computeShader);
	GLint linkResult = GL_TRUE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
	if (linkResult == GL_FALSE)
	{
		char szLog[1024] = { 0 };
		GLsizei logLen = 0;
		glGetProgramInfoLog(program, 1024, &logLen, szLog);
		printf("link program fail error log : %s \n vs : %s\n", szLog, computeShaderPath);
		glDeleteProgram(program);
		program = 0;
	}
	return program;
}

unsigned char*DecodeBMPData(unsigned char*imgData,int&width,int&height)
{
	int pixelDataOffset = *((int*)(imgData + 10));
	width = *((int*)(imgData + 18));
	height = *((int*)(imgData + 22));
	unsigned char*pixelData = (imgData + pixelDataOffset);
	for (int i = 0; i < width*height * 3; i += 3)
	{
		//bgr->rgb
		unsigned char temp = pixelData[i + 2];
		pixelData[i + 2] = pixelData[i];
		pixelData[i] = temp;
	}
	return pixelData;
}

const unsigned long FORMAT_DXT1 = 0x31545844l;//DXT1 -> 1 T X D
unsigned char*DecodeDXT1Data(unsigned char*imgData, int&width, int&height,int&pixelDataSize)
{
	height = *(unsigned long*)(imgData + sizeof(unsigned long) * 3);
	width = *(unsigned long*)(imgData + sizeof(unsigned long) * 4);
	pixelDataSize= *(unsigned long*)(imgData + sizeof(unsigned long) * 5);
	unsigned long compressFormat;
	compressFormat = *(unsigned long*)(imgData + sizeof(unsigned long) * 21);
	switch (compressFormat)
	{
	case FORMAT_DXT1:
		break;
	}
	unsigned char*pixelData = new unsigned char[pixelDataSize];
	memcpy(pixelData,imgData + sizeof(unsigned long) * 32,pixelDataSize);

	return pixelData;
}

void SaveImage(const char*imagePath, unsigned char*imgData, int width, int height)
{
	FILE*pFile = fopen(imagePath, "wb");
	if (pFile)
	{
		BITMAPFILEHEADER bfh;
		memset(&bfh,0,sizeof(BITMAPFILEHEADER));
		bfh.bfType = 0x4D42;
		bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width*height * 3;
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,pFile);

		BITMAPINFOHEADER bih;
		memset(&bih, 0, sizeof(BITMAPINFOHEADER));
		bih.biWidth = width;
		bih.biHeight = height;
		bih.biBitCount = 24;
		bih.biSize = sizeof(BITMAPINFOHEADER);
		fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, pFile);
		unsigned char temp = 0;
		for (int i=0;i<width*height*3;i+=3)
		{
			temp = imgData[i + 2];
			imgData[i + 2] = imgData[i];
			imgData[i] = temp;
		}
		fwrite(imgData,1,width*height*3,pFile);
		fclose(pFile);
	}
}

GLuint CreateTextureFromFile(const char*imagePath)
{
	unsigned char*imgData = (unsigned char*)LoadFileContent(imagePath);

	int width = 0;
	int height = 0;
	unsigned char*pixelData = nullptr;
	int pixelDataSize = 0;
	GLenum srcFormat = GL_RGB;
	if (*((unsigned short*)imgData) == 0x4D42)
	{
		pixelData = DecodeBMPData(imgData, width, height);
	}
	else if (memcmp(imgData,"DDS ",4)==0)
	{
		pixelData = DecodeDXT1Data(imgData, width, height, pixelDataSize);
		srcFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	}

	if (pixelData==nullptr)
	{
		delete imgData;
		return 0;
	}

	//decode bmp
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (srcFormat==GL_RGB)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
	}
	else if(srcFormat==GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
	{
		glCompressedTexImage2D(GL_TEXTURE_2D,0,srcFormat,width,height,0, pixelDataSize,pixelData);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	delete imgData;
	return texture;
}


void CheckGLError(const char*file, int line)
{

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		switch (error)
		{
		case  GL_INVALID_ENUM:
			printf("GL Error GL_INVALID_ENUM %s : %d\n",file,line);
			break;
		case  GL_INVALID_VALUE:
			printf("GL Error GL_INVALID_VALUE %s : %d\n", file, line);
			break;
		case  GL_INVALID_OPERATION:
			printf("GL Error GL_INVALID_OPERATION %s : %d\n", file, line);
			break;
		case  GL_STACK_OVERFLOW:
			printf("GL Error GL_STACK_OVERFLOW %s : %d\n", file, line);
			break;
		case  GL_STACK_UNDERFLOW:
			printf("GL Error GL_STACK_UNDERFLOW %s : %d\n", file, line);
			break;
		case  GL_OUT_OF_MEMORY:
			printf("GL Error GL_OUT_OF_MEMORY %s : %d\n", file, line);
			break;
		default:
			printf("GL Error 0x%x %s : %d\n", error, file, line);
			break;
		}
	}
}