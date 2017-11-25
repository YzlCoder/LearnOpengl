#pragma once
#include "VertexBuffer.h"
#include "Shader.h"

class ParticleSystem
{
	VertexBuffer* mVertexBuffer;
	glm::mat4 mModeMatrix;
	Shader *mShader;
public:
	ParticleSystem() : mShader(nullptr), mVertexBuffer(nullptr) {}
	~ParticleSystem() { delete mShader; delete mVertexBuffer; }
	void Init(float x, float y, float z);
	void Draw(glm::mat4 & viewMat, glm::mat4 & projectMat);
	void Update(float deltaTime);
};

