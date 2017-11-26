#include "Scene.h"
#include "ggl.h"
#include "Model.h"
#include "ShaderTool.h"

GLuint program;
Shader* shader = nullptr;
Model* obj = nullptr;
GLint positionLocation, colorLocation, modelLocation, viewLocation, projectLocation;
glm::mat4 model, view, project;

void Init()
{
	shader = new Shader("SSBO.vert", "model_loading.frag");
	obj = new Model("nanosuit/nanosuit.obj");
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
}


void SetViewPortSize(float width, float height)
{
	project = glm::perspective(glm::radians(60.0f), width / height, 0.1f, 1000.0f);
}
void Draw()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->Use();
	shader->setMat4("projection", project);
	shader->setMat4("view", view);
	shader->setMat4("model", model);
	obj->Draw(*shader);
}