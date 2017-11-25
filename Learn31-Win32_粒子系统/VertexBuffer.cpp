#include "VertexBuffer.h"
#include "utils.h"

void VertexBuffer::SetSize(int vertexCount)
{
	mVertexCount = vertexCount;
	mVertexs = new Vertex[mVertexCount];
	memset(mVertexs, 0, sizeof(Vertex) * mVertexCount);
	mVBO = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(Vertex) * mVertexCount, GL_DYNAMIC_DRAW, nullptr);
}
void VertexBuffer::SetPosition(int index, float x, float y, float z, float w)
{
	mVertexs[index].Position[0] = x;
	mVertexs[index].Position[1] = y;
	mVertexs[index].Position[2] = z;
	mVertexs[index].Position[3] = w;
}
void VertexBuffer::SetColor(int index, float r, float g, float b, float a)
{
	mVertexs[index].Color[0] = r;
	mVertexs[index].Color[1] = g;
	mVertexs[index].Color[2] = b;
	mVertexs[index].Color[3] = a;
}
void VertexBuffer::SetTexcoord(int index, float x, float y)
{
	mVertexs[index].Texcoord[0] = x;
	mVertexs[index].Texcoord[1] = y;
}
void VertexBuffer::SetNormal(int index, float x, float y, float z)
{
	mVertexs[index].Normal[0] = x;
	mVertexs[index].Normal[1] = y;
	mVertexs[index].Normal[2] = z;
}
void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * mVertexCount, mVertexs);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Vertex& VertexBuffer::Get(int index)
{
	return mVertexs[index];
}

