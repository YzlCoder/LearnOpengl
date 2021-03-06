#include "Scene.h"
#include "ggl.h"
#include "utils.h"
#include "Timer.h"

GLuint vbo;
GLuint program;
GLint positionLocation, colorLocation, modelLocation, viewLocation, projectLocation;
glm::mat4 model, project;
GLuint texture,textureLocation;


GLuint Fanse_1(unsigned char* image, int width, int height);
GLuint Fanse_2(unsigned char* image, int width, int height);

void Init()
{
	glGetError();

	GLfloat data[] = {
		// 位置          // 颜色
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
	projectLocation = glGetUniformLocation(program, "project");
	textureLocation = glGetUniformLocation(program, "texture2d");
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.6f));

	int width, height;
	unsigned char* image = LoadTexture("Res/textures/awesomeface.jpg", width, height);

	// CPU 反色  0.002s
	//texture = Fanse_1(image, width, height);

	// GPU 反色 0.00004s
	texture = Fanse_2(image, width, height);

}

GLuint Fanse_1(unsigned char* image, int width, int height)
{
	Timer timer;
	timer.Start();
	for (int i = 0; i < width * height * 3; ++i)
		image[i] = 255 - image[i];
	printf("time cost %f\n", timer.GetPassTime());

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

GLuint Fanse_2(unsigned char* image, int width, int height)
{
	GLuint texture, computerDstTexture;
	glGenTextures(1, &computerDstTexture);
	glBindTexture(GL_TEXTURE_2D, computerDstTexture);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glBindTexture(GL_TEXTURE_2D, 0);
	ReleaseTexture(image);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint computerProgram = CreateComputerProgram("Res/shaders/test.computer");
	Timer timer;
	timer.Start();
	glUseProgram(computerProgram);
	glBindImageTexture(0, computerDstTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
	glBindImageTexture(1, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glDispatchCompute(width / 16, height / 16, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	printf("time cost %f\n", timer.GetPassTime());
	return texture;
}

void SetViewPortSize(float width, float height)
{
	project = glm::perspective(glm::radians(60.0f), width / height, 0.1f, 1000.0f);
}

void Draw()
{
	float frameTime = GetFramTime();
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(projectLocation, 1, GL_FALSE, glm::value_ptr(project));
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(4 * sizeof(float)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}