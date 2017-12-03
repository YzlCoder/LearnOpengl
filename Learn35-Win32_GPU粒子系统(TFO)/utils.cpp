#include "utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

char * LoadFileContent(const char* path, int& filesize)
{
	char* fileContent = nullptr;
	filesize = 0;
	FILE* pFile = fopen(path, "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		int nLen = ftell(pFile);
		if (nLen > 0)
		{
			rewind(pFile);
			fileContent = new char[nLen + 1];
			fread(fileContent, sizeof(char), nLen, pFile);
			fileContent[nLen] = 0;
		}
		fclose(pFile);
	}
	return fileContent;
}

GLuint CompileShader(GLenum shaderType, const char*shaderPath)
{
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0)
	{
		printf("glCreateShader fail\n");
		return 0;
	}
	int fileSize;
	const char* shaderCode = LoadFileContent(shaderPath, fileSize);
	if (shaderCode == nullptr)
	{
		printf("load shader code from file : %s fail\n", shaderPath);
		glDeleteShader(shader);
		return 0;
	}
	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);
	GLint compileResult = GL_TRUE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE)
	{
		char szLog[1024] = { 0 };
		GLsizei logLen = 0;
		glGetShaderInfoLog(shader, 1024, &logLen, szLog);
		printf("Compile Shader fail error log : %s \nshader code :\n%s\n", szLog, shaderCode);
		glDeleteShader(shader);
		shader = 0;
	}
	delete shaderCode;
	return shader;
}

GLuint CreateComputerProgram(const char* path)
{
	GLuint program = glCreateProgram();
	int len;
	char * code = LoadFileContent(path, len);
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

GLuint CreateGPUProgram(const char*vsShaderPath, const char*fsShaderPath, const char*gsPath)
{
	GLuint vsShader = CompileShader(GL_VERTEX_SHADER, vsShaderPath);
	GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, fsShaderPath);
	GLuint program = glCreateProgram();
	glAttachShader(program, vsShader);
	glAttachShader(program, fsShader);
	GLuint gsShader = 0;
	if (gsPath != nullptr)
	{
		gsShader = CompileShader(GL_GEOMETRY_SHADER, gsPath);
		glAttachShader(program, gsShader);
	}
	glLinkProgram(program);
	glDetachShader(program, vsShader);
	glDetachShader(program, fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);
	if (gsPath != nullptr)
	{

		glDetachShader(program, gsShader);
		glDeleteShader(gsShader);
	}
	GLint linkResult = GL_TRUE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
	if (linkResult == GL_FALSE)
	{
		char szLog[1024] = { 0 };
		GLsizei logLen = 0;
		glGetProgramInfoLog(program, 1024, &logLen, szLog);
		printf("link program fail error log : %s \n vs : %s\n fs : %s\n", szLog, vsShaderPath, fsShaderPath);
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


GLuint CreateTransformFeedbackObject(GLuint tfoBuffer)
{
	GLuint tfo;
	glGenTransformFeedbacks(1, &tfo);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfo);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tfoBuffer);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	return tfo;
}

GLuint CreateTFOProgram(const char*vsShaderPath, const char* const*attris, int nCount, GLenum memoryFormat, const char*gsPath /* = nullptr */)
{
	GLuint vsShader = CompileShader(GL_VERTEX_SHADER, vsShaderPath);
	GLuint program = glCreateProgram();
	glAttachShader(program, vsShader);
	GLuint gsShader = 0;
	if (gsPath != nullptr)
	{
		gsShader = CompileShader(GL_GEOMETRY_SHADER, gsPath);
		glAttachShader(program, gsShader);
	}
	glTransformFeedbackVaryings(program, nCount, attris, memoryFormat);
	glLinkProgram(program);
	glDetachShader(program, vsShader);
	glDeleteShader(vsShader);
	if (gsPath != nullptr)
	{

		glDetachShader(program, gsShader);
		glDeleteShader(gsShader);
	}
	GLint linkResult = GL_TRUE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
	if (linkResult == GL_FALSE)
	{
		char szLog[1024] = { 0 };
		GLsizei logLen = 0;
		glGetProgramInfoLog(program, 1024, &logLen, szLog);
		printf("link program fail error log : %s \n vs : %s\n", szLog, vsShaderPath);
		glDeleteProgram(program);
		program = 0;
	}
	return program;
}