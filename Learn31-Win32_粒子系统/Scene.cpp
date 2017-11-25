#include "Scene.h"
#include "ggl.h"
#include "utils.h"
#include "ParticleSystem.h"

glm::mat4 view, project;
ParticleSystem ps;

void Init()
{
	ps.Init(400.0f, 300.0f, 0.0f);
}

void SetViewPortSize(float width, float height)
{
	project = glm::ortho(0.0f, static_cast<GLfloat>(width),
		static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
	//project = glm::perspective(glm::radians(60.0f), width / height, 0.1f, 1000.0f);
}

void Draw()
{
	float frameTime = GetFramTime();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ps.Update(frameTime);
	ps.Draw(view, project);

}