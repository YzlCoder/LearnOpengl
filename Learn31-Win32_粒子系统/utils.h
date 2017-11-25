#pragma once
#include "ggl.h"

unsigned char * LoadFileContent(const char* path, int& filesize);

GLuint CompileShader(GLenum shaderType, const char* charCode);

GLuint CreateProgram(GLuint vsShader, GLuint fsShader);

float GetFramTime();

GLuint loadTexture(const GLchar* path);

GLuint CreateTexture(unsigned char* data, int width, int height, int mode);

GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void *data = nullptr);

GLuint CreateProcedureTexture(int size);