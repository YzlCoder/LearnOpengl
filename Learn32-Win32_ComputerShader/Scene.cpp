#include "Scene.h"
#include "ggl.h"
#include "utils.h"


GLuint vbo;
GLuint program;
GLint positionLocation, colorLocation, modelLocation, viewLocation, projectLocation;
glm::mat4 model, view, project;
GLuint texture,textureLocation;
void Init()
{
	GLfloat data[] = {
		// Î»ÖÃ          // ÑÕÉ«
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
	};
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	int filesize = 0;
	unsigned char * shaderCode = LoadFileContent("Res/shaders/test.vs", filesize);
	GLuint vsShader = CompileShader(GL_VERTEX_SHADER, (char*)shaderCode);
	delete shaderCode;

	shaderCode = LoadFileContent("Res/shaders/test.frag", filesize);
	GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, (char*)shaderCode);
	delete shaderCode;

	program = CreateProgram(vsShader, fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);

	positionLocation = glGetAttribLocation(program, "position");
	colorLocation = glGetAttribLocation(program, "texcoord");
	modelLocation = glGetUniformLocation(program, "model");
	viewLocation = glGetUniformLocation(program, "view");
	projectLocation = glGetUniformLocation(program, "project");
	textureLocation = glGetUniformLocation(program, "texture2d");
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.6f));

	texture = loadTexture("Res/textures/awesomeface.png");
}

void SetViewPortSize(float width, float height)
{
	project = glm::perspective(glm::radians(60.0f), width / height, 0.1f, 1000.0f);
}

void Draw()
{
	float frameTime = GetFramTime();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectLocation, 1, GL_FALSE, glm::value_ptr(project));
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(4 * sizeof(float)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glUniform1i(textureLocation, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}