#pragma once
#include "ggl.h"

unsigned char * LoadFileContent(const char* path, int& filesize);

GLuint CompileShader(GLenum shaderType, const char* charCode);

GLuint CreateComputerProgram(const char* path);

GLuint CreateProgram(GLuint vsShader, GLuint fsShader);

float GetFramTime();

unsigned char* LoadTexture(GLchar* path, int& width, int& height);

void ReleaseTexture(unsigned char*);