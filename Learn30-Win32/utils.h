#pragma once
#include "ggl.h"

unsigned char * LoadFileContent(const char* path, int& filesize);

GLuint CompileShader(GLenum shaderType, const char* charCode);

GLuint CreateProgram(GLuint vsShader, GLuint fsShader);

float GetFramTime();

GLuint loadTexture(GLchar* path);