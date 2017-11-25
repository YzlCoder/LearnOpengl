#include "Shader.h"
#include "utils.h"
#include "VertexBuffer.h"

Shader::~Shader()
{
	for (auto texture : mUniformTextures)
	{
		glDeleteTextures(1, &texture.second->mTexture);
		delete texture.second;
	}

	for (auto vec4 : mUniformVec4s)
	{
		delete vec4.second;
	}
}


void Shader :: Init(const char* vs, const char* fs)
{
	int nFileSize = 0;
	const char * vsCode = (char*)LoadFileContent(vs, nFileSize);
	const char *fsCode = (char*)LoadFileContent(fs, nFileSize);
	GLuint vsShader = CompileShader(GL_VERTEX_SHADER, vsCode);
	if (vsShader == 0)
	{
		return;
	}
	GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, fsCode);
	if (fsShader == 0)
	{
		return;
	}
	mProgram = CreateProgram(vsShader, fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);
	if (mProgram != 0)
	{
		mModelLocation = glGetUniformLocation(mProgram, "model");
		mViewLocation = glGetUniformLocation(mProgram, "view");
		mProjectLocation = glGetUniformLocation(mProgram, "project");

		mPostionLocation = glGetAttribLocation(mProgram, "position");
		mColorLocation = glGetAttribLocation(mProgram, "color");
		mTexcoordLocation = glGetAttribLocation(mProgram, "texcoord");
		mNormalLocation = glGetAttribLocation(mProgram, "normal");
	}
}


void Shader::Bind(float* M, float *V, float *P)
{
	glUseProgram(mProgram);

	glUniformMatrix4fv(mModelLocation, 1, GL_FALSE, M);
	glUniformMatrix4fv(mViewLocation, 1, GL_FALSE, V);
	glUniformMatrix4fv(mProjectLocation, 1, GL_FALSE, P);
	int iIndex = 0;

	for (auto iter = mUniformTextures.begin(); iter != mUniformTextures.end(); ++iter)
	{
		glActiveTexture(GL_TEXTURE0 + iIndex);
		glBindTexture(GL_TEXTURE_2D, iter->second->mTexture);
		glUniform1i(iter->second->mLocation, iIndex ++);
	}

	for (auto iter = mUniformVec4s.begin(); iter != mUniformVec4s.end(); ++iter)
	{
		glUniform4fv(iter->second->mLocation, 1, iter->second->v);
	}
	
	glEnableVertexAttribArray(mPostionLocation);
	glVertexAttribPointer(mPostionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(mColorLocation);
	glVertexAttribPointer(mColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 4));
	glEnableVertexAttribArray(mTexcoordLocation);
	glVertexAttribPointer(mTexcoordLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 8));
	glEnableVertexAttribArray(mNormalLocation);
	glVertexAttribPointer(mNormalLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 12));

}
void Shader::SetTexture(const char* name, const char* imagePath)
{
	auto iter = mUniformTextures.find(name);
	if (iter == mUniformTextures.end())
	{
		GLuint locaiton = glGetUniformLocation(mProgram, name);
		if (locaiton != -1)
		{
			UniformTexture* t = new UniformTexture;
			t->mLocation = locaiton;
			t->mTexture = loadTexture(imagePath);
			mUniformTextures.insert(std::make_pair(name, t));
		}
	}
	else
	{
		glDeleteTextures(1, &iter->second->mTexture);
		iter->second->mTexture = loadTexture(imagePath);
	}
}

void Shader::SetTexture(const char* name, GLuint texture)
{
	auto iter = mUniformTextures.find(name);
	if (iter == mUniformTextures.end())
	{
		GLuint locaiton = glGetUniformLocation(mProgram, name);
		if (locaiton != -1)
		{
			UniformTexture* t = new UniformTexture;
			t->mLocation = locaiton;
			t->mTexture = texture;
			mUniformTextures.insert(std::make_pair(name, t));
		}
	}
	else
	{
		glDeleteTextures(1, &iter->second->mTexture);
		iter->second->mTexture = texture;
	}
}

void Shader::SetVec4(const char* name, float x, float y, float z, float w)
{
	auto iter = mUniformVec4s.find(name);
	if (iter == mUniformVec4s.end())
	{
		GLuint location = glGetUniformLocation(mProgram, name);
		if (location != -1)
		{
			UniformVector4f *v = new UniformVector4f;
			v->v[0] = x;
			v->v[1] = y;
			v->v[2] = z;
			v->v[3] = w;
			v->mLocation = location;
			mUniformVec4s.insert(std::make_pair(name, v));
		}
	}
	else
	{
		iter->second->v[0] = x;
		iter->second->v[1] = y;
		iter->second->v[2] = z;
		iter->second->v[3] = w;
	}
}