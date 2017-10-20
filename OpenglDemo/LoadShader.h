#pragma once

/**********
* loadshader.h
**********/
#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#define GLFW_NO_GLU 1


#include <Windows.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string.h>

#define GLFW_INCLUDE_GLCOREARB 1

GLuint LoadShader(const char *shader_path, GLenum shader_type);
void LinkShader(GLuint shader_pragram, ...);  /// “‘0Ω· ¯
bool compileShader(GLuint &shader_id, const char *path);

static const GLchar *getFileData(const char * path);

static const int MAX_LOG_LEN = 16 * 1024;