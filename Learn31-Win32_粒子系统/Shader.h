#pragma once
#include "ggl.h"

struct UniformTexture
{
	GLint mLocation;
	GLuint mTexture;
	UniformTexture()
	{
		mLocation = -1;
		mTexture = 0;
	}
};

struct UniformVector4f
{
	GLint mLocation;
	float v[4];
	UniformVector4f()
	{
		mLocation = -1;
		memset(v, 0, sizeof(v));
	}
};



class Shader
{
public:

	~Shader();
	GLuint mProgram;
	GLint mModelLocation, mViewLocation, mProjectLocation;
	GLint mPostionLocation, mColorLocation, mTexcoordLocation, mNormalLocation;
	std::map<std::string, UniformTexture *> mUniformTextures;
	std::map<std::string, UniformVector4f*> mUniformVec4s;

	void Init(const char* vs, const char* fs);
	void Bind(float* M, float *V, float *P);
	void SetTexture(const char* name, const char* imagePath);
	void SetTexture(const char* name, GLuint texture);
	void SetVec4(const char* name, float x, float y, float z, float w);
};

