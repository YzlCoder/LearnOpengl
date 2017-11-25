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

GLuint CreateProgram(GLuint vsShader, GLuint fsShader)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vsShader);
	glAttachShader(program, fsShader);
	glLinkProgram(program);
	glDetachShader(program, vsShader);
	glDetachShader(program, fsShader);
	GLint nResult;
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

GLuint CreateTexture(unsigned char* data, int width, int height, int mode)
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, mode, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

GLuint loadTexture(const GLchar* path)
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height, nrChannels;
	unsigned char* image = stbi_load(path, &width, &height, &nrChannels, 0);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image);
	return textureID;

}

GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void *data)
{
	GLuint object;
	glGenBuffers(1, &object);
	glBindBuffer(bufferType, object);
	glBufferData(bufferType, size, data, usage);
	glBindBuffer(bufferType, 0);
	return object;
}


GLuint CreateProcedureTexture(int size)
{
	unsigned char *imageData = new unsigned char[size * size * 4];
	float halfSize = (float)size / 2.0f;
	float maxDistance = sqrtf(halfSize * halfSize + halfSize * halfSize);
	float centerX = halfSize;
	float centerY = halfSize;

	for (int y = 0; y < size; ++y)
	{
		for (int x = 0; x < size; ++x)
		{
			int currentPixelOffset = (x + y * size) * 4;
			imageData[currentPixelOffset] = 255;
			imageData[currentPixelOffset + 1] = 255;
			imageData[currentPixelOffset + 2] = 255;
			float deltaX = (float)x - centerX;
			float deltaY = (float)y - centerY;
			float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
			float alpha = pow(1.0f - (distance / maxDistance), 8.0f);
			alpha = alpha > 1.0f ? 1.0f : alpha;
			imageData[currentPixelOffset + 3] = (unsigned char)(alpha * 255.0f);
		} 
	}
	GLuint texture = CreateTexture(imageData, size, size, GL_RGBA);
	delete imageData;
	return texture;
}