#include "utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char * LoadFileContent(const char* path, int& filesize)
{
	unsigned char* fileContent = nullptr;
	filesize = 0;
	FILE* pFile = fopen(path, "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		int nLen = ftell(pFile);
		if (nLen > 0)
		{
			rewind(pFile);
			fileContent = new unsigned char[nLen + 1];
			fread(fileContent, sizeof(unsigned char), nLen, pFile);
			fileContent[nLen] = 0;
		}
		fclose(pFile);
	}
	return fileContent;
}

GLuint CompileShader(GLenum shaderType, const char* shaderCode)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);
	GLint compileResult = GL_TRUE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE)
	{
		char szLog[1024] = { 0 };
		GLsizei logLen = 0;
		glGetShaderInfoLog(shader, 1024, &logLen, szLog);
		printf("Compile Shder fail error log: %s\nshader code :%s\n", szLog, shaderCode);
		glDeleteShader(shader);
	}
	return shader;
}

GLuint CreateComputerProgram(const char* path)
{
	GLuint program = glCreateProgram();
	int len;
	unsigned char * code = LoadFileContent(path, len);
	GLuint shader = CompileShader(GL_COMPUTE_SHADER, (char*)code);
	glAttachShader(program, shader);
	glLinkProgram(program);
	glDetachShader(program, shader);
	glDeleteShader(shader);
	GLint nResult = GL_TRUE;
	glGetProgramiv(program, GL_LINK_STATUS, &nResult);
	if (nResult == GL_FALSE)
	{
		char log[1024] = { 0 };
		GLsizei writed = 0;
		glGetProgramInfoLog(program, 1024, &writed, log);
		printf("create computer program fail, link error : %s\n", log);
		glDeleteProgram(program);
		program = 0;
	}
	return program;
}

GLuint CreateProgram(GLuint vsShader, GLuint fsShader)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vsShader);
	glAttachShader(program, fsShader);
	glLinkProgram(program);
	glDetachShader(program, vsShader);
	glDetachShader(program, fsShader);
	GLint nResult = GL_TRUE;
	glGetProgramiv(program, GL_LINK_STATUS, &nResult);
	if (nResult == GL_FALSE)
	{
		char log[1024] = { 0 };
		GLsizei writed = 0;
		glGetProgramInfoLog(program, 1024, &writed, log);
		printf("create gpu program fail, link error : %s\n", log);
		glDeleteProgram(program);
		program = 0;
	}
	return program;
}

float GetFramTime()
{
	static unsigned long lastTime = 0, timeSinceComputerStart = 0;
	timeSinceComputerStart = timeGetTime();
	unsigned long frameTime = lastTime == 0 ? 0 : timeSinceComputerStart - lastTime;
	lastTime = timeSinceComputerStart;
	return float(frameTime);
}

unsigned char* LoadTexture(GLchar* path, int& width, int& height)
{
	int nrChannels;
	unsigned char* image = stbi_load(path, &width, &height, &nrChannels, 0);
	return image;
}

void ReleaseTexture(unsigned char* image)
{
	stbi_image_free(image);
}

GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void*data /* = nullptr */)
{
	GLuint object;
	glGenBuffers(1, &object);
	glBindBuffer(bufferType, object);
	glBufferData(bufferType, size, data, usage);
	glBindBuffer(bufferType, 0);
	return object;
}