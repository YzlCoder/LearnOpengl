#pragma once
#include "ggl.h"
struct Vertex
{
	float Position[4];
	float Color[4];
	float Texcoord[4];
	float Normal[4];
};

class VertexBuffer
{
public:
	Vertex *mVertexs;
	int mVertexCount;
	GLuint mVBO;
	VertexBuffer() :mVertexs(nullptr) {}
	~VertexBuffer() { delete mVertexs; }
	void SetSize(int vertexCount);
	void SetPosition(int index, float x, float y, float z, float w = 1.0f);
	void SetColor(int index, float r, float g, float b, float a = 1.0f);
	void SetTexcoord(int index, float x, float y);
	void SetNormal(int index, float x, float y, float z);
	void Bind();
	void Unbind();
	Vertex& Get(int index);
};

