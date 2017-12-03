#include "Scene.h"
#include "ggl.h"
#include "ShaderTool.h"
#include <cmath>
#include "utils.h"
#include "Timer.h"

struct VertexData
{
	float v[4];
};

Shader* shader = nullptr;
GLint	modelLocation, viewLocation, projectLocation;
glm::mat4 model, view, project;
GLuint ssbo, ssbov, ibo, cshader, noiseTex;
int vertexCount, indexCount;
GLuint loadTexture(GLchar* path);
float frand();
float sfrand();

Timer timer;
float lastPass = 0;


GLuint CreateTexture3D(int w, int h, int d)
{
	char*data = new char[w*h*d * 4];
	char*temp = data;
	for (int i = 0; i < w; ++i)
	{
		for (int ii = 0; ii < h; ++ii)
		{
			for (int iii = 0; iii < d; ++iii)
			{
				*temp++ = rand() & 0xff;
				*temp++ = rand() & 0xff;
				*temp++ = rand() & 0xff;
				*temp++ = rand() & 0xff;
			}
		}
	}
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_3D, texture);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8_SNORM, w, h, d, 0, GL_RGBA, GL_BYTE, data);
	glBindTexture(GL_TEXTURE_3D, 0);
	delete[] data;
	return texture;
}

void Init()
{
	shader = new Shader("PointSpriteSSBO.vs", "PointSprite.fs");
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f)); // Translate it down a bit so it's at the center of the scene
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	vertexCount = 1 << 19;
	indexCount = vertexCount * 6;

	VertexData* vertexes = new VertexData[vertexCount];
	VertexData* velocity = new VertexData[vertexCount];
	unsigned int* indexes = new unsigned int[indexCount];
	unsigned int* temp = indexes;
	for (int i = 0; i < vertexCount; ++i)
	{
		vertexes[i].v[0] = sfrand();
		vertexes[i].v[1] = sfrand();
		vertexes[i].v[2] = sfrand();
		vertexes[i].v[3] = 1.0f;

		velocity[i].v[0] = 0;
		velocity[i].v[1] = 0;
		velocity[i].v[2] = 0;
		velocity[i].v[3] = 1.0f;

	
		unsigned int idx = unsigned int(i << 2);

		*(temp++) = idx;
		*(temp++) = idx + 1;
		*(temp++) = idx + 2;
		*(temp++) = idx;
		*(temp++) = idx + 2;
		*(temp++) = idx + 3;

	}
	
	ssbo = CreateBufferObject(GL_SHADER_STORAGE_BUFFER, sizeof(VertexData) * vertexCount, GL_STATIC_DRAW, vertexes);
	ssbov = CreateBufferObject(GL_SHADER_STORAGE_BUFFER, sizeof(VertexData) * vertexCount, GL_STATIC_DRAW, velocity);
	ibo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, GL_STATIC_DRAW, indexes);

	cshader = CreateComputerProgram("test.computer");
	noiseTex = CreateTexture3D(16, 16, 16);
	delete vertexes;
	delete indexes;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}


void SetViewPortSize(float width, float height)
{
	project = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
}
void Draw()
{
	printf("%d\n", (int)(1 / (timer.GetPassTime() - lastPass)));
	lastPass = timer.GetPassTime();
	glUseProgram(cshader);
	glBindTexture(GL_TEXTURE_3D, noiseTex);
	glUniform1i(glGetUniformLocation(cshader, "U_MainTexture"), 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbov);
	glDispatchCompute(vertexCount / 128, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->Use();
	shader->setMat4("P", project);
	shader->setMat4("V", view);
	shader->setMat4("M", model);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glUseProgram(0);

}

float frand()
{
	return rand() / (float)RAND_MAX;
}

float sfrand()
{
	return frand() * 2 - 1.0f;
}