/**********
* loadshader.cpp
**********/
#include "LoadShader.h"
#include <fstream>
#include <vector>


GLuint LoadShader(const char *shader_path, GLenum shader_type)
{
	GLuint shader_id = glCreateShader(shader_type);

	bool bRet = compileShader(shader_id, shader_path);
	if (!bRet)
	{
		glDeleteShader(shader_id);
		return 0;
	}

	return shader_id;
}

static const GLchar* getFileData(const char * path)
{
	FILE* infile;
	fopen_s(&infile, path, "rb");
	if (!infile)
	{
		return NULL;
	}
	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	GLchar *source = new GLchar[len + 1];
	fread(source, 1, len, infile);
	fclose(infile);
	source[len] = 0;
	return const_cast<const GLchar *>(source);
}

bool compileShader(GLuint &shader_id, const char *path)
{
	const GLchar *shader_code = getFileData(path);
	if (strlen(shader_code) <= 0)
		return 0;

	GLint Result = GL_FALSE;
	GLint InfoLogLength;

	glShaderSource(shader_id, 1, &shader_code, NULL);
	glCompileShader(shader_id);
	delete[]shader_code;

	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &Result);
	if (!Result) {
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(shader_id, InfoLogLength, NULL, &ShaderErrorMessage[0]);
		printf("%s\n", &ShaderErrorMessage[0]);
		return false;
	}

	return true;
}

bool LoadTessellationShader(GLuint shader_program, const char *tes_shader_path)
{
	GLuint shader = glCreateShader(GL_TESS_CONTROL_SHADER);
	bool bRet = compileShader(shader, tes_shader_path);
	if (!bRet)
		return false;
	glAttachShader(shader_program, shader);
	glLinkProgram(shader_program);

	glDeleteProgram(shader);

	return true;
}

void LinkShader(GLuint shader_pragram, ...)
{
	va_list args;
	va_start(args, shader_pragram);
	while (true)
	{
		GLuint shader_id = va_arg(args, GLuint);
		if (shader_id <= 0)
			break;
		glAttachShader(shader_pragram, shader_id);
		glDeleteShader(shader_id);
	}
	va_end(args);
	glLinkProgram(shader_pragram);
}
