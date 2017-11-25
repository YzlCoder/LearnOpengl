#include "ParticleSystem.h"
#include "utils.h"

void ParticleSystem::Init(float x, float y, float z)
{
	mModeMatrix = glm::translate(mModeMatrix, glm::vec3(x, y, z));
	mVertexBuffer = new VertexBuffer;

	int particleCount = 180;
	mVertexBuffer->SetSize(particleCount); 
	for (int i = 0; i < particleCount; ++i)
	{
		mVertexBuffer->SetPosition(i, 
			50.0f * cos(float(i) * 8.0 * 3.14f / 180.0f), 
			50.0f * sin(float(i) * 8.0f * 3.14f / 180.0f), 
				0.0f);
		mVertexBuffer->SetColor(i, 0.1f, 0.4f, 0.6f);
	}


	mShader = new Shader;
	mShader->Init("Res/shaders/particle.vs", "Res/shaders/particle.frag");
	mShader->SetTexture("U_Texture", CreateProcedureTexture(128));
}

void ParticleSystem::Draw(glm::mat4 & viewMat, glm::mat4 & projectMat)
{
	glEnable(GL_POINT_SPRITE);    //点精灵
	glEnable(GL_PROGRAM_POINT_SIZE);  //启用程序控制点精灵大小
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	mVertexBuffer->Bind();

	mShader->Bind(glm::value_ptr(mModeMatrix), glm::value_ptr(viewMat), glm::value_ptr(projectMat));
	glDrawArrays(GL_POINTS, 0, mVertexBuffer->mVertexCount);
	mVertexBuffer->Unbind();
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void ParticleSystem::Update(float deltaTime)
{
	static float angle = 0.0f;
	angle += deltaTime * 10.0f;
	//mModeMatrix = glm::rotate(mModeMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));

	for (int i = 0; i < mVertexBuffer->mVertexCount; ++i)
	{
		Vertex& vertex = mVertexBuffer->Get(i);
		vertex.Position[1] += 0.0006f * (rand() % (i + 1));
		vertex.Position[0] += (rand() % 31 - 15) * 0.02f;
	}

}