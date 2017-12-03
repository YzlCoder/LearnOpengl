#pragma once
#include "ggl.h"

char * LoadFileContent(const char* path, int& filesize);

GLuint CompileShader(GLenum shaderType, const char*shaderPath);

GLuint CreateComputerProgram(const char* path);

GLuint CreateGPUProgram(const char*vsShaderPath, const char*fsShaderPath, const char*gsPath = nullptr);

float GetFramTime();

unsigned char* LoadTexture(GLchar* path, int& width, int& height);

void ReleaseTexture(unsigned char*);

GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void*data = nullptr);

GLuint CreateTransformFeedbackObject(GLuint tfoBuffer);

GLuint CreateTFOProgram(const char*vsShaderPath, const char* const*attris, int nCount, GLenum memoryFormat, const char*gsPath = nullptr);